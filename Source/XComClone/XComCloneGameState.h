// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameState.h"
#include "XComCloneTypes.h"
#include "XComCloneGameState.generated.h"

/**
 * 
 */
UCLASS()
class XCOMCLONE_API AXComCloneGameState : public AGameState
{
	GENERATED_BODY()
	
public:

	AXComCloneGameState();

	EPlayerId getCurrentPlayer()const;

	void changeTurn();

	bool isPlayerTurn(EPlayerId player)const;

	void setMaxPlayerTurnPoints(EPlayerId player, int32 value);
	int32 GetCurrentPlayerTurnPoints()const;
	int32 GetMaxPlayerTurnPoints(EPlayerId player)const;

	bool performAction(int32 points);

	void setInitPlayersUnits(int8 value);
	int8 getPlayerUnits(EPlayerId pleyer)const;

	void removePlayerUnit(EPlayerId player, int32 unitTurnPoints);
	
	DECLARE_EVENT_OneParam(AXComCloneGameState, FOnTurnChanged, const EPlayerId)
	FOnTurnChanged& OnTurnChanged();
		
private:

	EPlayerId CurrentPlayer;

	int32 CurrentPlayerTurnPoints;

	TStaticArray<int32, 2> MaxPlayerTurnPoints;

	TStaticArray<int8, 2> PlayerUnitsCounter;

	FOnTurnChanged mTurnChangedEvent;
};
