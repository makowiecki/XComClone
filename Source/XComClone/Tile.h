// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Tile.generated.h"

UCLASS()
class XCOMCLONE_API ATile : public AActor
{
	GENERATED_BODY()
			
public:	

	/* Cost of moving into this tile*/
	UPROPERTY(BlueprintReadWrite, EditAnyWhere, Category = "Tile")
	int32 Cost;

	/* X Coordinate in Map space */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Tile")
	int32 MapX;

	/* Y Coordinate in Map space */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Tile")
	int32 MapY;


	// Sets default values for this actor's properties
	ATile();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	static const FVector getSize();

	void setMapPosition(int32 pX, int32 pY);

	UFUNCTION()
	void OnMouseClicked(UPrimitiveComponent* clickedComponent);

	UFUNCTION()
	void OnBeginMouseOver(UPrimitiveComponent* pComponent);

	UFUNCTION()
	void OnEndMouseOver(UPrimitiveComponent* pComponent);

	bool isActive()const;

	void activate();

	void deactivate();

	const FVector getCenterInWorldLocation()const;

	DECLARE_EVENT_OneParam(ATile, FOnTileClicked, ATile*)
	FOnTileClicked& OnTileClicked();

	DECLARE_EVENT_OneParam(ATile, FOnBeginTileCursorOver, ATile*)
	FOnBeginTileCursorOver& OnBeginTileCursorOver();

	DECLARE_EVENT_OneParam(ATile, FOnEndTileCursorOver, ATile*)
	FOnEndTileCursorOver& OnEndTileCursorOver();
	

	FORCEINLINE class UStaticMeshComponent* GetMesh() const { return TileMesh; }
	FORCEINLINE class UDecalComponent* GetTileIndicator() const { return TileIndicator; }

private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Body, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent *TileMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Body, meta = (AllowPrivateAccess = "true"))
	class UDecalComponent *TileIndicator;

	bool bTileClicked;

	FOnTileClicked mTileClickedEvent;
	FOnBeginTileCursorOver mBeginTileCursorOverEvent;
	FOnEndTileCursorOver mEndTileCursorOverEvent;

};
