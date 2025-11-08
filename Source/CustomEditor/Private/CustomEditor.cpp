#include "CustomEditor.h"

#include "SBuildEditorWindow.h"
#include "Misc/Paths.h"
#include "Misc/FeedbackContext.h"
#include "HAL/PlatformProcess.h"

#if !UE_BUILD_SHIPPING
#include "ToolMenus.h"
#include "GameProjectUtils.h"
#include "SourceCodeNavigation.h"
#include "Dialogs/SOutputLogDialog.h"
#include "FileHelpers.h"
#include "UnrealEdGlobals.h"
#include "EditorUtilityWidgetBlueprint.h"
#include "EditorUtilitySubsystem.h"
#include "Styling/CoreStyle.h"
#include "Styling/SlateStyleRegistry.h"
#include "UObject/UObjectGlobals.h"
#include "CustomEditorSettings.h"
#include "Framework/Application/SlateApplication.h"
#include "Async/Async.h"

#endif

#define LOCTEXT_NAMESPACE "CustomEditorModule"

#if !UE_BUILD_SHIPPING
TSharedPtr<FSlateStyleSet> FCustomEditorModule::StyleSet = NULL;

void FCustomEditorModule::StartupModule()
{
	/// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module

	ToolMenus = UToolMenus::Get();
	if (!ToolMenus)
	{
		UE_LOG(LogCustomEditor, Error, TEXT("Failed to get ToolMenus. Disable Tool Editor"));
		return;
	}

	UCustomEditorSettings* Settings = GetMutableDefault<UCustomEditorSettings>();
	Settings->InitSettings();

	CreateEditorMenu();
	CreateCombatMenu();
	CreateOnlineMenu();
	AddToolBarCommands();
}

void FCustomEditorModule::ShutdownModule()
{
	/// This function may be called during shutdown to clean up your module.
	/// For modules that support dynamic reloading, we call this function before unloading the module.
	if (ToolMenus)
		ToolMenus->UnregisterOwner(this);
}

void FCustomEditorModule::EditDefaultSettings(const char* _pathPython)
{
	// Get python executable path
	FString _pathPythonExe = FPaths::RootDir() + PATH_DIRECTORY_PYTHON_EXEC;
	//UE_LOG(LogCustomEditor, Warning, TEXT("python: %s"), *_pathPythonExe);
	if (!FPaths::FileExists(_pathPythonExe))
	{
		UE_LOG(LogCustomEditor, Error, TEXT("Failed to find python exec. Abort rebuild"));
		return;
	}
	// Get python script path
	FString _pathPythonScript = FPaths::ProjectDir() + _pathPython;
	//UE_LOG(LogCustomEditor, Warning, TEXT("script: %s"), *_pathPythonScript);
	if (!FPaths::FileExists(_pathPythonScript))
	{
		UE_LOG(LogCustomEditor, Error, TEXT("Failed to find python script. Abort rebuild"));
		return;
	}

	// Launch python script
	FPlatformProcess::CreateProc(
		*_pathPythonExe,
		*_pathPythonScript,
		true, false, false, nullptr, 0, nullptr, nullptr
	);
}

