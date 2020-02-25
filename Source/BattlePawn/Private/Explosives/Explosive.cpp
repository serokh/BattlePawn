// Fill out your copyright notice in the Description page of Project Settings.

#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "Engine/World.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/DamageType.h"

#include "Effects/ExplosionEffect.h"

#include "Explosives/Explosive.h"

// Sets default values
AExplosive::AExplosive(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PropMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(PropMesh);

	SetReplicates(true);
	NetUpdateFrequency = 30.f;
	//bReplicateMovement = true;
}

// Called when the game starts or when spawned
void AExplosive::BeginPlay()
{
	Super::BeginPlay();
	
}

//// Called every frame
//void AExplosive::Tick(float DeltaTime)
//{
//	Super::Tick(DeltaTime);
//
//}

float AExplosive::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	FTimerDelegate TimerDel;
	TimerDel.BindUFunction(this, FName("Explode"), DamageCauser, EventInstigator);
	GetWorldTimerManager().SetTimer(ExplosionTimerHandle, TimerDel, ExplodeTimer, false);
	return DamageAmount;
}

void AExplosive::Explode(AActor* DamageCauser, AController* EventInstigator)
{
	SimulateExplosion(DamageCauser, EventInstigator);
}

void AExplosive::ExplosiveRespawn()
{
	PropMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	PropMesh->SetVisibility(true);
}

void AExplosive::SimulateExplosion_Implementation(AActor* DamageCauser, AController* EventInstigator)
{
	auto World = GetWorld();

	if (World && ExplosionEffect)
	{
		FActorSpawnParameters ActorSpawnParams;
		ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		FTransform SpawnTranform = FTransform(GetActorRotation(),FVector_NetQuantize(GetActorLocation()), GetActorScale3D() * ExplosionRadius / 300.f);
		//World->SpawnActor<AExplosionEffect>(ExplosionEffect, GetActorLocation(), GetActorRotation(), ActorSpawnParams);		
		World->SpawnActor<AExplosionEffect>(ExplosionEffect, SpawnTranform, ActorSpawnParams);

		TArray<AActor*> ActorsToIgonre;
		ActorsToIgonre.Add(this);
		auto DamageType = UDamageType::StaticClass();
		UGameplayStatics::ApplyRadialDamage
		(this,
			ExplosionDamage,
			GetActorLocation(),
			ExplosionRadius,
			DamageType,
			ActorsToIgonre,
			DamageCauser,
			EventInstigator);
	}
	if (!bRespawnable)
	{
		Destroy();
	}
	else
	{
		PropMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		PropMesh->SetVisibility(false);
		World->GetTimerManager().SetTimer(RespawnTimerHandle, this, &AExplosive::ExplosiveRespawn, RespawnTimer, false);
	}
}
