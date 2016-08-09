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

	for(size_t i = 0; i < ShotsNumber; i++)
	{
		for(size_t j = 0; j < BulletsInOneShot; j++)
		{
			const FVector Direction = destination - GetActorLocation();

			const FRotator SpawnRotation = FRotationMatrix::MakeFromX(Direction).Rotator();
			const FVector SpawnLocation = GetActorLocation() + SpawnRotation.RotateVector(ProjectileOffset);

			UWorld* const World = GetWorld();
			if(World != NULL)
			{
				World->SpawnActor<AProjectile>(ProjectileType, SpawnLocation, SpawnRotation);
			}
		}
	}
}

