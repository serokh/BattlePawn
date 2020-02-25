// Fill out your copyright notice in the Description page of Project Settings.


#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundAttenuation.h"


#include "Effects/ProjectileImpactEffect.h"

// Sets default values
AProjectileImpactEffect::AProjectileImpactEffect()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ParticleComp = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleComp"));
	ParticleComp->bAutoActivate = true;
	ParticleComp->bAutoDestroy = true;
	//ParticleComp->SetupAttachment(RootComponent);
	SetRootComponent(ParticleComp);
	InitialLifeSpan = 1.0f;
}

// Called when the game starts or when spawned
void AProjectileImpactEffect::BeginPlay()
{
	Super::BeginPlay();
	if (ExplosionSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, ExplosionSound, GetActorLocation());
	}
}

//// Called every frame
//void AProjectileImpactEffect::Tick(float DeltaTime)
//{
//	Super::Tick(DeltaTime);
//
//}

