// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/DamageType.h"
#include "Engine/EngineTypes.h"
#include "PhysicalMaterials/PhysicalMaterial.h"


#include "Effects/ProjectileImpactEffect.h"

#include "Waepons/ShooterProjectile.h"


// Sets default values
AShooterProjectile::AShooterProjectile()
{
	PrimaryActorTick.bCanEverTick = true;
	// Use a sphere as a simple collision representation
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	
	// Players can't walk on it
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	if (GetLocalRole() == ROLE_Authority)
	{
		CollisionComp->OnComponentHit.AddDynamic(this, &AShooterProjectile::OnProjectileImpact);
	}

	// Set as root component
	RootComponent = CollisionComp;

	// Use a ProjectileMovementComponent to govern this projectile's movement
	MovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	MovementComp->UpdatedComponent = CollisionComp;
	MovementComp->InitialSpeed = 3000.f;
	MovementComp->MaxSpeed = 3000.f;
	MovementComp->bRotationFollowsVelocity = true;
	MovementComp->bShouldBounce = true;

	ParticleComp = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleComp"));
	ParticleComp->bAutoActivate = true;
	ParticleComp->bAutoDestroy = false;
	ParticleComp->SetupAttachment(RootComponent);

	DamageType = UDamageType::StaticClass();

	// Die after 3 seconds by default
	InitialLifeSpan = 3.0f;

	SetReplicates(true);
	SetReplicatingMovement(true);
}

void AShooterProjectile::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);


	if (bCollideWithSomething)
	{
		ParticleComp->SetWorldScale3D(ParticleComp->GetRelativeScale3D() * 0.90);
	}
}

void AShooterProjectile::SpawnImpactEffect(const FHitResult& Hit)
{

	auto ImpactEffect = ExplosionEffect; 

	if (Hit.Actor.Get()->ActorHasTag(FName("Player")))
	{
		ImpactEffect = MeatEffect;
	}

	if (!ImpactEffect) return;
	auto World = GetWorld();
	auto SpawnRotation = Hit.ImpactNormal.ToOrientationRotator();// GetActorRotation() * -1.f;
	FActorSpawnParameters ActorSpawnParams;
	ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	World->SpawnActor<AProjectileImpactEffect>(ImpactEffect, GetActorLocation(), SpawnRotation, ActorSpawnParams);
}

void AShooterProjectile::OnProjectileImpact(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	bCollideWithSomething = true;
	GetWorldTimerManager().SetTimer(TimerHandle, this, &AShooterProjectile::CallDestroy, 1.0f);

	if (OtherActor != GetInstigator())
	{
		SpawnImpactEffect(Hit);
		CollisionComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		CollisionComp->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		CollisionComp->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);

		if ((OtherActor != NULL) && (OtherActor != this) && (OtherComp != NULL))
		{
			UGameplayStatics::ApplyPointDamage(OtherActor, ProjectileDamage, GetActorForwardVector(), Hit, MyController.Get(), this, DamageType);
		}
	}
}


void AShooterProjectile::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	CollisionComp->MoveIgnoreActors.Add(GetInstigator());

	MyController = GetInstigatorController();
}


void AShooterProjectile::CallDestroy()
{
	Destroy();
}