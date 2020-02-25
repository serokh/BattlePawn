// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MenuInterface.h"
#include "MenuWidget.h"


#include "MainMenu.generated.h"

USTRUCT()
struct FServerData
{

	GENERATED_BODY()

	FString ServerName;
	uint16 CurrentPlayers;
	uint16 MaxPlayers;
	FString HostUsername;

};
/**
 *
 */
UCLASS()
class BATTLEPAWN_API UMainMenu : public UMenuWidget
{
	GENERATED_BODY()

public:
	UMainMenu(const FObjectInitializer& ObjectInitializer);

	void SetServerList(TArray<FServerData> ServerData);

	void SelectIndex(uint32 Index);

protected:
	virtual void OnLevelRemovedFromWorld(ULevel* InLevel, UWorld* InWorld) override;

	virtual bool Initialize() override;

private:
	TSubclassOf<class UUserWidget> ServerRowClass;

///////////////////////////////////////////////////////
/////////////////Main Menu

	UPROPERTY(meta = (BindWidget))
	class UWidget* MainMenu;

	UPROPERTY(meta = (BindWidget))
	class UButton* HostButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* JoinButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* QuitGameButton;

///////////////////////////////////////////////////////
/////////////////Join Menu

	UPROPERTY(meta = (BindWidget))
	class UWidget* JoinMenu;
	
	UPROPERTY(meta = (BindWidget))
	class UScrollBox* ServerList;

	UPROPERTY(meta = (BindWidget))
	class UButton* CancelButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* JoinConfirmButton;

	UPROPERTY(meta = (BindWidget))
	class UWidgetSwitcher* MenuSwitcher;

///////////////////////////////////////////////////////
/////////////////Host Menu

	UPROPERTY(meta = (BindWidget))
	class UWidget* HostMenu;

	UPROPERTY(meta = (BindWidget))
	class UButton* CancelHostButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* ConfirmHostButton;

	UPROPERTY(meta = (BindWidget))
	class UEditableTextBox* InputServerName;


///////////////////////////////////////////////////////
/////////////////Menu Functions

	UFUNCTION()
	void HostServer();

	UFUNCTION()
	void JoinServer();

	UFUNCTION()
	void OpenJoinMenu();

	UFUNCTION()
	void OpenMainMenu();

	UFUNCTION()
	void OpenHostMenu();

	UFUNCTION()
	void QuitGame();

	// Server to connect
	TOptional<uint32> SelectedIndex;

	void UpdateServerNames();

};
