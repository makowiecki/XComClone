// Fill out your copyright notice in the Description page of Project Settings.

#include "XComClone.h"
#include "Explosives.h"
#include "Unit.h"

#define LOCTEXT_NAMESPACE "ExplosivesNamespace" 

// Sets default values
AExplosives::AExplosives()
{
	static ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleSystemAsset(TEXT("ParticleSystem'/Game/StarterContent/Particles/P_Explosion.P_Explosion'"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> ProjectileMeshAsset(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere'"));

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
	Mesh->SetRelativeScale3D(FVector(0.2f, 0.2f, 0.2f));

	if(ProjectileMeshAsset.Succeeded())
	{
		Mesh->SetStaticMesh(ProjectileMeshAsset.Object);
	}

	RootComponent = Mesh;


	// Use a sphere as a simple collision representation
	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
	CollisionComponent->SetupAttachment(Mesh);
	CollisionComponent->SetWorldScale3D(FVector(1.f, 1.f, 1.f));
	CollisionComponent->InitSphereRadius(150.0f);
	CollisionComponent->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	

	ExplosionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("ExplosionSphere"));
	CollisionComponent->SetupAttachment(Mesh);
	CollisionComponent->SetWorldScale3D(FVector(1.f, 1.f, 1.f));
	ExplosionSphere->InitSphereRadius(250.f);
	ExplosionSphere->BodyInstance.SetCollisionProfileName("Explosives");
	ExplosionSphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	
	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->UpdatedComponent = Mesh;
	ProjectileMovement->InitialSpeed = 0.f;
	ProjectileMovement->MaxSpeed = 0.f;
	ProjectileMovement->bShouldBounce = true;
	ProjectileMovement->Bounciness = 0.7f;

 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Damage = 30;

	{
		ExplosivesnName = LOCTEXT("SecondaryWeapon", "Secondary Weapon");
	}

	if(ParticleSystemAsset.Succeeded())
	{
		ExplosionEffect = ParticleSystemAsset.Object;
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
	
	if(ExplosionEffect)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionEffect, GetActorLocation());
	}

	Destroy();
}

const FText & AExplosives::getExplosivesName() const
{
	return ExplosivesnName;
}


#undef LOCTEXT_NAMESPACE 