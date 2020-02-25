// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"

#include "Interfaces/OnlineSessionInterface.h"

#include "MenuSystem/MenuInterface.h"

#include "BattlePawnGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class BATTLEPAWN_API UBattlePawnGameInstance : public UGameInstance, public IMenuInterface
{
	GENERATED_BODY()

public:
	UBattlePawnGameInstance(const FObjectInitializer& ObjectInitializer);

	virtual void Init();

	UFUNCTION(BlueprintCallable)
	void LoadMenu();

	UFUNCTION(BlueprintCallable)
	void LoadChangeClassMenu();

	UFUNCTION(BlueprintCallable)
	void LoadInGameMenu();

	UFUNCTION(Exec)
	void Host(FString& ServerName) override;

	UFUNCTION(Exec)
	void Join(uint32 Index) override;

	void QuitToMenu() override;

	void RefreshServerList() override;

	void StartSession();
private:

	TSubclassOf<class UUserWidget> MenuClass;
	TSubclassOf<class UUserWidget> InGameMenuClass;
	TSubclassOf<class UUserWidget> ChangeClassMenuClass;

	class UMenuWidget* Menu;

	IOnlineSessionPtr OnlineSessionInterface;
	TSharedPtr<class FOnlineSessionSearch> SessionSearch;

	void OnCreateSessionComplete(FName SessionName, bool Success);

	void OnDestroySessionComplete(FName SessionName, bool Success);

	void OnFindSessionsComplete(bool Success);

	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);

	void CreateSession(FString& ServerName);
};
