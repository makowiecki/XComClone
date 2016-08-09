// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "XComCloneTypes.h"
#include "Unit.generated.h"

class AWeapon;

UCLASS()
class XCOMCLONE_API AUnit : public ACharacter
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite, EditAnyWhere, Category = "Gameplay")
	int32 MaxUnitTurnPoints;
	//int32 MovementRangeInTiles;

	UPROPERTY(BlueprintReadWrite, EditAnyWhere, Category = "Gameplay")
	int32 HealthPoints;

	UPROPERTY(BlueprintReadWrite, EditAnyWhere, Category = "Gameplay")
	EPlayerId PlayerId;

	UPROPERTY(BlueprintReadWrite, EditAnyWhere, Category = "Gameplay")
	EUnitState UnitState;

	UPROPERTY(BlueprintReadWrite, EditAnyWhere, Category = "Weapon")
	TSubclassOf<AWeapon> PrimaryWeapon;

	UPROPERTY(BlueprintReadWrite, EditAnyWhere, Category = "Weapon")
	FVector PrimaryWeaponOffset;

	// Sets default values for this character's properties
	AUnit();

	virtual void OnConstruction(const FTransform& Transform) override;

	virtual void Destroyed() override;

	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	virtual void BeginPlay() override;
		
	void moveToLocation(const TArray<FVector>& path);

	bool isAlly(const AUnit& unit)const;

	void attack(AUnit& otherUnit);

	int32 getUnitRange()const;

	void applyDamage(float dmgValue);

	void setUnitState(EUnitState newUnitState);

	void setAttacking(EUnitAttackingWeapon unitWeapon);

	void setMoving();

	bool isShooting()const;

	const FText& getPrimaryWeaponName()const;

	DECLARE_EVENT_OneParam(AUnit, FOnUnitMovementBegin, const AUnit*)
	FOnUnitMovementBegin& OnUnitMovementBegin();

	DECLARE_EVENT_OneParam(AUnit, FOnUnitMovementEnd, const AUnit*)
	FOnUnitMovementEnd& OnUnitMovementEnd();

	DECLARE_EVENT_TwoParams(AUnit, FOnUnitStateChange, const AUnit*, int32)
	FOnUnitStateChange& OnUnitStateChange();

	FORCEINLINE class UChildActorComponent* GetWeaponActorComponent() const { return WeaponActorComponent; }

private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Body, meta = (AllowPrivateAccess = "true"))
	class UChildActorComponent *WeaponActorComponent;

	TArray<FVector> mPathLocations;
	int32 mCurrentPathLocationsIndex;

	EUnitAttackingWeapon mCurrentWeapon;

	int32 mCurrentUnitTurnPoints;

	bool bHasAttackedInCurrentTurn;

	bool bIsMoving;
	
	FOnUnitMovementBegin mUnitMovementBeginEvent;
	FOnUnitMovementEnd mUnitMovementEndEvent;
	FOnUnitStateChange mUnitStateChangeEvent;

	UFUNCTION()
	void OnTurnChange(const EPlayerId nextPlayerTurn);
};
