// Fill out your copyright notice in the Description page of Project Settings.


#include "Engine/Engine.h"
#include "GameFramework/PlayerController.h"
#include "Blueprint/UserWidget.h"
#include "UObject/ConstructorHelpers.h"

#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"

#include "MenuSystem/MainMenu.h"
#include "MenuSystem/MenuWidget.h"

#include "BattlePawnGameInstance.h"

const static FName SESSION_NAME = TEXT("Game");
const static FName SERVER_NAME_SETTINGS_KEY = TEXT("ServerName");

UBattlePawnGameInstance::UBattlePawnGameInstance(const FObjectInitializer& ObjectInitializer)
{
	ConstructorHelpers::FClassFinder<UUserWidget> MenuBPClass(TEXT("/Game/Core/Menu/WBP_MainMenu"));
	if (MenuBPClass.Class)
	{
		MenuClass = MenuBPClass.Class;
	}
	ConstructorHelpers::FClassFinder<UUserWidget> InGameMenuBPClass(TEXT("/Game/Core/Menu/WBP_InGameMenu"));
	if (InGameMenuBPClass.Class)
	{
		InGameMenuClass = InGameMenuBPClass.Class;
	}
	ConstructorHelpers::FClassFinder<UUserWidget> ChangeClassMenuBPClass(TEXT("/Game/Core/Menu/WBP_ChangeClassMenu"));
	if (ChangeClassMenuBPClass.Class)
	{
		ChangeClassMenuClass = ChangeClassMenuBPClass.Class;
	}

}

void UBattlePawnGameInstance::Init()
{
	Super::Init();
	IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get();
	if (OnlineSubsystem)
	{
		UE_LOG(LogTemp, Warning, TEXT("OnlineSubsystem: %s"), *OnlineSubsystem->GetSubsystemName().ToString());
		OnlineSessionInterface = OnlineSubsystem->GetSessionInterface();
		if (OnlineSessionInterface.IsValid())
		{
			UE_LOG(LogTemp, Warning, TEXT("OnlineSession found"));

			OnlineSessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UBattlePawnGameInstance::OnCreateSessionComplete);
			OnlineSessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, &UBattlePawnGameInstance::OnDestroySessionComplete);
			OnlineSessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UBattlePawnGameInstance::OnFindSessionsComplete);
			OnlineSessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UBattlePawnGameInstance::OnJoinSessionComplete);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("OnlineSubsystem not found"));
	}
}

void UBattlePawnGameInstance::LoadMenu()
{
	if (MenuClass)
	{
		Menu = CreateWidget<UMenuWidget>(this, MenuClass);
		if (Menu)
		{
			Menu->Setup();
			Menu->SetMenuInterface(this);
		}

	}
}

void UBattlePawnGameInstance::LoadChangeClassMenu()
{
	if (ChangeClassMenuClass)
	{
		Menu = CreateWidget<UMenuWidget>(this, ChangeClassMenuClass);
		if (Menu)
		{
			Menu->Setup();
			Menu->SetMenuInterface(this);
		}

	}
}

void UBattlePawnGameInstance::LoadInGameMenu()
{
	if (InGameMenuClass)
	{
		Menu = CreateWidget<UMenuWidget>(this, InGameMenuClass);
		if (Menu)
		{
			Menu->Setup();
			Menu->SetMenuInterface(this);
		}

	}
}

void UBattlePawnGameInstance::Host(FString& ServerName)
{
	if (OnlineSessionInterface.IsValid())
	{

		if (OnlineSessionInterface->GetNamedSession(FName(*ServerName)) != nullptr)
		{
			//destroy than in delegate call CreateSession
			OnlineSessionInterface->DestroySession(FName(*ServerName));
		}
		else
		{
			CreateSession(ServerName);
		}

	}
}

void UBattlePawnGameInstance::Join(uint32 Index)
{

	if (!OnlineSessionInterface.IsValid()) return;
	if (!SessionSearch.IsValid()) return;
	OnlineSessionInterface->JoinSession(0, TEXT("My Session Game"), SessionSearch->SearchResults[Index]);
}

void UBattlePawnGameInstance::QuitToMenu()
{
	UEngine* Engine = GetEngine();
	if (Engine)
	{
		Engine->AddOnScreenDebugMessage(0, 3, FColor::Yellow, FString::Printf(TEXT("Quiting from server")));
	}

	APlayerController* PlayerController = GetFirstLocalPlayerController();
	if (!PlayerController) return;

	PlayerController->ClientTravel("/Game/MenuSystem/MainMenu", ETravelType::TRAVEL_Absolute);
}

