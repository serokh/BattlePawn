// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "BattlePawnGameMode.h"
#include "BattlePawnHUD.h"
#include "BattlePawnCharacter.h"
#include "UObject/ConstructorHelpers.h"

ABattlePawnGameMode::ABattlePawnGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/Characters/BP_Shooter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = ABattlePawnHUD::StaticClass();
}
