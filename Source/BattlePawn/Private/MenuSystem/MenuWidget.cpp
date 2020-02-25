// Fill out your copyright notice in the Description page of Project Settings.

#include "Player/ShooterController.h"

#include "MenuSystem/MenuWidget.h"

void UMenuWidget::Setup()
{
	this->AddToViewport();

	UWorld* World = GetWorld();
	if (!World) return;
	PlayerController = GetWorld()->GetFirstPlayerController();
	if (PlayerController)
	{
		FInputModeUIOnly MenuInputMode; // For input focus only for menu
		this->bIsFocusable = true;
		MenuInputMode.SetWidgetToFocus(this->TakeWidget());
		MenuInputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		PlayerController->SetInputMode(MenuInputMode);
		PlayerController->bShowMouseCursor = true;
	}
}

void UMenuWidget::Teardown()
{
	this->RemoveFromViewport();

	UWorld* World = GetWorld();
	if (!World) return;

	//PlayerController = GetWorld()->GetFirstPlayerController();
	if (PlayerController)
	{
		FInputModeGameOnly GameInputMode; // For input focus only for menu
		this->bIsFocusable = false;
		this->Destruct();
		PlayerController->SetInputMode(GameInputMode);
		PlayerController->bShowMouseCursor = false;
	}
}

void UMenuWidget::SetMenuInterface(IMenuInterface* MenuInterface)
{
	MainMenuInterface = MenuInterface;
}