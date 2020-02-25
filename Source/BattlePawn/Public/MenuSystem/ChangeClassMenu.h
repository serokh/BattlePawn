// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MenuSystem/MenuWidget.h"
#include "ChangeClassMenu.generated.h"

/**
 * 
 */
UCLASS()
class BATTLEPAWN_API UChangeClassMenu : public UMenuWidget
{
	GENERATED_BODY()
	
protected:
	virtual bool Initialize() override;

private:

	UPROPERTY(meta = (BindWidget))
	class UButton* PlayerButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* SpectatorButton;

	UFUNCTION()
	void PlayerButtonPressed();

	UFUNCTION()
	void SpectatorButtonPressed();
	
	UFUNCTION(Server, Reliable)
	void ServerCallForSpawn(bool Spectating);
};
