// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameMode.h"
#include "XComCloneGameMode.generated.h"

/**
 * 
 */
UCLASS()
class XCOMCLONE_API AXComCloneGameMode : public AGameMode
{
	GENERATED_BODY()
	
public:

	AXComCloneGameMode();

	virtual void BeginPlay() override;
	
private:

	int32 MaxPlayerTurnPoints;

};
