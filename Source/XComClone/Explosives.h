// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Explosives.generated.h"

UCLASS(abstract)
class XCOMCLONE_API AExplosives : public AActor
{
	GENERATED_BODY()
	
public:

	UPROPERTY(BlueprintReadWrite, EditAnyWhere, Category = "Explosives")
	float Damage;

	UPROPERTY(BlueprintReadWrite, EditAnyWhere, Category = "Explosives")
	int32 TileDistanceToThrowRange;

	UPROPERTY(BlueprintReadWrite, EditAnyWhere, Category = "Explosives")
	FText ExplosivesnName;

	UPROPERTY(BlueprintReadWrite, EditAnyWhere, Category = "Explosives")
	UParticleSystem *ExplosionEffect;

	// Sets default values for this actor's properties
	AExplosives();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	virtual void OnConstruction(const FTransform& Transform)override;
	
	UFUNCTION(BlueprintCallable, Category = "Explosives")
	void Explode();

	const FText& getExplosivesName()const;


	FORCEINLINE class USphereComponent* GetCollisionComponent() const { return CollisionComponent; }

	FORCEINLINE class UStaticMeshComponent* GetMesh() const { return Mesh; }

	FORCEINLINE class USphereComponent* GetExplosionRange() const { return ExplosionSphere; }

	FORCEINLINE class UProjectileMovementComponent* GetProjectileMovement() const { return ProjectileMovement; }

protected:

	UPROPERTY(VisibleDefaultsOnly, Category = Explosives)
	USphereComponent *CollisionComponent;

	UPROPERTY(VisibleDefaultsOnly, Category = Explosives)
	UStaticMeshComponent *Mesh;

	UPROPERTY(VisibleDefaultsOnly, Category = Explosives)
	USphereComponent *ExplosionSphere;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	UProjectileMovementComponent *ProjectileMovement;
	
};
