// Fill out your copyright notice in the Description page of Project Settings.

#include "XComClone.h"
#include "Unit.h"
#include "Weapon.h"


// Sets default values
AUnit::AUnit()
{
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SkeletalMeshAsset(TEXT("SkeletalMesh'/Game/Mannequin/Character/Mesh/SK_Mannequin.SK_Mannequin'"));
	static ConstructorHelpers::FObjectFinder<UAnimBlueprint> AnimClassAsset(TEXT("AnimBlueprint'/Game/Mannequin/Animations/ThirdPerson_AnimBP.ThirdPerson_AnimBP'"));

	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
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

	PrimaryWeaponOffset.Set(0.f, 20.f, 20.f);

	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	GetCharacterMovement()->MaxWalkSpeed = 350;
	GetCharacterMovement()->bOrientRotationToMovement = true;

	bIsMoving = false;

	MovementRangeInTiles = 4;
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

// Called every frame
void AUnit::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	if(bIsMoving)
	{		
		FVector actorFloorLocation(GetActorLocation().X, GetActorLocation().Y, 0.f);
		FVector moveVector = mPathLocations[mCurrentIndex] - actorFloorLocation;
		moveVector.Normalize();
		
		AddMovementInput(moveVector, 1.f);

		actorFloorLocation.X = GetActorLocation().X;
		actorFloorLocation.Y = GetActorLocation().Y;

		if(FVector::PointsAreNear(actorFloorLocation, mPathLocations[mCurrentIndex], 5.f))
		{
			++mCurrentIndex;

			if(mCurrentIndex == mPathLocations.Num())
			{
				bIsMoving = false;
				mUnitMovementEndEvent.Broadcast(this);
			}
		}
	}
}

// Called to bind functionality to input
void AUnit::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

}

void AUnit::moveToLocation(const TArray<FVector>& path)
{
	mPathLocations.Empty();
	mPathLocations.Append(path);
	mCurrentIndex = 0;
	bIsMoving = true;

	mUnitMovementBeginEvent.Broadcast(this);
}

bool AUnit::isAlly(const AUnit& unit)const
{
	return PlayerId == unit.PlayerId;
}

void AUnit::attack(AUnit & otherUnit)
{
	//raycast check ??
	//applydamage

	AWeapon *unitWeapon = Cast <AWeapon>(WeaponActorComponent->GetChildActor());

	if(unitWeapon)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Emerald, FString::SanitizeFloat(unitWeapon->Damage));
	}
}

int32 AUnit::getUnitRange() const
{
	int32 retVal = 0;
	if(UnitState == EUnitState::MOVING)
	{
		retVal = MovementRangeInTiles;
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

AUnit::FOnUnitMovementBegin& AUnit::OnUnitMovementBegin()
{
	return mUnitMovementBeginEvent;
}

AUnit::FOnUnitMovementEnd& AUnit::OnUnitMovementEnd()
{
	return mUnitMovementEndEvent;
}
