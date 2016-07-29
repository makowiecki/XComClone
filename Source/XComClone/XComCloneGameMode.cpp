// Fill out your copyright notice in the Description page of Project Settings.

#include "XComClone.h"
#include "XComCloneGameMode.h"
#include "XComClonePlayerController.h"
#include "XComCloneSpectatorPawn.h"
#include "XComCloneGameState.h"


AXComCloneGameMode::AXComCloneGameMode()
{
	PlayerControllerClass = AXComClonePlayerController::StaticClass();
	GameStateClass = AXComCloneGameState::StaticClass();
	//SpectatorClass = AXComCloneSpectatorPawn::StaticClass();
	//DefaultPawnClass = AXComCloneSpectatorPawn::StaticClass();

	MaxPlayerTurnPoints = 15;

}

void AXComCloneGameMode::InitGameState()
{
	Super::InitGameState();

	AXComCloneGameState* const gameState = GetGameState<AXComCloneGameState>();

	if(gameState)
	{
		gameState->setMaxPlayerTurnPoints(MaxPlayerTurnPoints);
		gameState->setInitPlayersUnits(4);
	}
}