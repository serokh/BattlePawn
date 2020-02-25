// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "ShooterState.generated.h"

/**
 * 
 */
UCLASS()
class BATTLEPAWN_API AShooterState : public APlayerState
{
	GENERATED_BODY()

	

public:

	/** clear scores */
	virtual void Reset() override;

	/** player killed someone */
	void ScoreKill(AShooterState* Victim);

	/** player died */
	void ScoreDeath(AShooterState* KilledBy);

	/** get current team */
	int32 GetDeaths() const;

	/** get number of kills */
	int32 GetKills() const;


protected:

	/** number of kills */
	UPROPERTY(Transient, Replicated)
	int32 NumKills = 0;

	/** number of deaths */
	UPROPERTY(Transient, Replicated)
	int32 NumDeaths = 0;

public:

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};

