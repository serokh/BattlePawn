// Fill out your copyright notice in the Description page of Project Settings.

#include "Engine/Engine.h"
#include "Components/Button.h"

#include "Online/ShooterState.h"
#include "Online/DefaultBattlePawnGameMode.h"
#include "Player/ShooterController.h"

#include "MenuSystem/ChangeClassMenu.h"

bool UChangeClassMenu::Initialize()
{
	bool Success = Super::Initialize();
	if (!Success) return false;

	if (!PlayerButton) return false;
	PlayerButton->OnClicked.AddDynamic(this, &UChangeClassMenu::PlayerButtonPressed);

	if (!SpectatorButton) return false;
	SpectatorButton->OnClicked.AddDynamic(this, &UChangeClassMenu::SpectatorButtonPressed);


	return true;
}

void UChangeClassMenu::PlayerButtonPressed()
{
	ServerCallForSpawn(false);
	
	this->Teardown();
}

void UChangeClassMenu::SpectatorButtonPressed()
{

	ServerCallForSpawn(true);

	this->Teardown();
}

void UChangeClassMenu::ServerCallForSpawn_Implementation(bool Spectating)
{
	UE_LOG(LogTemp, Warning, TEXT("CLASS MENU ServerCallForSpawn Call"));
	Cast<AShooterController>(GetOwningPlayer())->ServerSetPawn(Spectating);
}
