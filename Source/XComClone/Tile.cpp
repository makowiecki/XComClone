// Fill out your copyright notice in the Description page of Project Settings.

#include "XComClone.h"
#include "Tile.h"

#include "Engine.h"

// Sets default values
ATile::ATile()
{
	static ConstructorHelpers::FObjectFinder<UStaticMesh> TileMeshAsset(TEXT("StaticMesh'/Game/StarterContent/Architecture/Floor_100x100.Floor_100x100'"));
	static ConstructorHelpers::FObjectFinder<UMaterial> TileIndicatorAsset(TEXT("Material'/Game/Materials/DecalTileIndicator.DecalTileIndicator'"));

	TileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TileMesh"));
	TileMesh->SetupAttachment(RootComponent);
	TileMesh->SetMobility(EComponentMobility::Static);
		
	if(TileMeshAsset.Succeeded())
	{
		TileMesh->SetStaticMesh(TileMeshAsset.Object);
	}

	TileIndicator = CreateDefaultSubobject<UDecalComponent>(TEXT("TileIndicator"));
	TileIndicator->SetupAttachment(TileMesh);
	TileIndicator->SetRelativeLocationAndRotation(FVector(getSize().X / 2, getSize().Y / 2, 0.f), FRotator(90.f, 0.f, 0.f));
	TileIndicator->DecalSize = FVector(5.f, getSize().X / 2, getSize().Y / 2);
	TileIndicator->SetMobility(EComponentMobility::Static);
	TileIndicator->SetVisibility(false);

	if(TileIndicatorAsset.Succeeded())
	{
		TileIndicator->SetDecalMaterial(TileIndicatorAsset.Object);
	}

	
	FScriptDelegate OnClickedDelegate;
	OnClickedDelegate.BindUFunction(this, "OnMouseClicked");
	TileMesh->OnClicked.Add(OnClickedDelegate);

	FScriptDelegate OnBeginMouseOverDelegate;
	OnBeginMouseOverDelegate.BindUFunction(this, "OnBeginMouseOver");
	TileMesh->OnBeginCursorOver.Add(OnBeginMouseOverDelegate);

	FScriptDelegate OnEndMouseOverDelegate;
	OnEndMouseOverDelegate.BindUFunction(this, "OnEndMouseOver");
	TileMesh->OnEndCursorOver.Add(OnEndMouseOverDelegate);


	Cost = 1;
	

 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;
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

const FVector ATile::getSize()
{
	return FVector(100.f, 100.f, 5.f);
	//return TileMesh->StaticMesh->GetBoundingBox().GetSize();
}

void ATile::setMapPosition(int32 pX, int32 pY)
{
	MapX = pX;
	MapY = pY;
}

void ATile::OnMouseClicked(UPrimitiveComponent * clickedComponent)
{
	FString msg = "Mouse clicked (" + FString::FromInt(MapX) + "; " + FString::FromInt(MapY) + ")";
	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 1.f, FColor::Red, msg, false);
}

void ATile::OnBeginMouseOver(UPrimitiveComponent* TouchedComponent)
{
	//FString msg = "Mouse begin over (" + FString::FromInt(MapX) + "; " + FString::FromInt(MapY) + ")";
	//GEngine->AddOnScreenDebugMessage(INDEX_NONE, 1.f, FColor::Red, msg, false);
	TileIndicator->SetVisibility(true);
}

void ATile::OnEndMouseOver(UPrimitiveComponent * pComponent)
{
	//FString msg = "Mouse end over (" + FString::FromInt(MapX) + "; " + FString::FromInt(MapY) + ")";
	//GEngine->AddOnScreenDebugMessage(INDEX_NONE, 1.f, FColor::Red, msg, false);
	TileIndicator->SetVisibility(false);
}
