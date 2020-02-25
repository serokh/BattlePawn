// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProjectileImpactEffect.generated.h"

UCLASS()
class BATTLEPAWN_API AProjectileImpactEffect : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectileImpactEffect();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

//public:	
//	// Called every frame
//	virtual void Tick(float DeltaTime) override;


///////////////////////////////////////////////////////
/////////////////Visual Properties

public:

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	class USoundBase* ExplosionSound;

private:
	
	UPROPERTY(VisibleDefaultsOnly, Category = "Projectile")
	UParticleSystemComponent* ParticleComp;


};
