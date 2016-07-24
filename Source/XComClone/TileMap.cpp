// Fill out your copyright notice in the Description page of Project Settings.

#include "XComClone.h"
#include "TileMap.h"
#include "Tile.h"


// Sets default values
ATileMap::ATileMap()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;

	BaseRoot = CreateDefaultSubobject<USphereComponent>(TEXT("RootComponent"));
	BaseRoot->SetupAttachment(RootComponent);
	BaseRoot->SetMobility(EComponentMobility::Static);
	BaseRoot->SetWorldLocation(FVector(0.f, 0.f, 0.f));
	
	RowCount = 14;
	ColumnCount = 15;
	
	mTilesArray.Reserve(RowCount * ColumnCount);

	for(size_t i = 0; i < RowCount * ColumnCount; i++)
	{
		FString CompName = FString(TEXT("Tile")).Append(FString::FromInt(i));
		UChildActorComponent* newComp = CreateDefaultSubobject<UChildActorComponent>(FName(*CompName));
		newComp->SetMobility(EComponentMobility::Static);
		newComp->SetChildActorClass(ATile::StaticClass());
		newComp->SetupAttachment(BaseRoot);
		newComp->SetRelativeLocation(FVector(i % ColumnCount * 100, i / ColumnCount * 100, 0.f));

		mTilesArray.Add(newComp);
	}
	

}

// Called when the game starts or when spawned
void ATileMap::BeginPlay()
{
	Super::BeginPlay();
	
}

void ATileMap::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	//for(size_t i = 0; i < mTilesArray.Num(); i++)
	//{
	//	mTilesArray[i]->Destroy();
	//}

	//mTilesArray.Empty();
}

// Called every frame
void ATileMap::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

void ATileMap::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
	//for(size_t i = 0; i < mTilesArray.Num(); i++)
	//{
	//	mTilesArray[i]->Destroy();
	//}

	//mTilesArray.Empty();

	//mTilesArray.Reserve(RowCount* ColumnCount);

	//for(size_t i = 0; i < RowCount*ColumnCount; i++)
	//{
	//	ATile *newTile = GetWorld()->SpawnActor<ATile>(FVector(0, 0, 0), FRotator::ZeroRotator);
	//	newTile->SetActorLocation(FVector(i % ColumnCount * newTile->getSize().X, i / ColumnCount * newTile->getSize().Y, 0.f));

	//	mTilesArray.Add(newTile);
	//}

}

void ATileMap::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	//for(size_t i = 0; i < mTilesArray.Num(); i++)
	//{
	//	mTilesArray[i]->Destroy();
	//}

	//mTilesArray.Empty();

	//mTilesArray.Reserve(RowCount* ColumnCount);


	//for(size_t i = 0; i < RowCount*ColumnCount; i++)
	//{
	//	ATile *newTile = GetWorld()->SpawnActor<ATile>(FVector(0, 0, 0), FRotator::ZeroRotator);
	//	newTile->SetActorLocation(FVector(i % ColumnCount * newTile->getSize().X, i / ColumnCount * newTile->getSize().Y, 0.f));

	//	mTilesArray.Add(newTile);
	//}

	
}
