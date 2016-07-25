// Fill out your copyright notice in the Description page of Project Settings.

#include "XComClone.h"
#include "XComCloneCameraComponent.h"
#include "XComCloneSpectatorPawnMovement.h"


UXComCloneCameraComponent::UXComCloneCameraComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	ZoomAlpha = 1.7f;
	CameraScrollSpeed = 4000.0f;
	MinZoomLevel = ZoomAlpha;
	MaxZoomLevel = 3.0f;
	MinCameraOffset = 200;
	MaxCameraOffset = 2800;
	FixedCameraAngle = FRotator(-40.f, -15.f, 0.f);
}

void UXComCloneCameraComponent::GetCameraView(float DeltaTime, FMinimalViewInfo& DesiredView)
{
	APlayerController* Controller = GetPlayerController();
	if(Controller)
	{
		DesiredView.FOV = 30.f;
		const float CurrentOffset = MinCameraOffset + ZoomAlpha * (MaxCameraOffset - MinCameraOffset);
		FVector Pos2 = Controller->GetFocalLocation();
		DesiredView.Location = Controller->GetFocalLocation() - FixedCameraAngle.Vector() * CurrentOffset;
		DesiredView.Rotation = FixedCameraAngle;
	}
}

void UXComCloneCameraComponent::OnZoomIn()
{
	SetZoomLevel(ZoomAlpha - 0.1f);
}

void UXComCloneCameraComponent::OnZoomOut()
{
	SetZoomLevel(ZoomAlpha + 0.1f);
}

void UXComCloneCameraComponent::MoveForward(float Val)
{
	APawn* OwnerPawn = GetOwnerPawn();
	if(OwnerPawn != NULL)
	{
		APlayerController* Controller = GetPlayerController();
		if((Val != 0.f) && (Controller != NULL))
		{
			const FRotationMatrix R(Controller->PlayerCameraManager->GetCameraRotation());
			const FVector WorldSpaceAccel = R.GetScaledAxis(EAxis::Z) * 100.0f;

			// transform to world space and add it
			OwnerPawn->AddMovementInput(WorldSpaceAccel, Val);
		}
	}
}

void UXComCloneCameraComponent::MoveRight(float Val)
{
	APawn* OwnerPawn = GetOwnerPawn();
	if(OwnerPawn != NULL)
	{
		APlayerController* Controller = GetPlayerController();
		if((Val != 0.f) && (Controller != NULL))
		{
			const FRotationMatrix R(Controller->PlayerCameraManager->GetCameraRotation());
			const FVector WorldSpaceAccel = R.GetScaledAxis(EAxis::Y) * 100.0f;

			// transform to world space and add it
			OwnerPawn->AddMovementInput(WorldSpaceAccel, Val);
		}
	}
}

void UXComCloneCameraComponent::UpdateCameraMovement(const APlayerController * InPlayerController)
{
	ULocalPlayer* const LocalPlayer = Cast<ULocalPlayer>(InPlayerController->Player);
	if(LocalPlayer && LocalPlayer->ViewportClient && LocalPlayer->ViewportClient->Viewport)
	{
		FVector2D MousePosition;
		if(LocalPlayer->ViewportClient->GetMousePosition(MousePosition) == false)
		{
			return;
		}

		const float MaxSpeed = CameraScrollSpeed * FMath::Clamp(ZoomAlpha, 0.3f, 1.0f);

		float SpectatorCameraSpeed = MaxSpeed;
		ASpectatorPawn* SpectatorPawn = NULL;

		if(GetPlayerController() != NULL)
		{
			SpectatorPawn = GetPlayerController()->GetSpectatorPawn();
			if(SpectatorPawn->GetMovementComponent() != NULL)
			{
				SpectatorCameraSpeed = GetDefault<UXComCloneSpectatorPawnMovement>(SpectatorPawn->GetMovementComponent()->GetClass())->MaxSpeed;
			}
		}

		if(SpectatorPawn != NULL)
		{
			UFloatingPawnMovement* PawnMovementComponent = Cast<UFloatingPawnMovement>(SpectatorPawn->GetMovementComponent());
			if(PawnMovementComponent)
			{
				PawnMovementComponent->MaxSpeed = SpectatorCameraSpeed;
			}
		}
	}
}

void UXComCloneCameraComponent::SetCameraTarget(const FVector & CameraTarget)
{
	ASpectatorPawn* SpectatorPawn = GetPlayerController()->GetSpectatorPawn();
	if(SpectatorPawn != NULL)
	{
		SpectatorPawn->SetActorLocation(CameraTarget, false);
	}
}

void UXComCloneCameraComponent::SetZoomLevel(float NewLevel)
{
	ZoomAlpha = FMath::Clamp(NewLevel, MinZoomLevel, MaxZoomLevel);
}

APawn * UXComCloneCameraComponent::GetOwnerPawn()
{
	return Cast<APawn>(GetOwner());
}

APlayerController * UXComCloneCameraComponent::GetPlayerController()
{
	APlayerController* Controller = NULL;
	APawn* Owner = GetOwnerPawn();
	if(Owner != NULL)
	{
		Controller = Cast<APlayerController>(Owner->GetController());
	}
	return Controller;
}
