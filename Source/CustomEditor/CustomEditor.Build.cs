using UnrealBuildTool;

public class CustomEditor : ModuleRules
{
	public CustomEditor(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core", "Slate",
			}
		);

		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
				"Json", "EditorStyle"
			}
		);

		if (Target.Type == TargetRules.TargetType.Editor)
		{
			PrivateDependencyModuleNames.AddRange(new string[]
			{
				"EditorFramework",
				"UnrealEd",
				"GameProjectGeneration",
				"MainFrame",
				"LevelEditor",
				"ToolMenus",
				"Blutility",
				"UMGEditor",
				"EditorScriptingUtilities",
			});
		}
	}
}