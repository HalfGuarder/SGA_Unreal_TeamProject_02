// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class TFT_Project_B : ModuleRules
{
	public TFT_Project_B(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "AIModule", "LevelSequence", "NavigationSystem", "UMG", "Niagara", "MovieScene"  });

		PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

        PublicIncludePaths.AddRange(new string[] {
            "TFT_Project_B",
            "TFT_Project_B/TFT_Components",
            "TFT_Project_B/TFT_AnimInstances",
            "TFT_Project_B/TFT_Characters",
            "TFT_Project_B/TFT_Controllers",
            "TFT_Project_B/TFT_Managers",
            "TFT_Project_B/TFT_Widgets",
            "TFT_Project_B/TFT_FXs",
            "TFT_Project_B/TFT_AI"
        });

        // Uncomment if you are using Slate UI
        // PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

        // Uncomment if you are using online features
        // PrivateDependencyModuleNames.Add("OnlineSubsystem");

        // To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
    }
}
