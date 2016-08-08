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

	virtual void BeginPlay()override;

private:

	struct ButtonData
	{
		float X;
		float Y;
		float Width;
		float Height;

		ButtonData(float pX, float pY, float pWidth, float pHeight)
			:X(pX),Y(pY), Width(pWidth), Height(pHeight)
		{}
	};

	UFont* TextFont;

	TMap<FName, ButtonData> mButtons;

	AUnit *mActiveUnit; //weakptr ??

	void drawTurnPoints(const AXComCloneGameState& gameState);

	void drawButton(const FName& buttonName, const FString& buttonText);

	void drawPlayerName(const AXComCloneGameState& gameState);

	void drawPlayerUnits(const AXComCloneGameState& gameState);


};