#pragma region MainMenu
void FCustomEditorModule::CreateEditorMenu()
{
	UToolMenu* MainMenu = ToolMenus->ExtendMenu("LevelEditor.MainMenu");
	FText ShortIDEName = FSourceCodeNavigation::GetSelectedSourceCodeIDE();
	if (ShortIDEName.ToString() == "None")
		ShortIDEName = FText::FromString("IDE");

	/// Add the SubMenu "Editor"
	UToolMenu* EditorMenu = MainMenu->AddSubMenu(
		MainMenu->MenuOwner,
		NAME_None,
		FName("Editor"),
		INVTEXT("Editor"),
		INVTEXT("Editor Options")
	);

	/// Add the section "EDITOR TOOL"
	FToolMenuSection& Section = EditorMenu->AddSection(
		"Editor tool",
		INVTEXT("EDITOR TOOL")
	);

	/// Add the command "Restart"
	Section.AddMenuEntry(
		"RestartCommand",
		INVTEXT("Restart"),
		INVTEXT("Restart the Editor"),
		FSlateIcon(FAppStyle::GetAppStyleSetName(), "Icons.Refresh"),
		FUIAction(FExecuteAction::CreateStatic(&FCustomEditorModule::RestartProject))
	);

	/// Add the command "Rebuild"
	Section.AddMenuEntry(
		"RebuildCommand",
		INVTEXT("Rebuild"),
		FText::Format(
			INVTEXT("Close Editor and {0} Project, delete Intermediate and Binaries folder and relaunch uproject"),
			ShortIDEName),
		FSlateIcon(FName("EditorStyle"), "UMGEditor.DPISettings"),
		FUIAction(FExecuteAction::CreateStatic(&FCustomEditorModule::RebuildProject, PATH_SCRIPT_PYTHON_REBUILD))
	);

	/// Add the command "Super-Rebuild"
	Section.AddMenuEntry(
		"SuperRebuildCommand",
		INVTEXT("SuperRebuild"),
		FText::Format(
			INVTEXT(
				"Close Editor and {0} Project, delete Intermediate, Binaries, .vs, DerivedDataCache and Saved folder, delete .vsconfig and .sln and relaunch uproject"),
			ShortIDEName),
		FSlateIcon(FName("EditorStyle"), "CurveEditor.DeleteTab"),
		FUIAction(FExecuteAction::CreateStatic(&FCustomEditorModule::RebuildProject, PATH_SCRIPT_PYTHON_SUPER_REBUILD))
	);

	/// Add the command "Open Project Solution"
	Section.AddDynamicEntry("CodeProject", FNewToolMenuSectionDelegate::CreateLambda(
		                        [ShortIDEName](FToolMenuSection& InSection)
		                        {
			                        /// Update label, description and icon depending on project's solution 
			                        FText Label = FText();
			                        FText ToolTip = FText();
			                        FSlateIcon Icon = FSlateIcon();

			                        if (FSourceCodeNavigation::DoesModuleSolutionExist())
			                        {
				                        Label = FText::Format(INVTEXT("Refresh {0} Project"), ShortIDEName);
				                        ToolTip = FText::Format(
					                        INVTEXT("Refreshes your C++ code project in {0}."), ShortIDEName);
				                        Icon = FSourceCodeNavigation::GetRefreshSourceCodeIDEIcon();
			                        }
			                        else
			                        {
				                        Label = FText::Format(INVTEXT("Generate {0} Project"), ShortIDEName);
				                        ToolTip = FText::Format(
					                        INVTEXT("Generates your C++ code project in {0}."), ShortIDEName);
				                        Icon = FSourceCodeNavigation::GetOpenSourceCodeIDEIcon();
			                        }

			                        InSection.AddMenuEntry(
				                        "OpenSolutionCommand",
				                        Label,
				                        ToolTip,
				                        Icon,
				                        FUIAction(FExecuteAction::CreateStatic(&FCustomEditorModule::OpenSolution))
			                        );
		                        }));

	/// Add the command "Documentation"
	Section.AddMenuEntry(
		"OpenDocumentationCommand",
		INVTEXT("Documentation"),
		INVTEXT("Open tool documentation"),
		FSlateIcon(FAppStyle::GetAppStyleSetName(), "Icons.Help"),
		FUIAction(FExecuteAction::CreateStatic(&FCustomEditorModule::OpenDocumentation))
	);

	/// Add the command "Clear save"
	Section.AddMenuEntry(
		"Reset Save and game settings",
		INVTEXT("Reset"),
		INVTEXT("Clear All Save Data and game settings\nin project folder and %userprofile%"),
		FSlateIcon(FName("LogVisualizerStyle"), "Toolbar.Remove.small"),
		FUIAction(FExecuteAction::CreateStatic(&FCustomEditorModule::ResetSettingsAndSave))
	);
	
	/// Add the command "Clear save"
	Section.AddMenuEntry(
		"Edit default settings",
		INVTEXT("Default Settings"),
		INVTEXT("Edit the default values of settings of first start game"),
		FSlateIcon(FName("LogVisualizerStyle"), "ToolBar.Icon"),
		FUIAction(FExecuteAction::CreateStatic(&FCustomEditorModule::EditDefaultSettings, PATH_SCRIPT_PYTHON_EDIT_DEFAULT))
	);
}

