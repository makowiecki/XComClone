// Fill out your copyright notice in the Description page of Project Settings.

#include "XComClone.h"
#include "TileMap.h"

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

	
	RowCount = 4;//14;
	ColumnCount = 3;// 15;
}

// Called when the game starts or when spawned
void ATileMap::BeginPlay()
{
	Super::BeginPlay();	
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

				FVector Position = GetActorLocation() + FVector(Tile->MapX * Tile::getSize().X, Tile->MapY * Tile::getSize().Y, 0.f);
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