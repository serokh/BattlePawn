// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Button.h"

#include "MenuSystem/InGameMenu.h"

bool UInGameMenu::Initialize()
{
	bool Success = Super::Initialize();
	if (!Success) return false;

	if (!CloseMenuButton) return false;
	CloseMenuButton->OnClicked.AddDynamic(this, &UInGameMenu::CancelPressed);

	if (!QuitButton) return false;
	QuitButton->OnClicked.AddDynamic(this, &UInGameMenu::QuitPressed);

	return true;
}

void UInGameMenu::CancelPressed()
{
	this->Teardown();
}

void UInGameMenu::QuitPressed()
{
	MainMenuInterface->QuitToMenu();
}