void FCustomEditorModule::RestartProject()
{
	// Check Editor can restart
	if (!GIsEditor)
	{
		UE_LOG(LogCustomEditor, Error, TEXT("Restart only available from Editor"));
		return;
	}

	// Save all
	const bool bPromptUserToSave = true;
	const bool bSaveMapPackages = true;
	const bool bSaveContentPackages = true;
	const bool bFastSave = false;
	const bool bNotifyNoPackagesSaved = false;
	const bool bCanBeDeclined = false;
	FEditorFileUtils::SaveDirtyPackages(bPromptUserToSave, bSaveMapPackages, bSaveContentPackages, bFastSave,
	                                    bNotifyNoPackagesSaved, bCanBeDeclined);

	// Restart Editor
	FUnrealEdMisc::Get().RestartEditor(false);

	UE_LOG(LogCustomEditor, Display, TEXT("Restart project"));
}

void FCustomEditorModule::RebuildProject(const char* _pathPython)
{
	const FText Message = INVTEXT("Are you sure you want to rebuild the project ?");
	const EAppReturnType::Type Result = FMessageDialog::Open(EAppMsgType::OkCancel, Message);
	if (Result == EAppReturnType::Cancel)
		return;

	// Check Editor can restart
	if (!GIsEditor)
	{
		UE_LOG(LogCustomEditor, Error, TEXT("Restart only available from Editor"));
		return;
	}

	// Save all
	const bool bPromptUserToSave = false;
	const bool bSaveMapPackages = true;
	const bool bSaveContentPackages = true;
	const bool bFastSave = false;
	const bool bNotifyNoPackagesSaved = false;
	const bool bCanBeDeclined = false;
	FEditorFileUtils::SaveDirtyPackages(bPromptUserToSave, bSaveMapPackages, bSaveContentPackages, bFastSave,
	                                    bNotifyNoPackagesSaved, bCanBeDeclined);

	// To close Editor
	FPlatformMisc::RequestExit(false);

	// Get python executable path
	FString _pathPythonExe = FPaths::RootDir() + PATH_DIRECTORY_PYTHON_EXEC;
	//UE_LOG(LogCustomEditor, Warning, TEXT("python: %s"), *_pathPythonExe);
	if (!FPaths::FileExists(_pathPythonExe))
	{
		UE_LOG(LogCustomEditor, Error, TEXT("Failed to find python exec. Abort rebuild"));
		return;
	}
	// Get python script path
	FString _pathPythonScript = FPaths::ProjectDir() + _pathPython;
	//UE_LOG(LogCustomEditor, Warning, TEXT("script: %s"), *_pathPythonScript);
	if (!FPaths::FileExists(_pathPythonScript))
	{
		UE_LOG(LogCustomEditor, Error, TEXT("Failed to find python script. Abort rebuild"));
		return;
	}

	// Launch python script
	FPlatformProcess::CreateProc(
		*_pathPythonExe,
		*_pathPythonScript,
		true, false, false, nullptr, 0, nullptr, nullptr
	);

	UE_LOG(LogCustomEditor, Display, TEXT("Rebuild project"));
}

void FCustomEditorModule::OpenSolution()
{
	// Create/Refresh code project
	if (!FSourceCodeNavigation::IsCompilerAvailable())
	{
		// Attempt to trigger the tutorial if the user doesn't have a compiler installed for the project.
		FSourceCodeNavigation::AccessOnCompilerNotFound().Broadcast();
	}
	FText FailReason, FailLog;
	if (!FGameProjectGenerationModule::Get().UpdateCodeProject(FailReason, FailLog))
		SOutputLogDialog::Open(LOCTEXT("RefreshProject", "Refresh Project"), FailReason, FailLog);

	// Can Open IDE
	const bool bIsCodeProject = IFileManager::Get().DirectoryExists(*FPaths::GameSourceDir());
	if (ensure(GUnrealEd) && bIsCodeProject)
	{
		// Open IDE
		if (!FSourceCodeNavigation::IsCompilerAvailable())
		{
			// Attempt to trigger the tutorial if the user doesn't have a compiler installed for the project.
			FSourceCodeNavigation::AccessOnCompilerNotFound().Broadcast();
		}
		else
		{
			if (!FSourceCodeNavigation::OpenModuleSolution())
			{
				FMessageDialog::Open(EAppMsgType::Ok,
				                     LOCTEXT("OpenIDEFailed_UnableToOpenSolution", "Unable to open solution"));
			}
		}
	}

	UE_LOG(LogCustomEditor, Display, TEXT("Refresh code project"));
}

