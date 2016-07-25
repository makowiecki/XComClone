// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/DefaultPawn.h"
#include "XComCloneCameraComponent.h"
#include "XComCloneSpectatorPawn.generated.h"

/**
 * 
 */
UCLASS()
class XCOMCLONE_API AXComCloneSpectatorPawn : public ADefaultPawn
{
	GENERATED_BODY()
	
public:

	AXComCloneSpectatorPawn(const FObjectInitializer& ObjectInitializer);

	/** event call on move forward input */
	virtual void MoveForward(float Value) override;

	/** event call on strafe right input */
	virtual void MoveRight(float Value) override;

	/** Handles the mouse scrolling down. */
	void OnMouseScrollUp();

	/** Handles the mouse scrolling up. */
	void OnMouseScrollDown();

	/** Add custom key bindings */
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	
	FORCEINLINE class UXComCloneCameraComponent* GetCameraComponent() const { return CameraComponent; }

private:

	UPROPERTY(Category = CameraActor, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UXComCloneCameraComponent* CameraComponent;

};
