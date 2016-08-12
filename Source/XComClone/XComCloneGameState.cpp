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
	else if(isPlayerTurn(EPlayerId::PLAYER_2))
	{
		CurrentPlayer = EPlayerId::PLAYER_1;
	}

	CurrentPlayerTurnPoints = MaxPlayerTurnPoints[static_cast<int8>(CurrentPlayer)];

	mTurnChangedEvent.Broadcast(CurrentPlayer);
}

bool AXComCloneGameState::isPlayerTurn(EPlayerId player)const
{
	return player == CurrentPlayer;
}

void AXComCloneGameState::setMaxPlayerTurnPoints(EPlayerId player, int32 value)
{
	MaxPlayerTurnPoints[static_cast<int8>(player)] = value;

	if(isPlayerTurn(player))
	{
		CurrentPlayerTurnPoints = MaxPlayerTurnPoints[static_cast<int8>(player)];
	}
}

int32 AXComCloneGameState::GetCurrentPlayerTurnPoints()const
{
	return CurrentPlayerTurnPoints;
}

int32 AXComCloneGameState::GetMaxPlayerTurnPoints(EPlayerId player) const
{
	return MaxPlayerTurnPoints[static_cast<int8>(player)];
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
	return PlayerUnitsCounter[static_cast<int8>(player)];
}

void AXComCloneGameState::removePlayerUnit(EPlayerId player, int32 unitTurnPoints)
{
	--PlayerUnitsCounter[static_cast<int8>(player)];
	MaxPlayerTurnPoints[static_cast<int8>(player)] -= unitTurnPoints;
}

AXComCloneGameState::FOnTurnChanged & AXComCloneGameState::OnTurnChanged()
{
	return mTurnChangedEvent;
}