void FCustomEditorModule::OpenDocumentation()
{
	FString _url = FString(URL_DOCUMENTATION);
	FPlatformProcess::LaunchURL(*_url, NULL, NULL);

	UE_LOG(LogCustomEditor, Display, TEXT("Open documentation module"));
}

void FCustomEditorModule::SendPopup(const FString& _msg)
{
	FMessageDialog::Open(EAppMsgCategory::Success, EAppMsgType::Ok,
	                     FText::FromString(_msg));
}

void FCustomEditorModule::ResetSettingsAndSave()
{
	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
	IFileManager& FileManager = IFileManager::Get();
	int lastChar;

	const FString pathProject = FPaths::GetProjectFilePath();
	pathProject.FindLastChar('/', lastChar);

	FString path = pathProject.Mid(0, lastChar + 1);
	path.Append("Saved/SaveGames/");

	if (PlatformFile.DirectoryExists(*path))
	{
		SendPopup("Successfully deleted folder in '" + path + "'");
		FileManager.DeleteDirectory(*path, false, true);
	}

	FString userPath = FPlatformProcess::UserDir();
	userPath.FindLastChar('/', lastChar);
	path = userPath.Mid(0, lastChar - 10 + 1);
	path.Append("AppData/Local/RobotHunter/Saved/SaveGames/");

	UE_LOG(LogTemp, Warning, TEXT("%s"), *path);

	if (PlatformFile.DirectoryExists(*path))
	{
		SendPopup("Successfully deleted folder in '" + path + "'");
		FileManager.DeleteDirectory(*path, false, true);
	}
}

#pragma endregion

#pragma region Combat
void FCustomEditorModule::CreateCombatMenu()
{
	UToolMenu* MainMenu = ToolMenus->ExtendMenu("LevelEditor.MainMenu");

	/// Add the SubMenu "Combat"
	UToolMenu* EditorMenu = MainMenu->AddSubMenu(
		MainMenu->MenuOwner,
		NAME_None,
		FName("Combat"),
		INVTEXT("Combat"),
		INVTEXT("Combat Options")
	);

	/// Add the section "COMBAT TOOL"
	FToolMenuSection& Section = EditorMenu->AddSection(
		"Combat tool",
		INVTEXT("COMBAT TOOL")
	);

	/// Add the command "Debugger"
	Section.AddMenuEntry(
		"DebuggerCommand",
		INVTEXT("Debugger"),
		INVTEXT("When enabled, allow the combat debugger"),
		FSlateIcon(),
		FUIAction(FExecuteAction::CreateStatic(&FCustomEditorModule::SetDebugger),
		          FCanExecuteAction::CreateStatic(&FCustomEditorModule::CanExecute),
		          FIsActionChecked::CreateStatic(&FCustomEditorModule::SetDebuggerIsChecked)
		),
		EUserInterfaceActionType::ToggleButton
	);
	/// Add the command "AIState"
	Section.AddMenuEntry(
		"AIStateCommand",
		INVTEXT("AI State"),
		INVTEXT("When enabled, display the state of AI"),
		FSlateIcon(),
		FUIAction(FExecuteAction::CreateStatic(&FCustomEditorModule::SetAIState),
		          FCanExecuteAction::CreateStatic(&FCustomEditorModule::CanSet),
		          FIsActionChecked::CreateStatic(&FCustomEditorModule::SetAIStateIsChecked)
		),
		EUserInterfaceActionType::ToggleButton
	);
	/// Add the command "AITarget"
	Section.AddMenuEntry(
		"AITargetCommand",
		INVTEXT("AI Target"),
		INVTEXT("When enabled, display the target of AI"),
		FSlateIcon(),
		FUIAction(FExecuteAction::CreateStatic(&FCustomEditorModule::SetAITarget),
		          FCanExecuteAction::CreateStatic(&FCustomEditorModule::CanSet),
		          FIsActionChecked::CreateStatic(&FCustomEditorModule::SetAITargetIsChecked)
		),
		EUserInterfaceActionType::ToggleButton
	);
	/// Add the command "threatValue"
	Section.AddMenuEntry(
		"threatValueCommand",
		INVTEXT("Threat Value"),
		INVTEXT("When enabled, display the threat value of AI"),
		FSlateIcon(),
		FUIAction(FExecuteAction::CreateStatic(&FCustomEditorModule::SetThreatValue),
		          FCanExecuteAction::CreateStatic(&FCustomEditorModule::CanSet),
		          FIsActionChecked::CreateStatic(&FCustomEditorModule::SetThreatValueIsChecked)
		),
		EUserInterfaceActionType::ToggleButton
	);
	/// Add the command "AIDamage"
	Section.AddMenuEntry(
		"AIDamageCommand",
		INVTEXT("AI Damage"),
		INVTEXT("When enabled, display the damage apply on AI"),
		FSlateIcon(),
		FUIAction(FExecuteAction::CreateStatic(&FCustomEditorModule::SetAIDamage),
		          FCanExecuteAction::CreateStatic(&FCustomEditorModule::CanSet),
		          FIsActionChecked::CreateStatic(&FCustomEditorModule::SetAIDamageIsChecked)
		),
		EUserInterfaceActionType::ToggleButton
	);
}

