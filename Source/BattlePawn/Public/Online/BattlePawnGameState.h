// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BattlePawnTypes.h"

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "BattlePawnGameState.generated.h"

/**
 * 
 */
UCLASS()
class BATTLEPAWN_API ABattlePawnGameState : public AGameState
{
	GENERATED_BODY()

public:

	void GetGameScore(TArray<FPlayerStateInfo> &OutRankedPlayersArray) const;
	
};
