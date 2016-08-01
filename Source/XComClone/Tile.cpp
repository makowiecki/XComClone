// Fill out your copyright notice in the Description page of Project Settings.

#include "XComClone.h"
#include "Tile.h"
#include "Unit.h"
#include "XComCloneGameState.h"

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

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	BoxComponent->SetupAttachment(TileMesh);
	BoxComponent->SetRelativeLocation(FVector(getSize().X / 2, getSize().Y / 2, 0.f));
	BoxComponent->InitBoxExtent(FVector(getSize().X / 2, getSize().Y / 2, 10.f));
	BoxComponent->bGenerateOverlapEvents = true;


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

	bTileClicked = false;
	TileMode = ETileMode::EMPTY;
	mUnitOnTile = nullptr;

 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
}

// Called when the game starts or when spawned
void ATile::BeginPlay()
{
	Super::BeginPlay();

	AXComCloneGameState * const MyGameState = GetWorld()->GetGameState<AXComCloneGameState>();

	if(MyGameState)
	{
		MyGameState->OnTurnChanged().AddUObject(this, &ATile::OnTurnChanged);
	}
}

// Called every frame
void ATile::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
	
	//{ DEBUG
	//	switch(TileMode)
	//	{
	//		case ETileMode::EMPTY:
	//			//TileMeshMaterial->SetVectorParameterValue(FName(TEXT("Color")), FLinearColor(0.f, 0.f, 0.5f));
	//			break;
	//		case ETileMode::BLOCKED:
	//			TileMeshMaterial->SetVectorParameterValue(FName(TEXT("Color")), FLinearColor(0.f, 0.f, 0.f));
	//			break;
	//		case ETileMode::ENEMY:
	//			TileMeshMaterial->SetVectorParameterValue(FName(TEXT("Color")), FLinearColor(1.f, 0.f, 0.f));
	//			break;
	//		case ETileMode::ALLY:
	//			TileMeshMaterial->SetVectorParameterValue(FName(TEXT("Color")), FLinearColor(0.f, 1.f, 0.f));
	//			break;
	//		default:
	//			break;
	//	}
	//}

	if(bChangedTileMode)
	{
		bChangedTileMode = false;

		switch(TileMode)
		{
			case ETileMode::EMPTY:
				TileIndicatorMaterial->SetVectorParameterValue(FName(TEXT("Color")), FLinearColor(0.f, 0.f, 0.5f));
				break;
			case ETileMode::ENEMY:
				TileIndicatorMaterial->SetVectorParameterValue(FName(TEXT("Color")), FLinearColor(1.f, 0.f, 0.f));
				break;
			case ETileMode::ALLY:
				TileIndicatorMaterial->SetVectorParameterValue(FName(TEXT("Color")), FLinearColor(0.f, 1.f, 0.f));
				break;
			default:
				break;
		}
	}
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

bool ATile::isActive() const
{
	return bTileClicked;
}

void ATile::activate()
{
	bTileClicked = true;
	TileIndicator->SetVisibility(true);
}

void ATile::deactivate()
{
	bTileClicked = false;
	TileIndicator->SetVisibility(false);
}

AUnit * ATile::getUnitOnTile() const
{
	return mUnitOnTile;
}

void ATile::setTileMode(ETileMode mode)
{
	if(TileMode != mode && TileMode != ETileMode::BLOCKED) //don't change tile mode if it is blocked
	{
		bChangedTileMode = true;
		TileMode = mode;
	}
}

void ATile::setStandardTileColor()
{
	if(TileMeshMaterial)
	{
		TileMeshMaterial->SetVectorParameterValue(TEXT("Color"), FLinearColor(0.26f, 0.32f, 0.38f)); //todo: add UPROPERTY
	}
}

void ATile::setInMovementRangeTileColor()
{
	if(TileMeshMaterial)
	{
		TileMeshMaterial->SetVectorParameterValue(TEXT("Color"), FLinearColor(0.16f, 0.19f, 0.26f)); //todo: add UPROPERTY
	}
}

void ATile::setInFireRangeTileColor()
{
	if(TileMeshMaterial)
	{
		TileMeshMaterial->SetVectorParameterValue(TEXT("Color"), FLinearColor(0.42f, 0.20f, 0.21f)); //todo: add UPROPERTY
	}
}

const FVector ATile::getCenterInWorldLocation() const
{
	FVector ret = GetActorLocation() + FVector(getSize().X / 2, getSize().Y / 2, 0.f);
	return ret;
}

ATile::FOnTileClicked& ATile::OnTileClicked()
{
	return mTileClickedEvent;
}

ATile::FOnBeginTileCursorOver& ATile::OnBeginTileCursorOver()
{
	return mBeginTileCursorOverEvent;
}

ATile::FOnEndTileCursorOver& ATile::OnEndTileCursorOver()
{
	return mEndTileCursorOverEvent;
}

void ATile::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	AUnit* unit = Cast<AUnit>(OtherActor);
	AXComCloneGameState * const MyGameState = GetWorld()->GetGameState<AXComCloneGameState>();

	if(unit && !mUnitOnTile)
	{
		mUnitOnTile = unit;

		if(MyGameState)
		{
			if(MyGameState->isPlayerTurn(mUnitOnTile->PlayerId))
			{
				setTileMode(ETileMode::ALLY);
			}
			else
			{
				setTileMode(ETileMode::ENEMY);
			}
		}
	}
}

void ATile::NotifyActorEndOverlap(AActor* OtherActor)
{
	Super::NotifyActorEndOverlap(OtherActor);

	AUnit* unit = Cast<AUnit>(OtherActor);

	if(unit && unit == mUnitOnTile)
	{
		mUnitOnTile = nullptr;

		setTileMode(ETileMode::EMPTY);
	}
}

void ATile::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if(TileIndicator->GetDecalMaterial())
	{
		TileIndicatorMaterial = TileIndicator->CreateDynamicMaterialInstance();
	}

	if(TileMesh->GetMaterial(0))
	{
		TileMeshMaterial = TileMesh->CreateDynamicMaterialInstance(0);
	}
}

void ATile::OnMouseClicked(UPrimitiveComponent * clickedComponent)
{

	mTileClickedEvent.Broadcast(this);
}

void ATile::OnBeginMouseOver(UPrimitiveComponent* TouchedComponent)
{
	TileIndicator->SetVisibility(TileMode == ETileMode::BLOCKED ? false : true);

	mBeginTileCursorOverEvent.Broadcast(this);
}

void ATile::OnEndMouseOver(UPrimitiveComponent * pComponent)
{

	if(!bTileClicked)
	{
		TileIndicator->SetVisibility(false);
	}

	mEndTileCursorOverEvent.Broadcast(this);
}

void ATile::OnTurnChanged(EPlayerId newPlayer)
{
	if(mUnitOnTile)
	{
		if(mUnitOnTile->PlayerId == newPlayer)
		{
			setTileMode(ETileMode::ALLY);
		}
		else
		{
			setTileMode(ETileMode::ENEMY);
		}
	}
}

