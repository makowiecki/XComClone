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

	CurrentPlayerTurnPoints = MaxPlayerTurnPoints;

	mTurnChangedEvent.Broadcast(CurrentPlayer);
}

bool AXComCloneGameState::isPlayerTurn(EPlayerId player)const
{
	return player == CurrentPlayer;
}

void AXComCloneGameState::setMaxPlayerTurnPoints(int32 value)
{
	MaxPlayerTurnPoints = value;
	CurrentPlayerTurnPoints = MaxPlayerTurnPoints;
}

int32 AXComCloneGameState::GetCurrentPlayerTurnPoints()const
{
	return CurrentPlayerTurnPoints;
}

bool AXComCloneGameState::performAction(int32 points)
{
	if(points <= 0) { return false; }

	if(points > CurrentPlayerTurnPoints) { return false; }

	CurrentPlayerTurnPoints -= points;

	if(CurrentPlayerTurnPoints <= 0)
	{
		changeTurn();
	}

	return true;
}

void AXComCloneGameState::setInitPlayersUnits(int8 value)
{
	for(size_t i = 0; i < PlayerUnitsCounter.Num(); i++)
	{
		PlayerUnitsCounter[i] = value > 0 ? value : 0;
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

void AXComCloneGameState::removePlayerUnit(EPlayerId player)
{
	if(player == EPlayerId::PLAYER_1)
	{
		--PlayerUnitsCounter[0];
	}
	else if(player == EPlayerId::PLAYER_2)
	{
		--PlayerUnitsCounter[1];
	}
}

AXComCloneGameState::FOnTurnChanged & AXComCloneGameState::OnTurnChanged()
{
	return mTurnChangedEvent;
}

void AXComCloneGameState::Tick(float DeltaSeconds)
{
	//GEngine->AddOnScreenDebugMessage(1, 1.f, FColor::Green, FString("GameStateTick; Player turn: ") + FString(CurrentPlayer == EPlayerId::PLAYER_1 ? "PLAYER_1" : "PLAYER_2"));
}
