// Fill out your copyright notice in the Description page of Project Settings.

#include "Online/ShooterState.h"

#include "Online/BattlePawnGameState.h"

void ABattlePawnGameState::GetGameScore(TArray<FPlayerStateInfo>& OutRankedPlayersArray) const
{
	OutRankedPlayersArray.Empty();

	TArray<FPlayerStateInfo> NotSortedStateInfo;
	for (const auto& PlayerState : PlayerArray)
	{
		FPlayerStateInfo PlayerStateInfo;
		PlayerStateInfo.NumKills = Cast<AShooterState>(PlayerState)->GetKills();
		PlayerStateInfo.NumDeaths = Cast<AShooterState>(PlayerState)->GetDeaths();
		PlayerStateInfo.PlayerName = Cast<AShooterState>(PlayerState)->GetPlayerName();
		PlayerStateInfo.bIsSpectator = Cast<AShooterState>(PlayerState)->bIsSpectator;

		NotSortedStateInfo.Add(PlayerStateInfo);
	}
	NotSortedStateInfo.Sort([](const FPlayerStateInfo& A, const FPlayerStateInfo& B)
		{ return (A.NumKills - A.NumDeaths) > (B.NumKills - B.NumDeaths); });

	OutRankedPlayersArray = NotSortedStateInfo;
}