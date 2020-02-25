// Fill out your copyright notice in the Description page of Project Settings.

//#include "UObject/UnrealNames.h"
#include "GameFramework/PlayerState.h"
#include "Engine/World.h"
#include "UObject/ConstructorHelpers.h"

#include "MenuSystem/GameStatsInfo.h"
#include "Online/ShooterState.h"
#include "Online/DefaultBattlePawnGameMode.h"

#include "Player/ShooterController.h"

AShooterController::AShooterController(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	ConstructorHelpers::FClassFinder<UGameStatsInfo> MenuBPClass(TEXT("/Game/Core/Menu/WBP_GameStats"));
	if (MenuBPClass.Class)
	{
		ScoreMenuClass = MenuBPClass.Class;
	}
}

void AShooterController::SetupInputComponent()
{
	Super::SetupInputComponent();

}

bool AShooterController::GetSpectatorModeOn() const
{
	return bIsSpectatorModeOn;
}

void AShooterController::SetSpectatorModeOn(const bool& Mode)
{
	bIsSpectatorModeOn = Mode;
	if (Mode)
	{
		UE_LOG(LogTemp, Warning, TEXT("try to set SPECTATOR"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("try to set NOT SPECTATOR"));
	}
}

void AShooterController::RefreshScoreMenu()
{
	if (!ScoreMenu) return;
	ScoreMenu->SetPlayerScoreList();
}

void AShooterController::ShowScoreMenu()
{
	if (!ScoreMenu) return;
	ScoreMenu->AddToViewport();
}

void AShooterController::CloseScoreMenu()
{
	if (!ScoreMenu) return;
	ScoreMenu->RemoveFromViewport();
}

void AShooterController::ServerSetPawn_Implementation(bool IsSpectating)
{
	this->PlayerState->bIsSpectator = IsSpectating;
	this->PlayerState->bOnlySpectator = IsSpectating;
	auto State = IsSpectating ? NAME_Spectating : NAME_Playing;
	this->ChangeState(State);
	this->ClientGotoState(State);

	UE_LOG(LogTemp, Warning, TEXT("PC ServerSetPawn  Call"));

	GetWorld()->GetAuthGameMode()->RestartPlayer(this);
	
}

bool AShooterController::ServerSetPawn_Validate(bool IsSpectating)
{
	return true;
}