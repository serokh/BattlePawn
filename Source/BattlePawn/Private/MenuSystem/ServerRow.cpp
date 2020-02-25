// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Button.h"

#include "MenuSystem/MainMenu.h"

#include "MenuSystem/ServerRow.h"

void UServerRow::Setup(class UMainMenu* InParent, uint32 InIndex)
{
	Parent = InParent;
	Index = InIndex;
	RowButton->OnClicked.AddDynamic(this, &UServerRow::OnClicked);
}

void UServerRow::OnClicked()
{
	Parent->SelectIndex(Index);
}