#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

DECLARE_LOG_CATEGORY_EXTERN(LogCustomEditor, All, All) /// Logging category for all files including .h
DEFINE_LOG_CATEGORY(LogCustomEditor)

#define PATH_DIRECTORY_PYTHON_EXEC "Engine/Binaries/ThirdParty/Python3/Win64/python.exe"
#define PATH_SCRIPT_PYTHON_REBUILD "Source/CustomEditor/Python/rebuild_project.py"
#define PATH_SCRIPT_PYTHON_SUPER_REBUILD "Source/CustomEditor/Python/super_rebuild_project.py"
#define PATH_SCRIPT_PYTHON_EDIT_DEFAULT "Source/CustomEditor/Python/default_value_settings.py"
#define URL_DOCUMENTATION "https://github.com/GabrielR26/Module_CustomEditor"

class FCustomEditorModule : public IModuleInterface
{
#if !UE_BUILD_SHIPPING /// Use preprocess to allow shipping
	UToolMenus* ToolMenus = nullptr;
	static TSharedPtr<class FSlateStyleSet> StyleSet;

public:
	void StartupModule() override;
	void ShutdownModule() override;

private:
#pragma region Editor

	static void EditDefaultSettings(const char* _pathPython);
	/// <summary>
	/// Add the "Editor" menu on Editor's MainMenu 
	/// </summary>
	void CreateEditorMenu();
	/// <summary>
	/// Use Unreal's methods body to restart Editor
	/// </summary>
	static void RestartProject();
	/// <summary>
	/// Use Unreal's methods body and python script to rebuild project
	/// </summary>
	static void RebuildProject(const char* _pathPython);
	/// <summary>
	/// Use Unreal's methods body to generate/refresh and open project's solution with actual IDE
	/// </summary>
	static void OpenSolution();
	/// <summary>
	/// Use Unreal's methods body to open the tool's documentation web page 
	/// </summary>
	static void OpenDocumentation();
	static void SendPopup(const FString& _msg);

	/**
	 * Clear all save in editor and folder shipping
	 */
	static void ResetSettingsAndSave();

	/**
	 * Get all path file in directory
	 * @param directory Directory to search
	 * @param fullPath if get full path
	 * @param onlyFilesStartingWith Start With string
	 * @param onlyFilesWithExtension Extension of file to find
	 * @return An array of path of file search
	 */
	static TArray<FString> GetAllFilesInDirectory(const FString& directory, const bool fullPath,
	                                              const FString& onlyFilesStartingWith,
	                                              const FString& onlyFilesWithExtension);
#pragma endregion

#pragma region Combat
	/// <summary>
	/// Add the "Combat" menu on Editor's MainMenu 
	/// </summary>
	void CreateCombatMenu();
	static bool CanExecute();
	static bool CanSet();

	static void SetDebugger();
	static bool SetDebuggerIsChecked();
	static void SetAIState();
	static bool SetAIStateIsChecked();
	static void SetAITarget();
	static bool SetAITargetIsChecked();
	static void SetThreatValue();
	static bool SetThreatValueIsChecked();
	static void SetAIDamage();
	static bool SetAIDamageIsChecked();
#pragma endregion

#pragma region Online
	void CreateOnlineMenu();
	static void EnableSeamless();
	static bool SetEnableSeamlessIsChecked();
#pragma endregion

#pragma region Tech
	/// <summary>
	/// Add tool commands on Editor's ToolBar 
	/// </summary>
	void AddToolBarCommands();

	/// <summary>
	/// Add tech submenu on Editor's ToolBar
	/// </summary>
	/// <returns> The widget to display </returns>
	TSharedRef<SWidget> GenerateTechMenuWidget();
	/// <summary>
	/// Add Prog submenu on Editor's ToolBar
	/// </summary>
	/// <returns> The widget to display </returns>
	TSharedRef<SWidget> GenerateProgMenuWidget();

	void PackageProject();
	void CompileFtext();

	/// <summary>
	/// Run an EditorUtilityWidget
	/// </summary>
	static void RunEUW(FString _EUWReference);
#pragma endregion
#endif
};
