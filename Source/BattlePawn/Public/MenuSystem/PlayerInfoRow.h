// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerInfoRow.generated.h"

/**
 * 
 */
UCLASS()
class BATTLEPAWN_API UPlayerInfoRow : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UPROPERTY(meta = (BindWidget))
	class UEditableTextBox* PlayerName;

	UPROPERTY(meta = (BindWidget))
	class UEditableTextBox* Score;
};