bool FCustomEditorModule::CanExecute()
{
	return true;
}

bool FCustomEditorModule::CanSet()
{
	return GetMutableDefault<UCustomEditorSettings>()->GetDebugger();
}

void FCustomEditorModule::SetDebugger()
{
	UCustomEditorSettings* Settings = GetMutableDefault<UCustomEditorSettings>();
	Settings->ToggleDebugger();

	UE_LOG(LogCustomEditor, Display, TEXT("Debugger command"));
}

bool FCustomEditorModule::SetDebuggerIsChecked()
{
	return GetMutableDefault<UCustomEditorSettings>()->GetDebugger();
}

void FCustomEditorModule::SetAIState()
{
	UCustomEditorSettings* Settings = GetMutableDefault<UCustomEditorSettings>();
	Settings->ToggleAIState();

	UE_LOG(LogCustomEditor, Display, TEXT("AIState command"));
}

bool FCustomEditorModule::SetAIStateIsChecked()
{
	return GetMutableDefault<UCustomEditorSettings>()->GetAIState();
}

void FCustomEditorModule::SetAITarget()
{
	UCustomEditorSettings* Settings = GetMutableDefault<UCustomEditorSettings>();
	Settings->ToggleAITarget();

	UE_LOG(LogCustomEditor, Display, TEXT("AITarget command"));
}

bool FCustomEditorModule::SetAITargetIsChecked()
{
	return GetMutableDefault<UCustomEditorSettings>()->GetAITarget();
}

void FCustomEditorModule::SetThreatValue()
{
	UCustomEditorSettings* Settings = GetMutableDefault<UCustomEditorSettings>();
	Settings->ToggleThreatValue();

	UE_LOG(LogCustomEditor, Display, TEXT("ThreatValue command"));
}

bool FCustomEditorModule::SetThreatValueIsChecked()
{
	return GetMutableDefault<UCustomEditorSettings>()->GetThreatValue();
}

void FCustomEditorModule::SetAIDamage()
{
	UCustomEditorSettings* Settings = GetMutableDefault<UCustomEditorSettings>();
	Settings->ToggleAIDamage();

	UE_LOG(LogCustomEditor, Display, TEXT("AIDamage command"));
}

bool FCustomEditorModule::SetAIDamageIsChecked()
{
	return GetMutableDefault<UCustomEditorSettings>()->GetAIDamage();
}
#pragma endregion

