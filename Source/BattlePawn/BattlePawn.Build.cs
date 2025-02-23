// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class BattlePawn : ModuleRules
{
	public BattlePawn(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        //PrivateIncludePaths.AddRange(
        //    new string[] {
        //        "BattlePawn/Private"
        //    }
        //);

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "UMG", "OnlineSubsystem"});

        //if (Target.bBuildDeveloperTools || (Target.Configuration != UnrealTargetConfiguration.Shipping && Target.Configuration != UnrealTargetConfiguration.Test))
        //{
        //    PrivateDependencyModuleNames.Add("GameplayDebugger");
        //    PublicDefinitions.Add("WITH_GAMEPLAY_DEBUGGER=1");
        //}
        //else
        //{
        //    PublicDefinitions.Add("WITH_GAMEPLAY_DEBUGGER=0");
        //}
    }
}
