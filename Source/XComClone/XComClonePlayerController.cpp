// Fill out your copyright notice in the Description page of Project Settings.

#include "XComClone.h"
#include "XComClonePlayerController.h"


AXComClonePlayerController::AXComClonePlayerController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;
	/* Enable mouse events */
	bEnableClickEvents = true;
	bEnableMouseOverEvents = true;
}

