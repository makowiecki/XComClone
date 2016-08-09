// Fill out your copyright notice in the Description page of Project Settings.

#include "XComClone.h"
#include "Weapon.h"
#include "Projectile.h"


// Sets default values
AWeapon::AWeapon()
{
	static ConstructorHelpers::FObjectFinder<UStaticMesh> WeaponMeshAsset(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_NarrowCapsule.Shape_NarrowCapsule'"));

	BaseRoot = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	BaseRoot->SetupAttachment(RootComponent);
	//BaseRoot->SetWorldLocation(FVector(0.f, 0.f, 0.f));
	//RootComponent = BaseRoot;

	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
	WeaponMesh->SetupAttachment(BaseRoot);
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	SetActorEnableCollision(false);

	if(WeaponMeshAsset.Succeeded())
	{
		WeaponMesh->SetStaticMesh(WeaponMeshAsset.Object);
	}

	FireDamage = false;

	ProjectileType = AProjectile::StaticClass();

	mHowManyShots = 0;
	bIsShooting = false;
}

const FText& AWeapon::getWeaponName() const
{
	return WeaponName;
}

bool AWeapon::isUsingFireDamage()
{
	return FireDamage;
}

void AWeapon::shootProjectiles(const FVector& destination)
{
	if(!ProjectileType) { return; }

	mHowManyShots = 0;
	mDestinationForProjectiles = destination;
	bIsShooting = true;
	GetWorldTimerManager().SetTimer(mSpawningProjectilesTimerHandle, this, &AWeapon::spawnProjectiles, 0.25f, true, 0.f);
}

bool AWeapon::isShooting()const
{
	return bIsShooting;
}

void AWeapon::spawnProjectiles()
{
	if(mHowManyShots == ShotsNumber)
	{
		bIsShooting = false;
		GetWorldTimerManager().ClearTimer(mSpawningProjectilesTimerHandle);
		return;
	}
	else
	{
		++mHowManyShots;
	}

	for(size_t j = 0; j < BulletsInOneShot; j++)
	{
		FVector newDestination = mDestinationForProjectiles;
		const float randomVal = 25.f;
		newDestination.Y += FMath::RandRange(-randomVal, randomVal);
		newDestination.Z += FMath::RandRange(-randomVal, randomVal);

		const FVector Direction = newDestination - GetActorLocation();

		const FRotator SpawnRotation = FRotationMatrix::MakeFromX(Direction).Rotator();
		FVector SpawnLocation = GetActorLocation() + SpawnRotation.RotateVector(ProjectileOffset);

		UWorld* const World = GetWorld();
		if(World)
		{
			World->SpawnActor<AProjectile>(ProjectileType, SpawnLocation, SpawnRotation);
		}
	}
}
