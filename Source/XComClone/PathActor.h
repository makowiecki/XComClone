// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "PathActor.generated.h"

UCLASS()
class XCOMCLONE_API APathActor: public AActor
{
	GENERATED_BODY()

public:

	APathActor();

	void SetVisibility(bool newVisibility);

	FORCEINLINE class USceneComponent* GetBaseRoot() const { return BaseRoot; }

	FORCEINLINE class UStaticMeshComponent* GetMesh() const { return mMesh; }

private:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components, meta = (AllowPrivateAccess = "true"))
	class USceneComponent* BaseRoot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* mMesh;

};
