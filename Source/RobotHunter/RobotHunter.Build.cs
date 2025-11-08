// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;

public class RobotHunter : ModuleRules
{
	public RobotHunter(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new[]
		{
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"InputDevice",
			"EnhancedInput",
			"UMG",
			"GameplayStateTreeModule",
			"AIModule",
			"StateTreeModule",
			"NavigationSystem",
			"Niagara",
			"AdvancedWidgets",
			"ControlRig",
			"RigVM",
			"MediaAssets",
			"LevelSequence"
		});

		if (Target.bBuildEditor)
		{
			PublicDependencyModuleNames.AddRange(new[]
			{
				"UnrealEd",
				"ToolMenus",
				"Blutility",
				"CustomEditor"
			});
		}

		// Uncomment if you are using Slate UI
		PrivateDependencyModuleNames.AddRange(new[]
			{ "Slate", "SlateCore", "CommonInput", "CommonUI" });

		// Uncomment if you are using online features
		PrivateDependencyModuleNames.AddRange(new[] { "OnlineSubsystem", "OnlineSubsystemSteam" });

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}