#pragma region Online
void FCustomEditorModule::CreateOnlineMenu()
{
	UToolMenu* MainMenu = ToolMenus->ExtendMenu("LevelEditor.MainMenu");

	/// Add the SubMenu "Online"
	UToolMenu* EditorMenu = MainMenu->AddSubMenu(
		MainMenu->MenuOwner,
		NAME_None,
		FName("Online"),
		INVTEXT("Online"),
		INVTEXT("Online Options")
	);

	/// Add the section "Online TOOL"
	FToolMenuSection& Section = EditorMenu->AddSection(
		"Online tool",
		INVTEXT("ONLINE TOOL")
	);

	/// Add the command "SeamLess"
	Section.AddMenuEntry(
		"SeamlessCommand",
		INVTEXT("Seamless"),
		INVTEXT("When enabled the seamless travel in LobbyGameMode and PrincipaleGameMode"),
		FSlateIcon(),
		FUIAction(FExecuteAction::CreateStatic(&FCustomEditorModule::EnableSeamless),
		          FCanExecuteAction::CreateStatic(&FCustomEditorModule::CanExecute),
		          FIsActionChecked::CreateStatic(&FCustomEditorModule::SetEnableSeamlessIsChecked)
		),
		EUserInterfaceActionType::ToggleButton
	);
}

void FCustomEditorModule::EnableSeamless()
{
	UCustomEditorSettings* Settings = GetMutableDefault<UCustomEditorSettings>();
	Settings->ToggleSeamless();

	UE_LOG(LogCustomEditor, Display, TEXT("Seamless command"));
}

bool FCustomEditorModule::SetEnableSeamlessIsChecked()
{
	return GetMutableDefault<UCustomEditorSettings>()->GetSeamless();
}
#pragma endregion

void FCustomEditorModule::AddToolBarCommands()
{
	// Add button on ToolBar 
	UToolMenu* ToolbarMenu = ToolMenus->ExtendMenu("LevelEditor.LevelEditorToolBar.User");
	FToolMenuSection& ToolbarSection = ToolbarMenu->FindOrAddSection("Custom");

	// Register custom icon
	StyleSet = MakeShareable(new FSlateStyleSet("CustomStyle"));
	// Set paths to content folders to support loading assets that ship with the engine
	StyleSet->SetCoreContentRoot(FPaths::EngineContentDir() / TEXT("Slate"));
	// Register some brushes
	const FVector2D Icon64x64(64.0f, 64.0f);
	FString _path = FPaths::ProjectDir() / TEXT("Source/CustomEditor/Content/IconProg.png");
	StyleSet->Set("Custom.IconProg", new FSlateImageBrush(_path, Icon64x64));
	FSlateStyleRegistry::RegisterSlateStyle(*StyleSet.Get());

	// Add Tech menu on toolbar
	FToolMenuEntry TechMenu = FToolMenuEntry::InitComboButton(
		FName("Tech"),
		FToolUIActionChoice(),
		FOnGetContent::CreateRaw(this, &FCustomEditorModule::GenerateTechMenuWidget),
		INVTEXT("Tech"),
		INVTEXT("Tech options"),
		FSlateIcon(FName("ControlRigEditorStyle"), "ControlRig.ConstructionMode")
	);
	TechMenu.StyleNameOverride = "CalloutToolbar";
	ToolbarSection.AddEntry(TechMenu);

	// Add Prog menu on toolbar
	FToolMenuEntry ProgMenu = FToolMenuEntry::InitComboButton(
		FName("Prog"),
		FToolUIActionChoice(),
		FOnGetContent::CreateRaw(this, &FCustomEditorModule::GenerateProgMenuWidget),
		INVTEXT("Prog"),
		INVTEXT("Prog options"),
		FSlateIcon(FName("EditorStyle"), "SessionFrontEnd.Tabs.Console")
	);
	ProgMenu.StyleNameOverride = "CalloutToolbar";
	ToolbarSection.AddEntry(ProgMenu);
}

