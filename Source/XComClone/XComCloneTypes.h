#pragma once

#include "XComCloneTypes.generated.h"

UENUM()
enum class ETileMode: uint8
{
	EMPTY UMETA(DisplayName = "EMPTY"),
	BLOCKED UMETA(DisplayName = "BLOCKED"),
	ENEMY UMETA(DisplayName = "ENEMY"),
	ALLY UMETA(DisplayName = "ALLY"),
};

UENUM()
enum class EUnitState: uint8
{
	MOVING UMETA(DisplayName = "MOVING"),
	ATTACKING UMETA(DisplayName = "ATTACKING")
};

UENUM()
enum class EPlayerId: uint8
{
	PLAYER_1 UMETA(DisplayName = "PLAYER 1"),
	PLAYER_2 UMETA(DisplayName = "PLAYER 2"),
};