// Fill out your copyright notice in the Description page of Project Settings.

#include "TimerManager.h"
#include "GameFramework/PlayerStart.h"
#include "Engine/PlayerStartPIE.h"
#include "Engine/World.h"
#include "EngineUtils.h"
#include "Containers/Queue.h"
#include "GameFramework/SpectatorPawn.h"

#include "GameFramework/GameState.h"
#include "Engine/GameInstance.h"
#include "Engine/Engine.h"

#include "Player/ShooterController.h"
#include "Online/ShooterState.h"

#include "Online/DefaultBattlePawnGameMode.h"


ADefaultBattlePawnGameMode::ADefaultBattlePawnGameMode(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	MinRespawnDelay = 4.0f;
}

void ADefaultBattlePawnGameMode::Killed(AController* Killer, AController* KilledPlayer, APawn* KilledPawn, const UDamageType* DamageType)
{
	AShooterState* KillerPlayerState = Killer ? Cast<AShooterState>(Killer->PlayerState) : NULL;
	AShooterState* VictimPlayerState = KilledPlayer ? Cast<AShooterState>(KilledPlayer->PlayerState) : NULL;

	if (KillerPlayerState && KillerPlayerState != VictimPlayerState)
	{
		KillerPlayerState->ScoreKill(VictimPlayerState);
	}

	if (VictimPlayerState)
	{
		UE_LOG(LogTemp, Warning, TEXT("Try score death"));
		VictimPlayerState->ScoreDeath(KillerPlayerState);
	}

	//TODO Make Function to update Stats Not in Tick but in "Killed"
	//SendGameStats();

	KilledPlayers.Enqueue(KilledPlayer);
	
	FTimerHandle TimeHandle;
	UWorld* World = GetWorld();
	World->GetTimerManager().SetTimer(TimeHandle, this, &ADefaultBattlePawnGameMode::Respawn, MinRespawnDelay, false);
	
}

void ADefaultBattlePawnGameMode::BeginPlay()
{
	Super::BeginPlay();
	//FindSpawnPoints();
	UE_LOG(LogTemp, Warning, TEXT("Spawn points: %d"), SpawnPoints.Num());
}

void ADefaultBattlePawnGameMode::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{

	NewPlayer->PlayerState->bIsSpectator = true;
	NewPlayer->PlayerState->bOnlySpectator = true;
	NewPlayer->ChangeState(NAME_Spectating);
	NewPlayer->ClientGotoState(NAME_Spectating);

	Super::HandleStartingNewPlayer_Implementation(NewPlayer);
	
}

void ADefaultBattlePawnGameMode::Respawn()
{
	if (SpawnPoints.IsValidIndex(0) && !KilledPlayers.IsEmpty())
	{
		AController* PlayerToRespawn;
		KilledPlayers.Dequeue(PlayerToRespawn);
		PlayerToRespawn->GetPawn()->DetachFromControllerPendingDestroy();

		PlayerToRespawn->UnPossess();
		UE_LOG(LogTemp, Warning, TEXT("Respawn Call"));
		auto x = FMath::RandHelper(SpawnPoints.Num());
		UE_LOG(LogTemp, Warning, TEXT("Respawn point is : %d"), x);
		auto Pawn = SpawnDefaultPawnFor(PlayerToRespawn, SpawnPoints[x]);
		PlayerToRespawn->Possess(Pawn);
	}
}

void ADefaultBattlePawnGameMode::SendGameStats()
{
	for (auto& PlayerState : GameState->PlayerArray)
	{
		auto PC = Cast<AShooterController>(PlayerState->GetPawn()->GetController());
		if (PC)
		{
			PC->RefreshScoreMenu();
		}
	}
}

void ADefaultBattlePawnGameMode::FindSpawnPoints()
{
	if (GetLocalRole() == ROLE_Authority)
	{
		for (TActorIterator<APlayerStart> MyIt(GetWorld()); MyIt; ++MyIt)
		{
			SpawnPoints.Add(*MyIt);
		}
	}
}



AActor* ADefaultBattlePawnGameMode::ChoosePlayerStart_Implementation(AController* Player)
{
	FindSpawnPoints();

	APlayerStart* BestStart = NULL;

	BestStart =  SpawnPoints[FMath::RandHelper(SpawnPoints.Num())];

	return BestStart ? BestStart : Super::ChoosePlayerStart_Implementation(Player);
}

