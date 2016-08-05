// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/HUD.h"
#include "XComCloneHUD.generated.h"

class AXComCloneGameState;
class AUnit;

/**
 * 
 */
UCLASS()
class XCOMCLONE_API AXComCloneHUD : public AHUD
{
	GENERATED_BODY()
	
public:

	AXComCloneHUD();

	void setActiveUnit(AUnit *unit);
	void deactivateUnit();
	
	virtual void DrawHUD()override;

private:

	UFont* TextFont;

	AUnit *mActiveUnit; //weakptr ??

	void drawTurnPoints(const AXComCloneGameState& gameState);

	void drawPlayerName(const AXComCloneGameState& gameState);

	void drawPlayerUnits(const AXComCloneGameState& gameState);


};
