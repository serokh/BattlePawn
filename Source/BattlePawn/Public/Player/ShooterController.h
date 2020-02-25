// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ShooterController.generated.h"

/**
 * 
 */
UCLASS()
class BATTLEPAWN_API AShooterController : public APlayerController
{
	GENERATED_BODY()

	
	
public:

	AShooterController(const FObjectInitializer& ObjectInitializer);

	virtual void SetupInputComponent() override;
	
	bool GetSpectatorModeOn() const;
	void SetSpectatorModeOn(const bool& Mode);

	/* Set PlayerState On Server For This Controller */
	UFUNCTION(Reliable, Server, WithValidation)
	virtual void ServerSetPawn(bool IsSpectating);

	void RefreshScoreMenu();

private:
	
	bool bIsSpectatorModeOn = false;

	TSubclassOf<class UGameStatsInfo> ScoreMenuClass;

	UGameStatsInfo* ScoreMenu;

	UFUNCTION()
	void ShowScoreMenu();

	UFUNCTION()
	void CloseScoreMenu();

};
