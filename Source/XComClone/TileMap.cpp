// Fill out your copyright notice in the Description page of Project Settings.

#include "XComClone.h"
#include "TileMap.h"
#include "Unit.h"

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
		}
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

	if(tile)
	{
		if(mSelectedTile == tile)
		{
			deselectTile();
		}
		else if(mSelectedTile)
		{
			if(tile->TileMode == ETileMode::EMPTY) //move to tile along path
			{
				AUnit* unitOnSelectedTile = mSelectedTile->getUnitOnTile();
				if(unitOnSelectedTile)
				{
					unitOnSelectedTile->moveToLocation(tile->getCenterInWorldLocation());

				}
			}
			else if(tile->TileMode == ETileMode::ENEMY) // mselecctertile unit attack tile unit
			{

			}

			deselectTile();
			selectTile(tile);
		}
		else if(tile->getUnitOnTile())
		{
			selectTile(tile);
		}

	}
}

void ATileMap::OnBeginTileCursorOver(ATile* tile)
{
	//if mSelected Tile then create path from selectedTile to tile
	if(tile)
	{
		if(bIsUnitMoving) 
		{
			tile->deactivate();
			return; 
		}

		if(mSelectedTile)
		{
			TArray<ATile*> rangeTiles;
			findTilesInUnitMovementRange(*mSelectedTile, rangeTiles);
			
			if(rangeTiles.Contains(tile))
			{
				if(tile->getUnitOnTile())
				{
					if(tile->getUnitOnTile()->isAlly(*mSelectedTile->getUnitOnTile()))
					{
						tile->setTileMode(ETileMode::ALLY);
					}
					else
					{
						tile->setTileMode(ETileMode::ENEMY);
					}
				}
				else
				{
					//find path
					//draw path
				}
			}
		}
	}
}

void ATileMap::OnEndTileCursorOver(ATile* tile)
{
	//if mSelected Tile then delete path from selectedTile to taile
	if(tile)
	{

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
	if(unit)
	{
		bIsUnitMoving = false;
		mSelectedTile->setTileMode(ETileMode::ALLY);

		TArray<ATile*> rangeTiles;
		findTilesInUnitMovementRange(*mSelectedTile, rangeTiles);

		for(size_t i = 0; i < rangeTiles.Num(); i++)
		{
			rangeTiles[i]->setInMovementRangeTileColor();
		}
	}
}

void ATileMap::selectTile(ATile * tile)
{
	if(mSelectedTile != tile)
	{
		mSelectedTile = tile;
		tile->activate();


		TArray<ATile*> rangeTiles;
		findTilesInUnitMovementRange(*tile, rangeTiles);

		for(size_t i = 0; i < rangeTiles.Num(); i++)
		{
			rangeTiles[i]->setInMovementRangeTileColor();
		}
	}
}

void ATileMap::deselectTile()
{
	if(mSelectedTile)
	{
		TArray<ATile*> rangeTiles;
		findTilesInUnitMovementRange(*mSelectedTile, rangeTiles);

		for(size_t i = 0; i < rangeTiles.Num(); i++)
		{
			rangeTiles[i]->setStandardTileColor();
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

void ATileMap::findTilesInUnitMovementRange(ATile& tileContainingUnit, TArray<ATile*>& outArray)
{
	if(!tileContainingUnit.getUnitOnTile()) { return; }

	const int32 UnitRange = tileContainingUnit.getUnitOnTile()->MovementRangeInTiles;
	
	outArray.Empty();
	outArray.Reserve(UnitRange * (2 * UnitRange + 2)); //max number of tiles in range
	
	TQueue<ATile*> tileQueue;
	TArray<ATile*> tileNeighbours;

	tileQueue.Enqueue(&tileContainingUnit);
	int32 queueSize = 1;
	int32 AddedToQueue = 0;

	for(size_t i = 0; i < UnitRange; i++)
	{

		for(size_t j = 0; j < queueSize; j++)
		{
			ATile* t;
			tileQueue.Dequeue(t);

			if(t->TileMode != ETileMode::BLOCKED)
			{
				getTileNeighbours(*t, tileNeighbours);

				for(size_t k = 0; k < tileNeighbours.Num(); k++, ++AddedToQueue)
				{
					tileQueue.Enqueue(tileNeighbours[k]);
					outArray.AddUnique(tileNeighbours[k]);
				}
			}
		}
		
		queueSize = AddedToQueue;
		AddedToQueue = 0;
	}
}
