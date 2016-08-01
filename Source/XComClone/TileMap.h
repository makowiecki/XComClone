// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Array.h"
#include "Tile.h"
#include "MyCableActor.h"
#include "TileMap.generated.h"

class AUnit;


UCLASS()
class XCOMCLONE_API ATileMap : public AActor
{
	GENERATED_BODY()
		
public:	

	UPROPERTY(EditAnywhere, Category = Map)
	int32 RowCount;

	UPROPERTY(EditAnywhere, Category = Map)
	int32 ColumnCount;


	// Sets default values for this actor's properties
	ATileMap();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called when the game ends or when destroyed
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;


	FORCEINLINE class USceneComponent* GetBaseRoot() const { return BaseRoot; }


protected:

	virtual void OnConstruction(const FTransform& Transform)override;

	virtual void Destroyed() override;

	virtual void PostInitializeComponents()override;
	
private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Body, meta = (AllowPrivateAccess = "true"))
	class USceneComponent* BaseRoot;

	UPROPERTY()
	TArray<ATile*>	mTilesArray;

	UPROPERTY()
	TArray<AMyCableActor*> mPathArray;

	TArray<FVector> mPathSteps;

	bool bFoundPath;

	int32 mPreviousRowCount;
	int32 mPreviousColumnCount;

	ATile* mSelectedTile;

	bool bIsUnitMoving;

	void AdjustNumberOfTiles();

	UFUNCTION()
	void OnTileClicked(ATile* tile);

	UFUNCTION()
	void OnBeginTileCursorOver(ATile* tile);

	UFUNCTION()
	void OnEndTileCursorOver(ATile* tile);

	UFUNCTION()
	void OnUnitMovementBegin(const AUnit* unit);

	UFUNCTION()
	void OnUnitMovementEnd(const AUnit* unit);

	void selectTile(ATile* tile);
	void deselectTile();

	void getTileNeighbours(const ATile& tile, TArray<ATile*>& outArray);

	void findTilesInRange(ATile& startTile, TArray<ATile*>& outArray, int32 range);

	bool findPath(ATile& destination, TArray<FVector>& outArray);

	void drawPath();

	void removePath();
};
