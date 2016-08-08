// Fill out your copyright notice in the Description page of Project Settings.

#include "XComClone.h"
#include "TileMap.h"
#include "Unit.h"
#include "XComCloneGameState.h"
#include "XComCloneHUD.h"

#include "Engine.h"
#include "Editor.h"

// Sets default values
ATileMap::ATileMap()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;	

	BaseRoot = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	BaseRoot->SetMobility(EComponentMobility::Static);
	BaseRoot->SetWorldLocation(FVector(0.f, 0.f, 0.f));	
	RootComponent = BaseRoot;

	
	RowCount = 14;
	ColumnCount = 15;

	mSelectedTile = nullptr;

	bIsUnitMoving = false;
	bFoundPath = false;
}

// Called when the game starts or when spawned
void ATileMap::BeginPlay()
{
	Super::BeginPlay();	

	for(FConstPawnIterator it = GetWorld()->GetPawnIterator(); it; ++it)
	{
		AUnit *worldUnit = Cast<AUnit>(*it);

		if(worldUnit)
		{
			worldUnit->OnUnitMovementBegin().AddUObject(this, &ATileMap::OnUnitMovementBegin);
			worldUnit->OnUnitMovementEnd().AddUObject(this, &ATileMap::OnUnitMovementEnd);
			worldUnit->OnUnitStateChange().AddUObject(this, &ATileMap::OnUnitStateChange);
		}
	}
	
	AXComCloneGameState * const gameState = GetWorld()->GetGameState<AXComCloneGameState>();
	if(gameState)
	{
		gameState->OnTurnChanged().AddUObject(this, &ATileMap::OnTurnChange);
	}
}

void ATileMap::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

// Called every frame
void ATileMap::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
}

void ATileMap::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	RowCount = FPlatformMath::Max<int32>(1, RowCount);
	ColumnCount = FPlatformMath::Max<int32>(1, ColumnCount);

	if(RowCount != mPreviousRowCount || ColumnCount != mPreviousColumnCount)
	{
		// create or destroy tiles to fill the grid
		AdjustNumberOfTiles();
		mPreviousRowCount = RowCount;
		mPreviousColumnCount = ColumnCount;
	}
}

void ATileMap::Destroyed()
{
	Super::Destroyed();

	for(ATile *Tile : mTilesArray)
	{
		if(Tile && Tile->IsValidLowLevel()) { Tile->Destroy(); }
	}
	mTilesArray.Empty();

	for(APathActor *pathActor : mPathArray)
	{
		if(pathActor && pathActor->IsValidLowLevel()) { pathActor->Destroy(); }
	}
	mPathArray.Empty();
}

void ATileMap::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	for(ATile* Tile : mTilesArray)
	{
		Tile->OnTileClicked().AddUObject(this, &ATileMap::OnTileClicked);
		Tile->OnBeginTileCursorOver().AddUObject(this, &ATileMap::OnBeginTileCursorOver);
		Tile->OnEndTileCursorOver().AddUObject(this, &ATileMap::OnEndTileCursorOver);
	}
}

void ATileMap::AdjustNumberOfTiles()
{
	int32 NewSize = ColumnCount * RowCount;
	int32 OldSize = mPreviousRowCount * mPreviousColumnCount;

	UPROPERTY() TArray<ATile*> NewTiles;
	NewTiles.SetNum(NewSize, true);

	// create the new grid
	for(int32 X = 0; X < ColumnCount; X++)
	{
		for(int32 Y = 0; Y < RowCount; Y++)
		{
			if(X < mPreviousColumnCount && Y < mPreviousRowCount)
			{
				NewTiles[Y * ColumnCount + X] = mTilesArray[Y * mPreviousColumnCount + X];
				mTilesArray[Y * mPreviousColumnCount + X] = NULL; // all remaining pointers are destroyed later 
			}
			else
			{
				FActorSpawnParameters fp;
				fp.bAllowDuringConstructionScript = true;
				UPROPERTY() ATile *Tile = GetWorld()->SpawnActor<ATile>(ATile::StaticClass(), fp);
				Tile->SetOwner(this);
				GEditor->ParentActors(this, Tile, NAME_None);
				

				//set position
				Tile->setMapPosition(X, Y);

				FVector Position = GetActorLocation() + FVector(Tile->MapX * ATile::getSize().X, Tile->MapY * ATile::getSize().Y, 0.f);
				Tile->SetActorLocation(Position);

				NewTiles[Y * ColumnCount + X] = Tile;
			}
		}
	}
	// clean up the old grid, 
	for(ATile *Tile : mTilesArray)
	{
		if(Tile) Tile->Destroy();
	}

	mTilesArray = NewTiles;
}

