// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "DefaultBattlePawnGameMode.generated.h"

/**
 * 
 */
UCLASS()
class BATTLEPAWN_API ADefaultBattlePawnGameMode : public AGameMode
{
	GENERATED_BODY()
	

	/** select best spawn point for player */
	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;


public:

	ADefaultBattlePawnGameMode(const FObjectInitializer& ObjectInitializer);
	/** notify about kills */
	virtual void Killed(AController* Killer, AController* KilledPlayer, APawn* KilledPawn, const UDamageType* DamageType);

	virtual void BeginPlay() override;

	virtual void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;

protected:

	virtual void Respawn();

	void SendGameStats();


private:

	void FindSpawnPoints();

	TArray<class APlayerStart*> SpawnPoints;

	TQueue<AController*> KilledPlayers;
};
