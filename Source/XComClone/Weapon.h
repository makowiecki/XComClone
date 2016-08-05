// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

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

	// Sets default values for this actor's properties
	AWeapon();

	virtual ~AWeapon() {};

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	virtual const FText& getWeaponName()const;
	
	virtual bool isUsingFireDamage();

	//virtual class UTexture2D* getWeaponTexture();


	FORCEINLINE class USceneComponent* GetBaseRoot() const { return BaseRoot; }

	FORCEINLINE class UStaticMeshComponent* GetWeaponMesh() const { return WeaponMesh; }

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Body, meta = (AllowPrivateAccess = "true"))
	class USceneComponent* BaseRoot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Body, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent *WeaponMesh;

};