void ATileMap::OnTileClicked(ATile* tile)
{
	if(bIsUnitMoving) { return; }

	if(!tile) { return; }


	if(mSelectedTile == tile)
	{
		deselectTile(mSelectedTile->getUnitOnTile()->getUnitRange());
	}
	else if(mSelectedTile)
	{
		if(tile->TileMode == ETileMode::EMPTY &&
		   mSelectedTile->getUnitOnTile()->UnitState == EUnitState::MOVING) //move to tile along path
		{
			TArray<ATile*> rangeTiles;
			findTilesInRange(*mSelectedTile, rangeTiles, mSelectedTile->getUnitOnTile()->getUnitRange(), false);

			if(rangeTiles.Contains(tile))
			{
				AUnit* unitOnSelectedTile = mSelectedTile->getUnitOnTile();
				if(unitOnSelectedTile)
				{
					if(bFoundPath)
					{
						unitOnSelectedTile->moveToLocation(mPathSteps);
						removePath();
					}
				}

				deselectTile(rangeTiles.Num());
				selectTile(tile);
			}
		}
		else if(tile->TileMode == ETileMode::ENEMY &&
				mSelectedTile->getUnitOnTile()->UnitState == EUnitState::ATTACKING) // mSelectedTile unit attack tile unit
		{
			TArray<ATile*> rangeTiles;
			findTilesInRange(*mSelectedTile, rangeTiles, mSelectedTile->getUnitOnTile()->getUnitRange(), true);

			if(rangeTiles.Contains(tile))
			{
				mSelectedTile->getUnitOnTile()->attack(*tile->getUnitOnTile());
			}
		}
		else if(tile->TileMode == ETileMode::ALLY)
		{
			deselectTile(mSelectedTile->getUnitOnTile()->getUnitRange());
			selectTile(tile);
		}
	}
	else if(tile->getUnitOnTile())
	{
		selectTile(tile);
	}
}
void ATileMap::OnBeginTileCursorOver(ATile* tile)
{
	//if mSelected Tile then create path from selectedTile to tile
	if(!tile) { return; }

	if(bIsUnitMoving)
	{
		tile->deactivate();
		return;
	}

	if(mSelectedTile)
	{
		TArray<ATile*> rangeTiles;
		findTilesInRange(*mSelectedTile, rangeTiles, mSelectedTile->getUnitOnTile()->getUnitRange(), mSelectedTile->getUnitOnTile()->UnitState == EUnitState::ATTACKING ? true : false); //if attacking add enemy tiles to range

		if(rangeTiles.Contains(tile))
		{
			/*if(tile->getUnitOnTile())
			{
			}
			else*/
			if(mSelectedTile->getUnitOnTile()->UnitState == EUnitState::MOVING)
			{			
				TArray<FVector> path;
				bFoundPath = findPath(*tile, path);
				if(bFoundPath)
				{
					mPathSteps.Empty();
					mPathSteps.Append(path);
					drawPath();
				}
			}
			else if(mSelectedTile->getUnitOnTile()->UnitState == EUnitState::ATTACKING)
			{
				if(tile->TileMode != ETileMode::ENEMY &&
				   mSelectedTile != tile)
				{
					tile->deactivate();
				}
			}
		}
		else if(tile->TileMode != ETileMode::ALLY )
		{
			tile->deactivate();
		}
	}
}

void ATileMap::OnEndTileCursorOver(ATile* tile)
{
	//if mSelected Tile then delete path from selectedTile to taile
	if(tile)
	{
		//bFoundPath = false; //tmp??

		removePath();
	}
}

void ATileMap::OnUnitMovementBegin(const AUnit* unit)
{
	if(unit)
	{
		bIsUnitMoving = true;
	}
}

void ATileMap::OnUnitMovementEnd(const AUnit* unit)
{
	if(!unit) { return; }

	bIsUnitMoving = false;

	if(!mSelectedTile) { return; }

	mSelectedTile->setTileMode(ETileMode::ALLY);

	TArray<ATile*> rangeTiles;

	findTilesInRange(*mSelectedTile, rangeTiles, mSelectedTile->getUnitOnTile()->getUnitRange(), mSelectedTile->getUnitOnTile()->UnitState == EUnitState::ATTACKING ? true : false);

	for(size_t i = 0; i < rangeTiles.Num(); i++)
	{
		if(mSelectedTile->getUnitOnTile()->UnitState == EUnitState::MOVING)
		{
			rangeTiles[i]->setInMovementRangeTileColor();
		}
		else if(mSelectedTile->getUnitOnTile()->UnitState == EUnitState::ATTACKING)
		{
			rangeTiles[i]->setInFireRangeTileColor();
		}
	}

	AXComCloneHUD *myHud = Cast<AXComCloneHUD>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD());
	if(myHud)
	{
		myHud->setActiveUnit(mSelectedTile->getUnitOnTile());
	}
}

