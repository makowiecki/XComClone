// Fill out your copyright notice in the Description page of Project Settings.

#include "XComClone.h"
#include "Projectile.h"


// Sets default values
AProjectile::AProjectile()
{
	static ConstructorHelpers::FObjectFinder<UStaticMesh> ProjectileMeshAsset(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere'"));

	// Use a sphere as a simple collision representation
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(15.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionComp->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);

	FScriptDelegate OnHitDelegate;
	OnHitDelegate.BindUFunction(this, "OnHit");
	CollisionComp->OnComponentHit.Add(OnHitDelegate);	

	// Players can't walk on it
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	// Set as root component
	RootComponent = CollisionComp;

	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
	ProjectileMesh->SetupAttachment(CollisionComp);
	ProjectileMesh->SetRelativeScale3D(FVector(0.2f, 0.2f, 0.2f));
	ProjectileMesh->SetRelativeLocation(FVector(0.f, 0.f, -10.f));

	if(ProjectileMeshAsset.Succeeded())
	{
		ProjectileMesh->SetStaticMesh(ProjectileMeshAsset.Object);
	}

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 1000.f;
	ProjectileMovement->MaxSpeed = 1000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->ProjectileGravityScale = 0.f;

	// Die after 2 seconds by default
	InitialLifeSpan = 2.0f;
}

void AProjectile::OnHit(AActor * OtherActor, UPrimitiveComponent * OtherComp, FVector NormalImpulse, const FHitResult & Hit)
{
	if((OtherActor != NULL) && (OtherActor != this)/* && (OtherComp != NULL) && OtherComp->IsSimulatingPhysics()*/)
	{
		Destroy();
	}
}
