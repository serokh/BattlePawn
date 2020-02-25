// Fill out your copyright notice in the Description page of Project Settings.


#include "Net/UnrealNetwork.h"

#include "Online/ShooterState.h"


void AShooterState::Reset()
{
	Super::Reset();
	NumKills = 0;
	NumDeaths = 0;
}

void AShooterState::ScoreKill(AShooterState* Victim)
{
	NumKills++;
}

void AShooterState::ScoreDeath(AShooterState* KilledBy)
{
	NumDeaths++;
}

int32 AShooterState::GetDeaths() const
{
	return NumDeaths;
}

int32 AShooterState::GetKills() const
{
	return NumKills;
}

void AShooterState::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AShooterState, NumKills);
	DOREPLIFETIME(AShooterState, NumDeaths);
}