#pragma region Tech
TSharedRef<SWidget> FCustomEditorModule::GenerateTechMenuWidget()
{
	if (ToolMenus->IsMenuRegistered("LevelEditor.LevelEditorToolBar.TechMenu"))
		return ToolMenus->GenerateWidget("LevelEditor.LevelEditorToolBar.TechMenu", nullptr);

	UToolMenu* TechMenu = ToolMenus->RegisterMenu("LevelEditor.LevelEditorToolBar.TechMenu");
	{
		// Add section "EditorUtilityWidget"
		FToolMenuSection& EUWSection = TechMenu->AddSection("EUW", INVTEXT("EUW"));
		// Add the command "Selector EUW"
		EUWSection.AddMenuEntry(
			"SelectorEUWCommand",
			INVTEXT("Selector"),
			INVTEXT("The EUW Selector"),
			FSlateIcon(FName("EditorStyle"), "Icons.SelectInViewport"),
			FUIAction(FExecuteAction::CreateStatic(&FCustomEditorModule::RunEUW,
			                                       FString(
				                                       "/Script/Blutility.EditorUtilityWidgetBlueprint'/Game/06_Tech/EUW/EUW_Selector.EUW_Selector'")))
		);
		// Add the command "Stats EUW"
		EUWSection.AddMenuEntry(
			"StatsEUWCommand",
			INVTEXT("Stats"),
			INVTEXT("The EUW Stats"),
			FSlateIcon(FName("EditorStyle"), "EditorViewport.ToggleStats"),
			FUIAction(FExecuteAction::CreateStatic(&FCustomEditorModule::RunEUW,
			                                       FString(
				                                       "/Script/Blutility.EditorUtilityWidgetBlueprint'/Game/06_Tech/EUW/EUW_Stats_Shortcuts.EUW_Stats_Shortcuts'")))
		);

		// Add section "Weather"
		FToolMenuSection& WeatherSection = TechMenu->AddSection("WEATHER", INVTEXT("WEATHER"));
		// Add the command "Loader EUW"
		WeatherSection.AddMenuEntry(
			"LoaderEUWCommand",
			INVTEXT("Weather Loader"),
			INVTEXT("The EUW Weather loader"),
			FSlateIcon(FName("EditorStyle"), "ClassIcon.VolumetricCloud"),
			FUIAction(FExecuteAction::CreateStatic(&FCustomEditorModule::RunEUW,
			                                       FString(
				                                       "/Script/Blutility.EditorUtilityWidgetBlueprint'/Game/06_Tech/Weather/EUW/EUW_Weather_Loader.EUW_Weather_Loader'")))
		);
	}

	return ToolMenus->GenerateWidget(TechMenu);
}

TSharedRef<SWidget> FCustomEditorModule::GenerateProgMenuWidget()
{
	if (ToolMenus->IsMenuRegistered("LevelEditor.LevelEditorToolBar.ProgMenu"))
		return ToolMenus->GenerateWidget("LevelEditor.LevelEditorToolBar.ProgMenu", nullptr);

	UToolMenu* ProgMenu = ToolMenus->RegisterMenu("LevelEditor.LevelEditorToolBar.ProgMenu");
	{
		// Add section "Tool"
		FToolMenuSection& ToolSection = ProgMenu->AddSection("TOOL", INVTEXT("TOOL"));
		// Add the command "Movable Data EUW"
		ToolSection.AddMenuEntry(
			"MovableDataCommand",
			INVTEXT("Movable Data Modifier"),
			INVTEXT("The EUW Movable Data Modifier"),
			FSlateIcon(FName("EditorStyle"), "Icons.Transform"),
			FUIAction(FExecuteAction::CreateStatic(&FCustomEditorModule::RunEUW,
			                                       FString(
				                                       "/Script/Blutility.EditorUtilityWidgetBlueprint'/Game/00_Prog/Tool/MovableDataModifier/T_MovableDataModifier.T_MovableDataModifier'")))
		);
		// Add the command "Rail Link EUW"
		ToolSection.AddMenuEntry(
			"RailLinkCommand",
			INVTEXT("Rail link"),
			INVTEXT("The EUW Rail link - WARNING : use it only if you didn't modify RailLinkUtilityWidget"),
			FSlateIcon(FName("CoreStyle"), "Icons.Link"),
			FUIAction(FExecuteAction::CreateStatic(&FCustomEditorModule::RunEUW,
			                                       FString(
				                                       "/Script/Blutility.EditorUtilityWidgetBlueprint'/Game/00_Prog/Tool/Rail/BP_RailLinkUtiliyWidget.BP_RailLinkUtiliyWidget'")))
		);
		// Add the command "Tree component EUW"
		ToolSection.AddMenuEntry(
			"TreeComponentCommand",
			INVTEXT("Tree Component Modifier"),
			INVTEXT("The EUW Tree Component"),
			FSlateIcon(FName("EditorStyle"), "StructViewer.TabIcon"),
			FUIAction(FExecuteAction::CreateStatic(&FCustomEditorModule::RunEUW,
			                                       FString(
				                                       "/Script/Blutility.EditorUtilityWidgetBlueprint'/Game/00_Prog/Tool/TreeComponent/T_TreeComponent.T_TreeComponent'")))
		);

		ToolSection.AddMenuEntry(
			"CompilProject",
			INVTEXT("Package project"),
			INVTEXT("Package project"),
			FSlateIcon(FName("EditorStyle"), "MainFrame.PackageProject"),
			FUIAction(FExecuteAction::CreateRaw(this, &FCustomEditorModule::PackageProject))
		);
	}

	return ToolMenus->GenerateWidget(ProgMenu);
}

