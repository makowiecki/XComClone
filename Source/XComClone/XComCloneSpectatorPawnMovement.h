// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/SpectatorPawnMovement.h"
#include "XComCloneSpectatorPawnMovement.generated.h"

/**
 * 
 */
UCLASS()
class XCOMCLONE_API UXComCloneSpectatorPawnMovement : public USpectatorPawnMovement
{
	GENERATED_BODY()

public:

	UXComCloneSpectatorPawnMovement(const FObjectInitializer& ObjectInitializer);

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;

private:
	bool bInitialLocationSet;	
	
};
