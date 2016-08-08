// Fill out your copyright notice in the Description page of Project Settings.

#include "XComClone.h"
#include "PathActor.h"


// Sets default values
APathActor::APathActor()
{
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshComponentAsset(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere'"));

	BaseRoot = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	BaseRoot->SetupAttachment(RootComponent);
	
	mMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	mMesh->SetupAttachment(BaseRoot);
	mMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	mMesh->SetRelativeScale3D(FVector(0.4f, 0.4f, 0.4f));
	mMesh->SetRelativeLocation(FVector(0.f, 0.f, -15.f));
	
	if(MeshComponentAsset.Succeeded())
	{
		mMesh->SetStaticMesh(MeshComponentAsset.Object);
	}

	SetActorEnableCollision(false);
}

void APathActor::SetVisibility(bool newVisibility)
{
	mMesh->SetVisibility(newVisibility);
}