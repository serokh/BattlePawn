// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ShooterProjectile.generated.h"

UCLASS()
class BATTLEPAWN_API AShooterProjectile : public AActor
{
	GENERATED_BODY()

public:
	AShooterProjectile();

	///** called when projectile hits something */
	//UFUNCTION()
	//void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);


	/** Returns CollisionComp subobject **/
	FORCEINLINE class USphereComponent* GetCollisionComp() const { return CollisionComp; }
	/** Returns ProjectileMovement subobject **/
	FORCEINLINE class UProjectileMovementComponent* GetProjectileMovement() const { return MovementComp; }

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Damage")
    TSubclassOf<UDamageType> DamageType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Damage")
	float ProjectileDamage = 1.f;
private:
	
	virtual void Tick(float DeltaSeconds) override;

	UPROPERTY(VisibleDefaultsOnly, Category = "Projectile")
	UParticleSystemComponent* ParticleComp;
	
	/** Sphere collision component */
	UPROPERTY(VisibleDefaultsOnly, Category = "Projectile")
	class USphereComponent* CollisionComp;

	/** Projectile movement component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	class UProjectileMovementComponent* MovementComp;

	/** initial setup */
	virtual void PostInitializeComponents() override;

	
	void CallDestroy();
	bool bCollideWithSomething = false;
	FTimerHandle TimerHandle;
	
	void SpawnImpactEffect(const FHitResult& Hit);

protected:

	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	TSubclassOf<class AProjectileImpactEffect> ExplosionEffect;

	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	TSubclassOf<class AProjectileImpactEffect> MeatEffect;

	UFUNCTION()
    void OnProjectileImpact(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);


	/** controller that fired me (cache for damage calculations) */
	TWeakObjectPtr<AController> MyController;

};
