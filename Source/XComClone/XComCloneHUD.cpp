// Fill out your copyright notice in the Description page of Project Settings.

#include "XComClone.h"
#include "XComCloneHUD.h"
#include "XComCloneGameState.h"
#include "XComCloneTypes.h"
#include "Unit.h"

#include "Engine.h"


AXComCloneHUD::AXComCloneHUD()
{
	static ConstructorHelpers::FObjectFinder<UFont> TextFontAsset(TEXT("Font'/Game/UI/Roboto-Regular.Roboto-Regular'"));

	if(TextFontAsset.Succeeded())
	{
		TextFont = TextFontAsset.Object;
	}
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
		drawButton(TEXT("EndTurn"), FString(TEXT("End Turn")));
		
		drawPlayerName(*gameState);
		drawPlayerUnits(*gameState);

		if(mActiveUnit)
		{
			drawButton(TEXT("PrimaryWeapon"), FString(TEXT("Primary Weapon")));
			drawButton(TEXT("MoveUnit"), FString(TEXT("Move Unit")));
			drawButton(TEXT("SecondaryWeapon"), FString(TEXT("Secondary Weapon")));
		}
	}
}

void AXComCloneHUD::NotifyHitBoxClick(FName BoxName)
{
	AXComCloneGameState * const gameState = GetWorld()->GetGameState<AXComCloneGameState>();
	
	if(BoxName == TEXT("EndTurn"))
	{
		if(gameState)
		{
			gameState->changeTurn();
		}
	}
	else if(BoxName == TEXT("PrimaryWeapon"))
	{
		mActiveUnit->setAttacking(EUnitAttackingWeapon::PRIMARY_WEAPON);
	}
	else if(BoxName == TEXT("MoveUnit"))
	{
		mActiveUnit->setMoving();
	}
	else if(BoxName == TEXT("SecondaryWeapon"))
	{
		mActiveUnit->setAttacking(EUnitAttackingWeapon::SECONDARY_WEAPON);
	}
}

void AXComCloneHUD::BeginPlay()
{
	Super::BeginPlay();
	
	FVector2D viewSize = FVector2D(100.f, 100.f);

	if(GEngine && GEngine->GameViewport)
	{
		GEngine->GameViewport->GetViewportSize(viewSize);
	}

	mButtons.Add(TEXT("EndTurn"), ButtonData(25.f, 75.f, 100.f, 50.f));
	mButtons.Add(TEXT("PrimaryWeapon"), ButtonData(25.f, viewSize.Y - 25.f - 50.f, 100.f, 50.f));
	mButtons.Add(TEXT("MoveUnit"), ButtonData(viewSize.X / 2 - 50.f, viewSize.Y - 25.f - 50.f, 100.f, 50.f));
	mButtons.Add(TEXT("SecondaryWeapon"), ButtonData(viewSize.X - 25.f - 100.f, viewSize.Y - 25.f - 50.f, 100.f, 50.f));
}

void AXComCloneHUD::drawTurnPoints(const AXComCloneGameState& gameState)
{
	const int32 currnetPlayerTurnPoints = gameState.GetCurrentPlayerTurnPoints();
	const int32 maxPlayerTurnPoints = gameState.GetMaxPlayerTurnPoints(gameState.getCurrentPlayer());

	FText text = FText::FromString(FString(TEXT("Turn points: ")) + FString::FromInt(currnetPlayerTurnPoints) + FString(TEXT("/")) + FString::FromInt(maxPlayerTurnPoints));

	FCanvasTextItem textItem(FVector2D(25.f, 25.f), text, TextFont, FLinearColor::Black);

	Canvas->DrawItem(textItem);
}

void AXComCloneHUD::drawButton(const FName& buttonName, const FString& buttonText)
{
	FVector2D buttonPosition(mButtons[buttonName].X, mButtons[buttonName].Y);
	FVector2D buttonSize(mButtons[buttonName].Width, mButtons[buttonName].Height);

	FCanvasTileItem button(buttonPosition, buttonSize, FLinearColor(0.5f, 0.75f, 1.f, 0.15f));

	Canvas->DrawItem(button);

	AddHitBox(FVector2D(mButtons[buttonName].X, mButtons[buttonName].Y), FVector2D(mButtons[buttonName].Width, mButtons[buttonName].Height), buttonName, true);

	FCanvasTextItem textItem(buttonPosition + buttonSize / 2, FText::FromString(buttonText), TextFont, FLinearColor::Black);
	textItem.bCentreX = true;
	textItem.bCentreY = true;
	textItem.Scale.Set(0.75f, 0.75f);

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