void FCustomEditorModule::PackageProject()
{
	TSharedRef<SWindow> MyWindow = SNew(SWindow)
		.Title(FText::FromString("Custom Package Window"))
		.ClientSize(FVector2D(700, 300))
		[
			SNew(SBuildEditorWindow)
		];

	// Affiche la fenêtre
	FSlateApplication::Get().AddWindow(MyWindow);
	/*
	// Chemin vers le projet et l'exécutable de l'éditeur
	FString ProjectPath = FPaths::ConvertRelativePathToFull(FPaths::GetProjectFilePath());

	// Chemin vers Unreal Automation Tool (UAT)
	FString UATPath = FPaths::Combine(FPaths::EngineDir(), TEXT("Build"), TEXT("BatchFiles"), TEXT("RunUAT.bat"));

	// Définir la plateforme cible et la configuration
	FString PlatformTarget = TEXT("Win64");
	FString BuildConfiguration = TEXT("Development");

	// Préparer la commande à exécuter
	FString Command = FString::Printf(
		TEXT(
			"BuildCookRun -project=\"%s\" -noP4 -clientconfig=%s -serverconfig=%s -platform=%s -cook -allmaps -build -stage -package"),
		*ProjectPath,
		*BuildConfiguration,
		*BuildConfiguration,
		*PlatformTarget
	);

	UE_LOG(LogTemp, Log, TEXT("Packaging started in async task..."));

	// Lancer une tâche asynchrone
	Async(EAsyncExecution::Thread, [UATPath, Command]
	{
		int32 ReturnCode = 0;
		FString Output;
		FString ErrorOutput;

		// Exécuter la commande via FPlatformProcess
		FPlatformProcess::ExecProcess(*UATPath, *Command, &ReturnCode, &Output, &ErrorOutput);

		if (ReturnCode == 0)
		{
			UE_LOG(LogTemp, Log, TEXT("Packaging completed successfully!"));
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Packaging failed: %s"), *ErrorOutput);
		}
	});*/
}


void FCustomEditorModule::RunEUW(FString _EUWReference)
{
	UBlueprint* UMGBP = LoadObject<UBlueprint>(nullptr, *_EUWReference);
	UEditorUtilityWidgetBlueprint* EditorWidget = Cast<UEditorUtilityWidgetBlueprint>(UMGBP);
	if (EditorWidget)
	{
		UEditorUtilitySubsystem* EditorUtilitySubsystem = GEditor->GetEditorSubsystem<UEditorUtilitySubsystem>();
		EditorUtilitySubsystem->SpawnAndRegisterTab(EditorWidget);
	}

	UE_LOG(LogCustomEditor, Display, TEXT("Run EUW"));
}
#pragma endregion
#endif

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FCustomEditorModule, CustomEditor)
