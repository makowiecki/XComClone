// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Camera/CameraComponent.h"
#include "XComCloneCameraComponent.generated.h"

/**
 * 
 */
UCLASS()
class XCOMCLONE_API UXComCloneCameraComponent : public UCameraComponent
{
	GENERATED_BODY()
	
public:

	/** How fast the camera moves around when the mouse is at the edge of the screen. */
	UPROPERTY(BlueprintReadWrite, EditAnyWhere, Category = "Camera")
	float CameraScrollSpeed;
	
	/** Minimum amount of camera zoom (How close we can get to the map). */
	UPROPERTY(BlueprintReadWrite, EditAnyWhere, Category = "Camera")
	float MinZoomLevel;

	/** Maximum amount of camera zoom (How close we can get to the map). */
	UPROPERTY(BlueprintReadWrite, EditAnyWhere, Category = "Camera")
	float MaxZoomLevel;

	/** The minimum offset of the camera. */
	UPROPERTY(BlueprintReadWrite, EditAnyWhere, Category = "Camera")
	float MinCameraOffset;

	/** The maximum offset of the camera. */
	UPROPERTY(BlueprintReadWrite, EditAnyWhere, Category = "Camera")
	float MaxCameraOffset;

	/** The angle to look down on the map. */
	UPROPERTY(BlueprintReadWrite, EditAnyWhere, Category = "Camera")
	FRotator FixedCameraAngle;


	UXComCloneCameraComponent(const FObjectInitializer& ObjectInitializer);

	// Begin UCameraComponent interface

	virtual void GetCameraView(float DeltaTime, FMinimalViewInfo& DesiredView) override;

	// End UCameraComponent interface

	/** Handle zooming in. */
	void OnZoomIn();

	/** Handle zooming out. */
	void OnZoomOut();
	
	/*
	* Move the camera on the forward axis
	*
	* @param	Val		Amount to move
	*/
	void MoveForward(float Val);

	/*
	* Move the camera on the left/right axis
	*
	* @param	Val		Amount to move
	*/
	void MoveRight(float Val);

	/*
	* Update the mouse controlled camera movement.
	*
	* @param	InPlayerController		The relevant player controller.
	*/
	void UpdateCameraMovement(const APlayerController* InPlayerController);
	
	/** Set the desired camera position. */
	void SetCameraTarget(const FVector& CameraTarget);

	/** Sets the desired zoom level; clamping if necessary */
	void SetZoomLevel(float NewLevel);

private:

	/** Return the pawn that owns this component. */
	APawn* GetOwnerPawn();

	/** Return the player controller of the pawn that owns this component. */
	APlayerController* GetPlayerController();
	
	/** Current amount of camera zoom. */
	float ZoomAlpha;
};