void ATileMap::OnUnitStateChange(const AUnit* unit, int32 previousRange)
{
	if(!unit) { return; }
	if(!mSelectedTile) { return; }
	if(!mSelectedTile->getUnitOnTile()) { return; }


	TArray<ATile*> rangeTiles;
	findTilesInRange(*mSelectedTile, rangeTiles, previousRange, mSelectedTile->getUnitOnTile()->UnitState == EUnitState::ATTACKING ? true : false);

	for(size_t i = 0; i < rangeTiles.Num(); i++)
	{
		rangeTiles[i]->setStandardTileColor();
	}


	findTilesInRange(*mSelectedTile, rangeTiles, mSelectedTile->getUnitOnTile()->getUnitRange(), mSelectedTile->getUnitOnTile()->UnitState == EUnitState::ATTACKING ? true : false);

	for(size_t i = 0; i < rangeTiles.Num(); i++)
	{
		if(mSelectedTile->getUnitOnTile()->UnitState == EUnitState::MOVING)
		{
			rangeTiles[i]->setInMovementRangeTileColor();
		}
		else if(mSelectedTile->getUnitOnTile()->UnitState == EUnitState::ATTACKING)
		{
			rangeTiles[i]->setInFireRangeTileColor();
		}
	}
}

void ATileMap::OnTurnChange(const EPlayerId nextPlayerTurn)
{
	if(!mSelectedTile) { return; }

	deselectTile(mSelectedTile->getUnitOnTile()->getUnitRange());
}

void ATileMap::selectTile(ATile * tile)
{
	if(mSelectedTile == tile) { return; }


	if(tile->TileMode == ETileMode::ALLY)
	{
		TArray<ATile*> rangeTiles;
		findTilesInRange(*tile, rangeTiles, tile->getUnitOnTile()->getUnitRange(), tile->getUnitOnTile()->UnitState == EUnitState::ATTACKING ? true : false);

		for(size_t i = 0; i < rangeTiles.Num(); i++)
		{
			if(tile->getUnitOnTile()->UnitState == EUnitState::MOVING)
			{
				rangeTiles[i]->setInMovementRangeTileColor();
			}
			else if(tile->getUnitOnTile()->UnitState == EUnitState::ATTACKING)
			{
				rangeTiles[i]->setInFireRangeTileColor();
			}
		}

		AXComCloneHUD *myHud = Cast<AXComCloneHUD>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD());
		if(myHud)
		{
			myHud->setActiveUnit(tile->getUnitOnTile());
		}

		tile->activate();
		mSelectedTile = tile;
	}
	else if(tile->TileMode == ETileMode::EMPTY)
	{
		tile->activate();
		mSelectedTile = tile;
	}
}

void ATileMap::deselectTile(int32 tileRange)
{
	if(mSelectedTile)
	{
		TArray<ATile*> rangeTiles;
		findTilesInRange(*mSelectedTile, rangeTiles, tileRange, mSelectedTile->getUnitOnTile()->UnitState == EUnitState::ATTACKING ? true : false);
		
		for(size_t i = 0; i < rangeTiles.Num(); i++)
		{
			rangeTiles[i]->setStandardTileColor();
		}

		AXComCloneHUD *myHud = Cast<AXComCloneHUD>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD());
		if(myHud)
		{
			myHud->deactivateUnit();
		}

		mSelectedTile->deactivate();
		mSelectedTile = nullptr;
	}
}

void ATileMap::getTileNeighbours(const ATile& tile, TArray<ATile*>& outArray)
{
	outArray.Empty();
	outArray.Reserve(4); //max 4 Neighbours

	if(tile.MapX - 1 >= 0) { outArray.Add(mTilesArray[tile.MapY*ColumnCount + (tile.MapX - 1)]); }
	if(tile.MapX + 1 < ColumnCount) { outArray.Add(mTilesArray[tile.MapY*ColumnCount + (tile.MapX + 1)]); }
	if(tile.MapY - 1 >= 0) { outArray.Add(mTilesArray[(tile.MapY - 1)*ColumnCount + tile.MapX ]); }
	if(tile.MapY + 1 < RowCount) { outArray.Add(mTilesArray[(tile.MapY + 1)*ColumnCount + tile.MapX]); }
}

