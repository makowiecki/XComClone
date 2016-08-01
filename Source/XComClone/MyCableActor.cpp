// Fill out your copyright notice in the Description page of Project Settings.

#include "XComClone.h"
#include "MyCableActor.h"


// Sets default values
AMyCableActor::AMyCableActor()
{
	static ConstructorHelpers::FObjectFinder<UMaterial> CableComponentAsset(TEXT("Material'/Game/Materials/PathIndicator.PathIndicator'"));

	CableComponent = CreateDefaultSubobject<UCableComponent>(TEXT("CableComponent0"));
	RootComponent = CableComponent;

	CableComponent->CableLength = 1.f;
	CableComponent->NumSegments = 10;
	CableComponent->SolverIterations = 5;
	CableComponent->NumSides = 8;
	CableComponent->CableWidth = 6;

	if(CableComponentAsset.Succeeded())
	{
		CableComponent->SetMaterial(0, CableComponentAsset.Object);
	}
}