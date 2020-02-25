// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameStatsInfo.generated.h"

/**
 * 
 */
UCLASS()
class BATTLEPAWN_API UGameStatsInfo : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UGameStatsInfo(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable)
	void SetPlayerScoreList();

	
private:
	
	UPROPERTY(meta = (BindWidget))
	class UScrollBox* PlayerScoreList;

	TSubclassOf<class UUserWidget> PlayerInfoRowClass;
};
