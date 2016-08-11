// Fill out your copyright notice in the Description page of Project Settings.

#include "XComClone.h"
#include "Unit.h"
#include "Weapon.h"
#include "XComCloneGameState.h"


// Sets default values
AUnit::AUnit()
{
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SkeletalMeshAsset(TEXT("SkeletalMesh'/Game/Mannequin/Character/Mesh/SK_Mannequin.SK_Mannequin'"));
	static ConstructorHelpers::FObjectFinder<UAnimBlueprint> AnimClassAsset(TEXT("AnimBlueprint'/Game/Mannequin/Animations/ThirdPerson_AnimBP.ThirdPerson_AnimBP'"));
	static ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleSystemAsset(TEXT("ParticleSystem'/Game/StarterContent/Particles/P_Fire.P_Fire'"));

	GetCapsuleComponent()->InitCapsuleSize(35.f, 96.0f);
	GetCapsuleComponent()->bGenerateOverlapEvents = true;

	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetMesh()->SetRelativeLocationAndRotation(FVector(0.f, 0.f, -88.f), FRotator(0.f, -90.f, 0.f));

	if(SkeletalMeshAsset.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SkeletalMeshAsset.Object);
	}

	if(AnimClassAsset.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(AnimClassAsset.Object->GetAnimBlueprintGeneratedClass());
	}
	
	WeaponActorComponent = CreateDefaultSubobject<UChildActorComponent>(TEXT("WeaponActorComponent"));
	WeaponActorComponent->SetupAttachment(GetCapsuleComponent());
	WeaponActorComponent->SetChildActorClass(PrimaryWeapon);
	WeaponActorComponent->SetRelativeLocation(PrimaryWeaponOffset);

	OnFireComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("OnFireComponent"));
	OnFireComponent->SetupAttachment(GetCapsuleComponent());
	OnFireComponent->bAutoActivate = false;

	if(ParticleSystemAsset.Succeeded())
	{
		OnFireComponent->SetTemplate(ParticleSystemAsset.Object);
	}


	PrimaryWeaponOffset.Set(0.f, 20.f, 20.f);

	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	GetCharacterMovement()->MaxWalkSpeed = 350;
	GetCharacterMovement()->bOrientRotationToMovement = true;

	bIsMoving = false;
	bHasAttackedInCurrentTurn = false;
	bIsOnFire = false;

	MaxUnitTurnPoints = 4;
	HealthPoints = 100;

	UnitState = EUnitState::MOVING;
}


void AUnit::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	WeaponActorComponent->SetRelativeLocation(PrimaryWeaponOffset);

	if(WeaponActorComponent->GetChildActorClass() != PrimaryWeapon)
	{
		WeaponActorComponent->SetChildActorClass(PrimaryWeapon);
	}
}

void AUnit::Destroyed()
{
	Super::Destroyed();

	AXComCloneGameState * const gameState = GetWorld()->GetGameState<AXComCloneGameState>();

	if(gameState)
	{
		gameState->removePlayerUnit(PlayerId, MaxUnitTurnPoints + 1); // +1 is for an attack turn point
	}
}

// Called every frame
void AUnit::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	if(bIsMoving)
	{		
		FVector actorFloorLocation(GetActorLocation().X, GetActorLocation().Y, 0.f);
		FVector moveVector = mPathLocations[mCurrentPathLocationsIndex] - actorFloorLocation;
		moveVector.Normalize();
		
		AddMovementInput(moveVector, 1.f);

		actorFloorLocation.X = GetActorLocation().X;
		actorFloorLocation.Y = GetActorLocation().Y;

		if(FVector::PointsAreNear(actorFloorLocation, mPathLocations[mCurrentPathLocationsIndex], 5.f))
		{
			++mCurrentPathLocationsIndex;

			if(mCurrentPathLocationsIndex == mPathLocations.Num())
			{
				AXComCloneGameState * const gameState = GetWorld()->GetGameState<AXComCloneGameState>();
				if(gameState)
				{
					if(isOnFire())
					{
						gameState->performAction(MaxUnitTurnPoints + 1); //+1 if for attack point
					}
					else
					{
						gameState->performAction(mPathLocations.Num());
					}
				}

				mCurrentUnitTurnPoints -= isOnFire() ? (MaxUnitTurnPoints + 1) : mPathLocations.Num();
				bIsMoving = false;
				mUnitMovementEndEvent.Broadcast(this);
			}
		}
	}
}

