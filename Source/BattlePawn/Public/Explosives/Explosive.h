// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Explosive.generated.h"

UCLASS()
class BATTLEPAWN_API AExplosive : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AExplosive(const class FObjectInitializer& ObjectInitializer);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	TSubclassOf<class AExplosionEffect> ExplosionEffect;


///////////////////////////////////////////////////////
/////////////////Explosion

public:	
	//// Called every frame
	//virtual void Tick(float DeltaTime) override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	UFUNCTION()
	void Explode(AActor* DamageCauser, AController* EventInstigator);

	UFUNCTION(Reliable, NetMulticast)
	void SimulateExplosion(AActor* DamageCauser, AController* EventInstigator);

	
private:


	/** Explosive mesh */
	UPROPERTY(VisibleDefaultsOnly, Category=Mesh)
	class UStaticMeshComponent* PropMesh;

	UPROPERTY(EditAnywhere)
	float ExplosionDamage = 2.f;

	UPROPERTY(EditAnywhere)
	float ExplosionRadius = 60.f;

	UPROPERTY(EditAnywhere)
	float ExplodeTimer = 0.5f;
	FTimerHandle ExplosionTimerHandle;

///////////////////////////////////////////////////////
/////////////////Respawn 

private:
	   
	void ExplosiveRespawn();

	UPROPERTY(EditAnywhere)
	bool bRespawnable = true;

	UPROPERTY(EditAnywhere)
	float RespawnTimer = 15.f;
	FTimerHandle RespawnTimerHandle;
};
