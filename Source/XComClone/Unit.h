// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "XComCloneTypes.h"
#include "Unit.generated.h"

UCLASS()
class XCOMCLONE_API AUnit : public ACharacter
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite, EditAnyWhere, Category = "Gameplay")
	int32 MovementRangeInTiles;

	UPROPERTY(BlueprintReadWrite, EditAnyWhere, Category = "Gameplay")
	int32 HealthPoints;

	UPROPERTY(BlueprintReadWrite, EditAnyWhere, Category = "Gameplay")
	EPlayerId PlayerId;

	// Sets default values for this character's properties
	AUnit();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;
		
	void moveToLocation(const TArray<FVector>& path);

	bool isAlly(const AUnit& unit)const;


	DECLARE_EVENT_OneParam(AUnit, FOnUnitMovementBegin, const AUnit*)
	FOnUnitMovementBegin& OnUnitMovementBegin();

	DECLARE_EVENT_OneParam(AUnit, FOnUnitMovementEnd, const AUnit*)
	FOnUnitMovementEnd& OnUnitMovementEnd();

private:

	TArray<FVector> mPathLocations;
	int32 mCurrentIndex;

	bool bIsMoving;
	
	FOnUnitMovementBegin mUnitMovementBeginEvent;
	FOnUnitMovementEnd mUnitMovementEndEvent;
};
