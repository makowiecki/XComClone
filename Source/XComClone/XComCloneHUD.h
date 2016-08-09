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

	virtual void NotifyHitBoxClick(FName BoxName)override;

private:
	
	UFont* TextFont;

	AUnit *mActiveUnit; //weakptr ??

	void drawTurnPoints(const AXComCloneGameState& gameState);

	void drawEndTurnButton(const FText& buttonText);
	void drawPrimaryWeaponButton(const FText& buttonText);
	void drawMoveUnitButton(const FText& buttonText);
	void drawSecondaryWeaponButton(const FText& buttonText);

	void drawButton(const FVector2D& buttonPosition, const FVector2D& buttonSize, const FName& buttonName, const FText& buttonText);

	void drawPlayerName(const AXComCloneGameState& gameState);

	void drawPlayerUnits(const AXComCloneGameState& gameState);


};
