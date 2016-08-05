// Fill out your copyright notice in the Description page of Project Settings.

#include "XComClone.h"
#include "Weapon.h"


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

 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	FireDamage = false;
}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWeapon::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

const FText& AWeapon::getWeaponName() const
{
	return WeaponName;
}

bool AWeapon::isUsingFireDamage()
{
	return FireDamage;
}

