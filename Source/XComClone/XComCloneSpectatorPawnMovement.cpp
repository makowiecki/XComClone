// Fill out your copyright notice in the Description page of Project Settings.

#include "XComClone.h"
#include "XComCloneSpectatorPawnMovement.h"
#include "XComClonePlayerController.h"
#include "XComCloneSpectatorPawn.h"


UXComCloneSpectatorPawnMovement::UXComCloneSpectatorPawnMovement(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer), bInitialLocationSet(false)
{
	MaxSpeed = 16000.f;
	Acceleration = 5000.f;
	Deceleration = 4000.f;
}


void UXComCloneSpectatorPawnMovement::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if(!PawnOwner || !UpdatedComponent)
	{
		return;
	}

	AXComClonePlayerController* PlayerController = Cast<AXComClonePlayerController>(PawnOwner->GetController());
	if(PlayerController && PlayerController->IsLocalController())
	{
		if(!bInitialLocationSet)
		{
			PawnOwner->SetActorRotation(PlayerController->GetControlRotation());
			PawnOwner->SetActorLocation(PlayerController->GetSpawnLocation());
			bInitialLocationSet = true;
		}

		FVector MyLocation = UpdatedComponent->GetComponentLocation();

		UpdatedComponent->SetWorldLocation(MyLocation, false);
	}
}


