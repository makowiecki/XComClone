// Fill out your copyright notice in the Description page of Project Settings.

#include "XComClone.h"
#include "XComCloneGameMode.h"
#include "XComClonePlayerController.h"
#include "XComCloneSpectatorPawn.h"


AXComCloneGameMode::AXComCloneGameMode()
{
	PlayerControllerClass = AXComClonePlayerController::StaticClass();
	SpectatorClass = AXComCloneSpectatorPawn::StaticClass();
	DefaultPawnClass = AXComCloneSpectatorPawn::StaticClass();


}

