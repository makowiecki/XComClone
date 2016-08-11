// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

class AProjectile;

UCLASS(abstract)
class XCOMCLONE_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:	

	UPROPERTY(BlueprintReadWrite, EditAnyWhere, Category = "Weapon")
	float Damage;

	UPROPERTY(BlueprintReadWrite, EditAnyWhere, Category = "Weapon", meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float Accuracy;

	UPROPERTY(BlueprintReadWrite, EditAnyWhere, Category = "Weapon")
	int32 TileRange;

	UPROPERTY(BlueprintReadWrite, EditAnyWhere, Category = "Weapon")
	int32 ShotsNumber;

	UPROPERTY(BlueprintReadWrite, EditAnyWhere, Category = "Weapon")
	int32 BulletsInOneShot;

	UPROPERTY(BlueprintReadWrite, EditAnyWhere, Category = "Weapon")
	bool FireDamage;

	UPROPERTY(BlueprintReadWrite, EditAnyWhere, Category = "Weapon")
	FText WeaponName;

	UPROPERTY(BlueprintReadWrite, EditAnyWhere, Category = "Projectile")
	TSubclassOf<AProjectile> ProjectileType;

	UPROPERTY(BlueprintReadWrite, EditAnyWhere, Category = "Projectile")
	FVector ProjectileOffset;

	// Sets default values for this actor's properties
	AWeapon();

	virtual ~AWeapon() {};

	const FText& getWeaponName()const;
	
	bool isUsingFireDamage();

	void shootProjectiles(const FVector& destination);

	bool isShooting()const;


	FORCEINLINE class USceneComponent* GetBaseRoot() const { return BaseRoot; }

	FORCEINLINE class UStaticMeshComponent* GetWeaponMesh() const { return WeaponMesh; }

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Body, meta = (AllowPrivateAccess = "true"))
	class USceneComponent* BaseRoot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Body, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent *WeaponMesh;

	FTimerHandle mSpawningProjectilesTimerHandle;
	int32 mHowManyShots;
	FVector mDestinationForProjectiles;
	bool bIsShooting;

	void spawnProjectiles();

};
