// Fill out your copyright notice in the Description page of Project Settings.

#include "XComClone.h"
#include "XComCloneSpectatorPawn.h"
#include "XComCloneSpectatorPawnMovement.h"


AXComCloneSpectatorPawn::AXComCloneSpectatorPawn(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UXComCloneSpectatorPawnMovement>(Super::MovementComponentName))
{
	GetCollisionComponent()->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
	bAddDefaultMovementBindings = false;
	CameraComponent = CreateDefaultSubobject<UXComCloneCameraComponent>(TEXT("CameraComponent"));
}


void AXComCloneSpectatorPawn::MoveForward(float Value)
{
	if(Value != 0.0f)
	{
		CameraComponent->MoveForward(Value);

	}
}

void AXComCloneSpectatorPawn::MoveRight(float Value)
{
	if(Value != 0.0f)
	{
		CameraComponent->MoveRight(Value);
	}
}

void AXComCloneSpectatorPawn::OnMouseScrollUp()
{
	CameraComponent->OnZoomIn();
}

void AXComCloneSpectatorPawn::OnMouseScrollDown()
{
	CameraComponent->OnZoomOut();
}

void AXComCloneSpectatorPawn::SetupPlayerInputComponent(UInputComponent* InputComponent)
{
	check(InputComponent);

	InputComponent->BindAction("ZoomOut", IE_Pressed, this, &AXComCloneSpectatorPawn::OnMouseScrollUp);
	InputComponent->BindAction("ZoomIn", IE_Pressed, this, &AXComCloneSpectatorPawn::OnMouseScrollDown);

	InputComponent->BindAxis("MoveForward", this, &AXComCloneSpectatorPawn::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &AXComCloneSpectatorPawn::MoveRight);
}