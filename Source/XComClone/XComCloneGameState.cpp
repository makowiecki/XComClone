// Fill out your copyright notice in the Description page of Project Settings.

#include "XComClone.h"
#include "XComCloneGameState.h"

#include "Engine.h"

AXComCloneGameState::AXComCloneGameState()
{
	CurrentPlayer = EPlayerId::PLAYER_1;


	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
}

EPlayerId AXComCloneGameState::getCurrentPlayer()const
{
	return CurrentPlayer;
}

void AXComCloneGameState::changeTurn()
{
	if(isPlayerTurn(EPlayerId::PLAYER_1))
	{
		CurrentPlayer = EPlayerId::PLAYER_2;
	}
	else //PLAYER_2 turn
	{
		CurrentPlayer = EPlayerId::PLAYER_1;
	}

	mTurnChangedEvent.Broadcast(CurrentPlayer);
}

bool AXComCloneGameState::isPlayerTurn(EPlayerId player)const
{
	return player == CurrentPlayer;
}

void AXComCloneGameState::setMaxPlayerTurnPoints(int32 value)
{
	MaxPlayerTurnPoints = value;
}

int32 AXComCloneGameState::GetCurrentPlayerTurnPoints()const
{
	return CurrentPlayerTurnPoints;
}

void AXComCloneGameState::setInitPlayersUnits(int8 value)
{
	if(value > 0)
	{
		PlayerUnitsCounter[0] = value;
		PlayerUnitsCounter[1] = value;
	}
	else
	{
		PlayerUnitsCounter[0] = 0;
		PlayerUnitsCounter[1] = 0;
	}
}

int8 AXComCloneGameState::getPlayerUnits(EPlayerId player)const
{
	if(player == EPlayerId::PLAYER_1)
	{
		return PlayerUnitsCounter[0];
	}
	else if(player == EPlayerId::PLAYER_2)
	{
		return PlayerUnitsCounter[1];
	}

	return 0;
}

AXComCloneGameState::FOnTurnChanged & AXComCloneGameState::OnTurnChanged()
{
	return mTurnChangedEvent;
}

void AXComCloneGameState::Tick(float DeltaSeconds)
{
	GEngine->AddOnScreenDebugMessage(1, 1.f, FColor::Green, FString("GameStateTick; Player turn: ") + FString(CurrentPlayer == EPlayerId::PLAYER_1 ? "PLAYER_1" : "PLAYER_2"));
}