void ATileMap::findTilesInRange(ATile& startTile, TArray<ATile*>& outArray, int32 range, bool addEnemyTileToRange)
{	
	outArray.Empty();
	outArray.Reserve(range * (2 * range + 2)); //max number of tiles in range
	
	TQueue<ATile*> tileQueue;
	TArray<ATile*> tileNeighbours;

	ETileMode extraCheck = addEnemyTileToRange ? ETileMode::ENEMY : ETileMode::EMPTY;

	tileQueue.Enqueue(&startTile);
	outArray.AddUnique(&startTile);
	int32 queueSize = 1;
	int32 AddedToQueue = 0;

	for(size_t i = 0; i < range; i++)
	{
		for(size_t j = 0; j < queueSize; j++)
		{
			ATile* t;
			tileQueue.Dequeue(t);

			getTileNeighbours(*t, tileNeighbours);

			for(size_t k = 0; k < tileNeighbours.Num(); k++)
			{
				if((tileNeighbours[k]->TileMode == ETileMode::EMPTY || tileNeighbours[k]->TileMode == extraCheck) &&
				   !outArray.Contains(tileNeighbours[k]))
				{
					tileQueue.Enqueue(tileNeighbours[k]);
					++AddedToQueue;

					outArray.AddUnique(tileNeighbours[k]);
				}
			}
		}
		
		queueSize = AddedToQueue;
		AddedToQueue = 0;
	}
}

bool ATileMap::findPath(ATile& destination, TArray<FVector>& outArray)
{
	if(mSelectedTile == &destination) { return false; }

	if(mSelectedTile)
	{
		if(!mSelectedTile->getUnitOnTile()) { return false; } //find path only if there are unit on selected tile
	}

	TArray<ATile*> selectedTileRange;
	findTilesInRange(*mSelectedTile, selectedTileRange, mSelectedTile->getUnitOnTile()->getUnitRange(), false); //for pathfinding we need movement range and no enemy tiles

	if(!selectedTileRange.Contains(&destination)) { return false; } // find path only if destination is in movement range

	outArray.Empty();

	TQueue<ATile*> tileQueue;
	TArray<ATile*> tileNeighbours;
	TMap<ATile*, int32> pathValues;
	ATile *tmp = &destination;

	int32 queueSize = 1;
	int32 AddedToQueue = 0;
	int accualValue = 1;

	tileQueue.Enqueue(tmp);
	pathValues.Add(tmp, accualValue);
	++accualValue;

	bool findPath = true;
	while(findPath)
	{
		for(size_t j = 0; j < queueSize; j++)
		{
			tileQueue.Dequeue(tmp);
			getTileNeighbours(*tmp, tileNeighbours);

			for(size_t i = 0; i < tileNeighbours.Num(); i++)
			{
				if(tileNeighbours[i] == mSelectedTile)
				{
					pathValues.Add(tileNeighbours[i], accualValue);

					findPath = false; //mSelectedTile found, stop looking futher
					j = queueSize; //ugly
					break;
				}

				if(tileNeighbours[i]->TileMode == ETileMode::EMPTY &&
				   selectedTileRange.Contains(tileNeighbours[i]) &&
				   !pathValues.Contains(tileNeighbours[i]))
				{
					tileQueue.Enqueue(tileNeighbours[i]);
					++AddedToQueue;

					pathValues.Add(tileNeighbours[i], accualValue);
				}
			}
		}
		queueSize = AddedToQueue;
		AddedToQueue = 0;
		++accualValue;
	}

	accualValue -= 2; // because of the last incrementation
	tmp = mSelectedTile;

	while(accualValue != 0)
	{
		getTileNeighbours(*tmp, tileNeighbours);
		
		size_t i;
		for(i = 0; i < tileNeighbours.Num(); i++)
		{
			int32 *result = pathValues.Find(tileNeighbours[i]);

			if(result && *result == accualValue)
			{
				break;
			}

		}

		outArray.Add(tileNeighbours[i]->getCenterInWorldLocation());
		tmp = tileNeighbours[i];
		--accualValue;
	}

	return true;
}

void ATileMap::drawPath()
{
	if(bFoundPath && mSelectedTile && mPathSteps.Num() > 0)
	{
		for(size_t i = 0; i < mPathSteps.Num(); i++)
		{
			if(mPathArray.Num() > i)
			{
				mPathArray[i]->SetActorLocation(mPathSteps[i]);
				mPathArray[i]->SetVisibility(true);
			}
			else
			{
				UPROPERTY() APathActor *pathActor = GetWorld()->SpawnActor<APathActor>(mPathSteps[i], FRotator::ZeroRotator);
				pathActor->SetOwner(this);
				mPathArray.Add(pathActor);
			}
		}
	}
}

void ATileMap::removePath()
{
	for(APathActor *pathActor : mPathArray)
	{
		if(pathActor) { pathActor->SetVisibility(false); }
	}
}