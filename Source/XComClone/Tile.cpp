// Fill out your copyright notice in the Description page of Project Settings.

#include "XComClone.h"
#include "Tile.h"

#include "Engine.h"

// Sets default values
ATile::ATile()
{
	static ConstructorHelpers::FObjectFinder<UStaticMesh> TileMeshAsset(TEXT("StaticMesh'/Game/StarterContent/Architecture/Floor_100x100.Floor_100x100'"));

	TileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TileMesh"));
	TileMesh->SetupAttachment(RootComponent);
	TileMesh->SetMobility(EComponentMobility::Static);
	
	if(TileMeshAsset.Succeeded())
	{
		TileMesh->SetStaticMesh(TileMeshAsset.Object);
	}

	Cost = 1;
	

 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;

	//this->OnBeginCursorOver.Add(&ATile::doOnMouseOver);

	//TileMesh->OnBeginCursorOver.Add(&ATile::doOnMouseOver);
}

// Called when the game starts or when spawned
void ATile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATile::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

const FVector ATile::getSize()const
{
	return TileMesh->StaticMesh->GetBoundingBox().GetSize();
}

//void ATile::doOnMouseOver(UPrimitiveComponent* TouchedComponent)
//{
//	GEngine->AddOnScreenDebugMessage(0, 1.f, FColor::Red, TEXT("Tile clicked"));
//}