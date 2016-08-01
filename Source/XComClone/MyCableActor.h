// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "CableComponent.h"
#include "MyCableActor.generated.h"

UCLASS()
class XCOMCLONE_API AMyCableActor: public AActor
{
	GENERATED_BODY()

public:

	AMyCableActor();

	/** Cable component that performs simulation and rendering */
	UPROPERTY(Category = Cable, VisibleAnywhere, BlueprintReadOnly)
	class UCableComponent* CableComponent;

};