void AUnit::BeginPlay()
{
	Super::BeginPlay();

	mCurrentUnitTurnPoints = MaxUnitTurnPoints;

	AXComCloneGameState * const gameState = GetWorld()->GetGameState<AXComCloneGameState>();

	if(gameState)
	{
		gameState->OnTurnChanged().AddUObject(this, &AUnit::OnTurnChange);
	}	
}

void AUnit::moveToLocation(const TArray<FVector>& path)
{
	mPathLocations.Empty();
	mPathLocations.Append(path);
	mCurrentPathLocationsIndex = 0;
	bIsMoving = true;

	mUnitMovementBeginEvent.Broadcast(this);
}

bool AUnit::isAlly(const AUnit& unit)const
{
	return PlayerId == unit.PlayerId;
}

void AUnit::attack(AUnit & otherUnit)
{	
	if(bHasAttackedInCurrentTurn) { return; }


	AWeapon *unitWeapon = Cast <AWeapon>(WeaponActorComponent->GetChildActor());
	float summarizedDamage = 0.f;

	if(!unitWeapon) { return; }

	for(size_t i = 0; i < unitWeapon->ShotsNumber; i++)
	{
		if(FMath::RandRange(0.f, 1.f) <= unitWeapon->Accuracy) // hit
		{
			summarizedDamage += unitWeapon->Damage;
		}
	}
	
	otherUnit.bIsOnFire = unitWeapon->isUsingFireDamage();
	otherUnit.GetOnFireComponent()->SetActive(otherUnit.bIsOnFire);

	bHasAttackedInCurrentTurn = true;

	AXComCloneGameState * const gameState = GetWorld()->GetGameState<AXComCloneGameState>();
	if(gameState)
	{
		gameState->performAction(1); //1 point for attack
	}

	unitWeapon->shootProjectiles(otherUnit.GetActorLocation());
	otherUnit.applyDamage(summarizedDamage);
}

int32 AUnit::getUnitRange() const
{
	int32 retVal = 0;
	if(UnitState == EUnitState::MOVING)
	{
		retVal = mCurrentUnitTurnPoints;
	}
	else if(UnitState == EUnitState::ATTACKING)
	{
		AWeapon *unitWeapon = Cast <AWeapon>(WeaponActorComponent->GetChildActor());

		if(unitWeapon)
		{
			retVal = unitWeapon->TileRange;
		}
	}

	return retVal;
}

void AUnit::applyDamage(float dmgValue)
{
	HealthPoints -= dmgValue;

	if(HealthPoints <= 0)
	{
		this->Destroy();
	}
}

void AUnit::setUnitState(EUnitState newUnitState)
{
	if(UnitState != newUnitState)
	{
		int32 previousRange = getUnitRange();
		UnitState = newUnitState;

		mUnitStateChangeEvent.Broadcast(this, previousRange);
	}
}

void AUnit::setAttacking(EUnitAttackingWeapon unitWeapon)
{
	mCurrentWeapon = unitWeapon;
	setUnitState(EUnitState::ATTACKING);
}

void AUnit::setMoving()
{
	setUnitState(EUnitState::MOVING);
}

bool AUnit::isShooting()const
{
	if(!WeaponActorComponent) { return false; }

	AWeapon *unitWeapon = Cast <AWeapon>(WeaponActorComponent->GetChildActor());

	if(unitWeapon)
	{
		return unitWeapon->isShooting();
	}

	return false;
}

bool AUnit::isOnFire() const
{
	return bIsOnFire;
}

const FText & AUnit::getPrimaryWeaponName() const
{
	if(!WeaponActorComponent) { return FText::GetEmpty(); }

	AWeapon *unitWeapon = Cast <AWeapon>(WeaponActorComponent->GetChildActor());

	if(unitWeapon)
	{
		return unitWeapon->getWeaponName();
	}

	return FText::GetEmpty();
}

AUnit::FOnUnitMovementBegin& AUnit::OnUnitMovementBegin()
{
	return mUnitMovementBeginEvent;
}

AUnit::FOnUnitMovementEnd& AUnit::OnUnitMovementEnd()
{
	return mUnitMovementEndEvent;
}

AUnit::FOnUnitStateChange & AUnit::OnUnitStateChange()
{
	return mUnitStateChangeEvent;
}

void AUnit::OnTurnChange(const EPlayerId nextPlayerTurn)
{
	mCurrentUnitTurnPoints = MaxUnitTurnPoints;

	bHasAttackedInCurrentTurn = false;

	UnitState = EUnitState::MOVING;

	if(PlayerId != nextPlayerTurn)
	{
		bIsOnFire = false; // stop beeing on fire after my turn (condition - only 2 players)
		OnFireComponent->SetActive(bIsOnFire);
	}
}
