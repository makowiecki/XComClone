// Fill out your copyright notice in the Description page of Project Settings.

#include "XComClone.h"
#include "XComCloneHUD.h"
#include "XComCloneGameState.h"
#include "XComCloneTypes.h"
#include "Unit.h"


AXComCloneHUD::AXComCloneHUD()
{
	static ConstructorHelpers::FObjectFinder<UFont> TextFontAsset(TEXT("Font'/Game/UI/Roboto-Regular.Roboto-Regular'"));

	if(TextFontAsset.Succeeded())
	{
		TextFont = TextFontAsset.Object;
	}


	//bShowHitBoxDebugInfo = true;

	//AddHitBox(FVector2D(50, 50), FVector2D(100, 50), TEXT("PrimaryWeapon"), true);
}

void AXComCloneHUD::setActiveUnit(AUnit *unit)
{
	mActiveUnit = unit;
}

void AXComCloneHUD::deactivateUnit()
{
	mActiveUnit = nullptr;
}

void AXComCloneHUD::DrawHUD()
{
	Super::DrawHUD();
	
	AXComCloneGameState * const gameState = GetWorld()->GetGameState<AXComCloneGameState>();
	if(gameState)
	{
		drawTurnPoints(*gameState);
		drawPlayerName(*gameState);
		drawPlayerUnits(*gameState);
	}
}

void AXComCloneHUD::drawTurnPoints(const AXComCloneGameState& gameState)
{
	const int32 currnetPlayerTurnPoints = gameState.GetCurrentPlayerTurnPoints();
	const int32 maxPlayerTurnPoints = gameState.GetMaxPlayerTurnPoints(gameState.getCurrentPlayer());

	FText text = FText::FromString(FString(TEXT("Turn points: ")) + FString::FromInt(currnetPlayerTurnPoints) + FString(TEXT("/")) + FString::FromInt(maxPlayerTurnPoints));

	FCanvasTextItem textItem(FVector2D(25.f, 25.f), text, TextFont, FLinearColor::Black);

	Canvas->DrawItem(textItem);
}

void AXComCloneHUD::drawPlayerName(const AXComCloneGameState& gameState)
{
	const EPlayerId currentPlayer = gameState.getCurrentPlayer();
	FString playerName;

	switch(currentPlayer)
	{
		case EPlayerId::PLAYER_1:
			playerName = "PLAYER 1";
			break;
		case EPlayerId::PLAYER_2:
			playerName = "PLAYER 2";
			break;
		default:
			break;
	}

	FText text = FText::FromString(playerName);

	FCanvasTextItem textItem(FVector2D(Canvas->SizeX / 2, 25.f), text, TextFont, FLinearColor::Black);
	textItem.bCentreX = true;

	Canvas->DrawItem(textItem);
}

void AXComCloneHUD::drawPlayerUnits(const AXComCloneGameState& gameState)
{
	const int8 currentPlayerUnits = gameState.getPlayerUnits(gameState.getCurrentPlayer());

	FText text = FText::FromString(FString(TEXT("Player units: ")) + FString::FromInt(currentPlayerUnits));

	float textWidth = 0, textHeight = 0;
	GetTextSize(text.ToString(), textWidth, textHeight, TextFont);

	FCanvasTextItem textItem(FVector2D(Canvas->ClipX - textWidth - 25.f, 25.f), text, TextFont, FLinearColor::Black);	

	Canvas->DrawItem(textItem);
}

