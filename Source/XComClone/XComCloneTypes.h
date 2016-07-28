#pragma once

#include "XComCloneTypes.generated.h"

UENUM()
enum class ETileMode: uint8
{
	EMPTY,
	BLOCKED,
	ENEMY,
	ALLY
};

UENUM()
enum class EPlayerId: uint8
{
	PLAYER_1,
	PLAYER_2
};