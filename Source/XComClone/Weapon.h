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

	// Sets default values for this actor's properties
	AWeapon();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	virtual const FString getWeaponName()const PURE_VIRTUAL(AWeapon::getWeaponName, return FString(TEXT("Weapon")););
	
	virtual bool isUsingFireDamage();

	//virtual class UTexture2D* getWeaponTexture();
};
