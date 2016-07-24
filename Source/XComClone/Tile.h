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

	const FVector getSize()const;

	//UFUNCTION()
	//void doOnMouseOver(UPrimitiveComponent* TouchedComponent);


	FORCEINLINE class UStaticMeshComponent* GetMesh() const { return TileMesh; }

private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Body, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent *TileMesh;

	UFUNCTION()
	void Clicked();

	UFUNCTION()
	void CursorOver();

	UFUNCTION() 
	void EndCursorOver();
};
