// Fill out your copyright notice in the Description page of Project Settings.

#include "XComClone.h"
#include "Unit.h"


// Sets default values
AUnit::AUnit()
{
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SkeletalMeshAsset(TEXT("SkeletalMesh'/Game/Mannequin/Character/Mesh/SK_Mannequin.SK_Mannequin'"));
	static ConstructorHelpers::FObjectFinder<UAnimBlueprint> AnimClassAsset(TEXT("AnimBlueprint'/Game/Mannequin/Animations/ThirdPerson_AnimBP.ThirdPerson_AnimBP'"));

	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	GetCapsuleComponent()->bGenerateOverlapEvents = true;

	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetMesh()->SetRelativeLocationAndRotation(FVector(0.f, 0.f, -88.f), FRotator(0.f, -90.f, 0.f));
	
	if(SkeletalMeshAsset.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SkeletalMeshAsset.Object);
	}

	if(AnimClassAsset.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(AnimClassAsset.Object->GetAnimBlueprintGeneratedClass());
	}

	
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	bIsMoving = false;


}

// Called when the game starts or when spawned
void AUnit::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AUnit::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	if(bIsMoving)
	{		
		FVector actorFloorLocation(GetActorLocation().X, GetActorLocation().Y, 0.f);
		FVector moveVector = dstLocation - actorFloorLocation;
		moveVector.Normalize();
		
		AddMovementInput(moveVector, 1.f);
		
		actorFloorLocation.X = GetActorLocation().X;
		actorFloorLocation.Y = GetActorLocation().Y;
		if(FVector::PointsAreNear(actorFloorLocation, dstLocation, 1.f))
		{
			bIsMoving = false;
		}
	}
}

// Called to bind functionality to input
void AUnit::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

}

void AUnit::moveToLocation(const FVector & destination)
{
	dstLocation = destination;
	bIsMoving = true;
}

