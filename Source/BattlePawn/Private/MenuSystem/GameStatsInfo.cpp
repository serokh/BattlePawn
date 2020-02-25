// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/ScrollBox.h"
#include "Components/EditableTextBox.h"
#include "UObject/ConstructorHelpers.h"

#include "Online/BattlePawnGameState.h"
#include "BattlePawnTypes.h"
#include "MenuSystem/PlayerInfoRow.h"


#include "MenuSystem/GameStatsInfo.h"

UGameStatsInfo::UGameStatsInfo(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	ConstructorHelpers::FClassFinder<UUserWidget> PlayerInfoRowBPClass(TEXT("/Game/Core/Menu/WBP_StatRow"));
	if (PlayerInfoRowBPClass.Class)
	{
		PlayerInfoRowClass = PlayerInfoRowBPClass.Class;
	}
}

void UGameStatsInfo::SetPlayerScoreList()
{
	auto World = this->GetWorld();
	auto GameState = Cast<ABattlePawnGameState>(World->GetGameState());

	if (World && PlayerScoreList && GameState)
	{
		TArray<FPlayerStateInfo> RankedPlayerList;
		GameState->GetGameScore(RankedPlayerList);

		PlayerScoreList->ClearChildren();

		for (const auto& Player : RankedPlayerList)
		{
			UPlayerInfoRow* Row = CreateWidget<UPlayerInfoRow>(World, PlayerInfoRowClass);
			if (Row)
			{
				Row->PlayerName->SetText(FText::FromString(Player.PlayerName));
				FString PlayerScore;
				if (Player.bIsSpectator)
				{
					PlayerScore = FString::Printf(TEXT("SPEC"), Player.NumKills, Player.NumDeaths);
				}
				else
				{
					PlayerScore = FString::Printf(TEXT("%d/%d"), Player.NumKills, Player.NumDeaths);
				}
				
				Row->Score->SetText(FText::FromString(PlayerScore));
				
				PlayerScoreList->AddChild(Row);
			}
		}
	}
}