// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Components/EditableTextBox.h"
#include "Engine/Level.h"
#include "Engine/World.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"
#include "Components/EditableTextBox.h"
#include "GameFramework/PlayerState.h"

#include "MenuSystem/ServerRow.h"
#include "Player/ShooterController.h"

#include "MenuSystem/MainMenu.h"

UMainMenu::UMainMenu(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	ConstructorHelpers::FClassFinder<UUserWidget> ServerRowBPClass(TEXT("/Game/Core/Menu/WBP_ServerRow"));
	if (ServerRowBPClass.Class)
	{
		ServerRowClass = ServerRowBPClass.Class;
	}

}

void UMainMenu::SetServerList(TArray<FServerData> ServerData)
{
	auto World = this->GetWorld();
	if (World && ServerList)
	{
		uint32 i = 0;
		ServerList->ClearChildren();
		for (const FServerData& ServerInstance : ServerData)
		{
			UServerRow* Row = CreateWidget<UServerRow>(World, ServerRowClass);
			if (Row)
			{
				Row->ServerName->SetText(FText::FromString(ServerInstance.ServerName));
				Row->HostName->SetText(FText::FromString(ServerInstance.HostUsername));

				auto OpenSlotsString = FString::Printf(TEXT("%d/%d"), ServerInstance.CurrentPlayers, ServerInstance.MaxPlayers);
				Row->OpenSlots->SetText(FText::FromString(OpenSlotsString));

				ServerList->AddChild(Row);
				Row->Setup(this, i);
				++i;
			}
		}
	}
}

void UMainMenu::SelectIndex(uint32 Index)
{
	SelectedIndex = Index;
	UpdateServerNames();
}

void UMainMenu::UpdateServerNames()
{
	for (int32 i = 0; i < ServerList->GetChildrenCount(); i++)
	{
		auto Row = Cast<UServerRow>(ServerList->GetChildAt(i));
		if (Row && SelectedIndex.IsSet())
		{
			Row->Selected = (SelectedIndex.GetValue() == i);
		}
	}
}


void UMainMenu::OnLevelRemovedFromWorld(ULevel* InLevel, UWorld* InWorld)
{
	Super::OnLevelRemovedFromWorld(InLevel, InWorld);
	FInputModeGameOnly GameInputMode; // For input focus only for game
	if (PlayerController)
	{
		PlayerController->SetInputMode(GameInputMode);
		PlayerController->bShowMouseCursor = false;
	}
}

bool UMainMenu::Initialize()
{
	bool Success = Super::Initialize();
	if (!Success) return false;

	if (!HostButton) return false;
	HostButton->OnClicked.AddDynamic(this, &UMainMenu::OpenHostMenu);

	if (!JoinButton) return false;
	JoinButton->OnClicked.AddDynamic(this, &UMainMenu::OpenJoinMenu);

	if (!CancelButton) return false;
	CancelButton->OnClicked.AddDynamic(this, &UMainMenu::OpenMainMenu);

	if (!JoinConfirmButton) return false;
	JoinConfirmButton->OnClicked.AddDynamic(this, &UMainMenu::JoinServer);

	if (!QuitGameButton) return false;
	QuitGameButton->OnClicked.AddDynamic(this, &UMainMenu::QuitGame);

	if (!CancelHostButton) return false;
	CancelHostButton->OnClicked.AddDynamic(this, &UMainMenu::OpenMainMenu);

	if (!ConfirmHostButton) return false;
	ConfirmHostButton->OnClicked.AddDynamic(this, &UMainMenu::HostServer);

	return true;
}

void UMainMenu::HostServer()
{
	if (MainMenuInterface && InputServerName)
	{
		FString ServerName = InputServerName->GetText().ToString();
		MainMenuInterface->Host(ServerName);
	}
}

void UMainMenu::JoinServer()
{
	if (SelectedIndex.IsSet() && MainMenuInterface != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Selected index %d."), SelectedIndex.GetValue());
		MainMenuInterface->Join(SelectedIndex.GetValue());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Selected index not set."));
	}

}

void UMainMenu::OpenJoinMenu()
{
	if (MenuSwitcher && JoinMenu)
	{
		MenuSwitcher->SetActiveWidget(JoinMenu);
	}
	if (MainMenuInterface)
	{
		MainMenuInterface->RefreshServerList();
	}

}

void UMainMenu::OpenMainMenu()
{
	if (MenuSwitcher && JoinMenu)
	{
		MenuSwitcher->SetActiveWidget(MainMenu);
	}
}

void UMainMenu::OpenHostMenu()
{
	if (MenuSwitcher && JoinMenu)
	{
		MenuSwitcher->SetActiveWidget(HostMenu);
	}
}

void UMainMenu::QuitGame()
{
	if (PlayerController)
	{
		PlayerController->ConsoleCommand("quit");
	}
}

