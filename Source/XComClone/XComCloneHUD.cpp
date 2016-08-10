// Fill out your copyright notice in the Description page of Project Settings.

#include "XComClone.h"
#include "XComCloneHUD.h"
#include "XComCloneGameState.h"
#include "XComCloneTypes.h"
#include "Unit.h"

#include "Engine.h"

#define LOCTEXT_NAMESPACE "HUDNamespace" 

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
		drawEndTurnButton(LOCTEXT("EndTurn", "End Turn"));
		
		drawPlayerName(*gameState);
		drawPlayerUnits(*gameState);

		drawUnitsHealthPoints();

		if(mActiveUnit)
		{
			drawPrimaryWeaponButton(mActiveUnit->getPrimaryWeaponName());
			drawMoveUnitButton(LOCTEXT("MoveUnit", "Move Unit"));
			drawSecondaryWeaponButton(LOCTEXT("SecondaryWeapon", "Secondary Weapon"));
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

void AXComCloneHUD::drawTurnPoints(const AXComCloneGameState& gameState)
{
	const int32 currnetPlayerTurnPoints = gameState.GetCurrentPlayerTurnPoints();
	const int32 maxPlayerTurnPoints = gameState.GetMaxPlayerTurnPoints(gameState.getCurrentPlayer());

	FText text = FText::FromString(FString(TEXT("Turn points: ")) + FString::FromInt(currnetPlayerTurnPoints) + FString(TEXT("/")) + FString::FromInt(maxPlayerTurnPoints));

	FCanvasTextItem textItem(FVector2D(25.f, 25.f), text, TextFont, FLinearColor::Black);

	Canvas->DrawItem(textItem);
}

void AXComCloneHUD::drawEndTurnButton(const FText& buttonText)
{
	const FVector2D buttonPosition(Canvas->OrgX + 25.f, Canvas->OrgY + 75.f);
	const FVector2D buttonSize(100.f, 50.f);

	drawButton(buttonPosition, buttonSize, TEXT("EndTurn"), buttonText);
}

void AXComCloneHUD::drawPrimaryWeaponButton(const FText& buttonText)
{
	const FVector2D buttonPosition(Canvas->OrgX + 25.f, Canvas->ClipY - 50.f - 25.f);
	const FVector2D buttonSize(125.f, 50.f);

	drawButton(buttonPosition, buttonSize, TEXT("PrimaryWeapon"), buttonText);

}

void AXComCloneHUD::drawMoveUnitButton(const FText& buttonText)
{
	const FVector2D buttonPosition(Canvas->OrgX + Canvas->SizeX / 2 - 50.f, Canvas->ClipY - 50.f - 25.f);
	const FVector2D buttonSize(100.f, 50.f);

	drawButton(buttonPosition, buttonSize, TEXT("MoveUnit"), buttonText);
}

void AXComCloneHUD::drawSecondaryWeaponButton(const FText& buttonText)
{
	const FVector2D buttonPosition(Canvas->ClipX - 125.f - 25.f, Canvas->ClipY - 50.f - 25.f);
	const FVector2D buttonSize(125.f, 50.f);

	drawButton(buttonPosition, buttonSize, TEXT("SecondaryWeapon"), buttonText);
}

void AXComCloneHUD::drawButton(const FVector2D& buttonPosition, const FVector2D& buttonSize, const FName& buttonName, const FText& buttonText)
{
	FCanvasTileItem button(buttonPosition, buttonSize, FLinearColor(0.5f, 0.75f, 1.f, 0.15f));

	Canvas->DrawItem(button);

	AddHitBox(buttonPosition, buttonSize, buttonName, true);

	FCanvasTextItem textItem(buttonPosition + buttonSize / 2, buttonText, TextFont, FLinearColor::Black);
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

void AXComCloneHUD::drawUnitsHealthPoints()
{
	for(FConstPawnIterator it = GetWorld()->GetPawnIterator(); it; ++it)
	{
		AUnit *worldUnit = Cast<AUnit>(*it);
		if(worldUnit)
		{
			const int32 unitHp = worldUnit->HealthPoints;
			const float zOffset = worldUnit->GetCapsuleComponent()->GetScaledCapsuleHalfHeight() + 30.f;
			const FString text = FString(TEXT("HP: ")) + FString::FromInt(unitHp);
			float textWidth = 0, textHeight = 0;
			GetTextSize(text, textWidth, textHeight, TextFont, 0.45f);

			FVector textLocation = worldUnit->GetActorLocation();
			textLocation.Z += zOffset;
			FVector2D textPosition = FVector2D(Canvas->Project(textLocation));
			FVector2D backgroundPosition = textPosition;
			backgroundPosition.X -= textWidth / 2 + 5.f;

			FCanvasTileItem background(backgroundPosition, FVector2D(textWidth + 10.f, textHeight), FLinearColor::White);

			FCanvasTextItem textItem(textPosition, FText::FromString(text), TextFont, FLinearColor::Black);
			textItem.bCentreX = true;
			textItem.Scale.Set(0.45f, 0.45f);

			Canvas->DrawItem(background);
			Canvas->DrawItem(textItem);
		}
	}
}


#undef LOCTEXT_NAMESPACE 