void UBattlePawnGameInstance::RefreshServerList()
{
	SessionSearch = MakeShareable(new FOnlineSessionSearch()); // Make new ptr to FOnlineSessionSearc and than make shared ptr from it
	if (SessionSearch.IsValid())
	{
		//SessionSearch->bIsLanQuery = true; //what kind of session you search
		SessionSearch->MaxSearchResults = 100;
		SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);
		UE_LOG(LogTemp, Warning, TEXT("Start session search"));
		OnlineSessionInterface->FindSessions(0, SessionSearch.ToSharedRef()); //Because SessionSearch is ptr, but you need ref
	}
}

void UBattlePawnGameInstance::StartSession()
{
	if (OnlineSessionInterface.IsValid())
	{
		OnlineSessionInterface->StartSession(SESSION_NAME);
	}
}

void UBattlePawnGameInstance::OnCreateSessionComplete(FName SessionName, bool Success)
{
	if (!Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("Could not create session"));
		return;
	}
	UEngine* Engine = GetEngine();
	if (Engine)
	{
		Engine->AddOnScreenDebugMessage(0, 3, FColor::Yellow, TEXT("Hosting"));
	}
	UWorld* World = GetWorld();
	if (World)
	{
		World->ServerTravel("/Game/Maps/ArenaLevel?listen");

	}
}

void UBattlePawnGameInstance::OnDestroySessionComplete(FName SessionName, bool Success)
{
	if (Success)
	{
		auto x = SessionName.ToString();
		CreateSession(x);
	}
}

void UBattlePawnGameInstance::OnFindSessionsComplete(bool Success)
{
	if (Success && SessionSearch.IsValid() && Menu)
	{
		UE_LOG(LogTemp, Warning, TEXT("Success session search"));
		//Get all results from session search
		TArray<FServerData> ServerData;
		for (const FOnlineSessionSearchResult& SearchResult : SessionSearch->SearchResults)
		{
			UE_LOG(LogTemp, Warning, TEXT("Session id: %s"), *SearchResult.GetSessionIdStr());

			FServerData Data;
			FString ServerName;

			if (SearchResult.Session.SessionSettings.Get(SERVER_NAME_SETTINGS_KEY, ServerName))
			{
				Data.ServerName = ServerName;
				auto SessionSettings = SearchResult.Session.SessionSettings;
				OnlineSessionInterface->UpdateSession(FName(*ServerName), SessionSettings);
			}
			else
			{
				Data.ServerName = "Error: No Name";
			}
			Data.MaxPlayers = SearchResult.Session.SessionSettings.NumPublicConnections;

			Data.CurrentPlayers = Data.MaxPlayers - SearchResult.Session.NumOpenPublicConnections;
			Data.HostUsername = SearchResult.Session.OwningUserName;
			ServerData.Add(Data);
			UE_LOG(LogTemp, Warning, TEXT("Found session names: %s"), *SearchResult.GetSessionIdStr());
		}
		Cast<UMainMenu>(Menu)->SetServerList(ServerData);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Fail session search"));
	}
}

void UBattlePawnGameInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	if (!OnlineSessionInterface.IsValid()) return;

	FString Address;
	if (!OnlineSessionInterface->GetResolvedConnectString(SessionName, Address)) //Adress is out param
	{
		UE_LOG(LogTemp, Warning, TEXT("Could not get connect string."));
		return;
	}
	UEngine* Engine = GetEngine();
	if (!Engine) return;

	Engine->AddOnScreenDebugMessage(0, 5, FColor::Green, FString::Printf(TEXT("Joining %s"), *Address));

	APlayerController* PlayerController = GetFirstLocalPlayerController();
	if (!ensure(PlayerController != nullptr)) return;

	PlayerController->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
}

void UBattlePawnGameInstance::CreateSession(FString& ServerName)
{
	if (OnlineSessionInterface.IsValid())
	{
		//Session setting for session search
		FOnlineSessionSettings OnlineSessionSettings;
		if (IOnlineSubsystem::Get()->GetSubsystemName() == "NULL")
		{
			OnlineSessionSettings.bIsLANMatch = true;
		}
		else
		{
			OnlineSessionSettings.bIsLANMatch = false;
		}
		OnlineSessionSettings.NumPublicConnections = 4;
		OnlineSessionSettings.bShouldAdvertise = true;
		OnlineSessionSettings.bUsesPresence = true;
		OnlineSessionSettings.Set(SERVER_NAME_SETTINGS_KEY, ServerName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);

		OnlineSessionInterface->CreateSession(0, FName(*ServerName), OnlineSessionSettings);
	}
}
