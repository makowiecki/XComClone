// Fill out your copyright notice in the Description page of Project Settings.

#include "XComClone.h"
#include "XComCloneGameMode.h"
#include "XComClonePlayerController.h"
#include "XComCloneSpectatorPawn.h"
#include "XComCloneGameState.h"
#include "XComCloneHUD.h"
#include "Unit.h"


AXComCloneGameMode::AXComCloneGameMode()
{
	PlayerControllerClass = AXComClonePlayerController::StaticClass();
	GameStateClass = AXComCloneGameState::StaticClass();
	HUDClass = AXComCloneHUD::StaticClass();
	//SpectatorClass = AXComCloneSpectatorPawn::StaticClass();
	//DefaultPawnClass = AXComCloneSpectatorPawn::StaticClass();

	MaxPlayerTurnPoints = 15;

}

void AXComCloneGameMode::BeginPlay()
{
	Super::BeginPlay();

	AXComCloneGameState* const gameState = GetGameState<AXComCloneGameState>();

	if(gameState)
	{
		int32 playerTurnPoints[2]{ 0,0 };
		for(FConstPawnIterator it = GetWorld()->GetPawnIterator(); it; ++it)
		{
			AUnit *worldUnit = Cast<AUnit>(*it);
			if(worldUnit)
			{
				playerTurnPoints[static_cast<int8>(worldUnit->PlayerId)] += (worldUnit->MaxUnitTurnPoints + 1);
			}
		}

		gameState->setMaxPlayerTurnPoints(EPlayerId::PLAYER_1, playerTurnPoints[0]);
		gameState->setMaxPlayerTurnPoints(EPlayerId::PLAYER_2, playerTurnPoints[1]);
		
		gameState->setInitPlayersUnits(4);
	}
}