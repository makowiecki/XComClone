// Fill out your copyright notice in the Description page of Project Settings.

#include "XComClone.h"
#include "Explosives.h"
#include "Unit.h"

#define LOCTEXT_NAMESPACE "ExplosivesNamespace" 

// Sets default values
AExplosives::AExplosives()
{
	static ConstructorHelpers::FObjectFinder<UStaticMesh> ProjectileMeshAsset(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere'"));

	// Use a sphere as a simple collision representation
	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
	CollisionComponent->InitSphereRadius(150.0f);
	CollisionComponent->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	//CollisionComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);

	CollisionComponent->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComponent->CanCharacterStepUpOn = ECB_No;

	RootComponent = CollisionComponent;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
	Mesh->SetupAttachment(CollisionComponent);
	Mesh->SetRelativeScale3D(FVector(0.2f, 0.2f, 0.2f));
	Mesh->SetRelativeLocation(FVector(0.f, 0.f, -5.f));

	if(ProjectileMeshAsset.Succeeded())
	{
		Mesh->SetStaticMesh(ProjectileMeshAsset.Object);
	}


	ExplosionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("ExplosionSphere"));
	ExplosionSphere->InitSphereRadius(250.f);
	ExplosionSphere->SetupAttachment(CollisionComponent);
	ExplosionSphere->BodyInstance.SetCollisionProfileName("Explosives");
	//ExplosionSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	ExplosionSphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	
	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->UpdatedComponent = CollisionComponent;
	ProjectileMovement->InitialSpeed = 250.f;
	ProjectileMovement->MaxSpeed = 1000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	//ProjectileMovement->ProjectileGravityScale = 0.f;

 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Damage = 30;
	ExplosionRange = 250.f;

	{
		ExplosivesnName = LOCTEXT("SecondaryWeapon", "Secondary Weapon");
	}
}

// Called when the game starts or when spawned
void AExplosives::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AExplosives::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

void AExplosives::OnConstruction(const FTransform & Transform)
{
	Super::OnConstruction(Transform);

	ExplosionSphere->InitSphereRadius(ExplosionRange);
}

void AExplosives::Explode()
{
	TArray<AActor*> overlappingActors;
	ExplosionSphere->GetOverlappingActors(overlappingActors, TSubclassOf<AUnit>());

	for(size_t i = 0; i < overlappingActors.Num(); i++)
	{
		AUnit *overlappingUnit = Cast<AUnit>(overlappingActors[i]);

		if(overlappingUnit)
		{
			overlappingUnit->applyDamage(Damage);
		}
	}
	
	static ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleSystemAsset(TEXT("ParticleSystem'/Game/StarterContent/Particles/P_Explosion.P_Explosion'"));
	if(ParticleSystemAsset.Succeeded())
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ParticleSystemAsset.Object, GetActorLocation());
	}

	Destroy();
}

const FText & AExplosives::getExplosivesName() const
{
	return ExplosivesnName;
}


#undef LOCTEXT_NAMESPACE 