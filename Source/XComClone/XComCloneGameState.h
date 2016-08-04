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

	bool isPlayerTurn(EPlayerId plyer)const;

	void setMaxPlayerTurnPoints(int32 value);
	int32 GetCurrentPlayerTurnPoints()const;

	//removing turn points

	void setInitPlayersUnits(int8 value);
	int8 getPlayerUnits(EPlayerId pleyer)const;
	
	DECLARE_EVENT_OneParam(AXComCloneGameState, FOnTurnChanged, const EPlayerId)
	FOnTurnChanged& OnTurnChanged();

protected:

	virtual void Tick(float DeltaSeconds)override;
	
private:

	EPlayerId CurrentPlayer;

	int32 CurrentPlayerTurnPoints;
	int32 MaxPlayerTurnPoints;

	//int8 PlayerUnitsCounter[2];
	TStaticArray<int8, 2> PlayerUnitsCounter;

	FOnTurnChanged mTurnChangedEvent;
};
