#include "StartMenu.h"

#include "StartMenuSettings.h"
#include "RobotHunter/Network/OnlineManagerSubsystem.h"
#include "RobotHunter/UI/HUD/MainHUD.h"
#include "RobotHunter/Utils/DebugUtils.h"


void UStartMenu::LoadMap()
{
	gi->OnLevelLoad().RemoveDynamic(this, &UStartMenu::LoadMap);

	CALLBACK_LAMBDA_AFTER_TIME(3.2f, 
	const FString LevelName = Cast<AMainCharacter>(GETFPC->GetCharacter())->GetDayManager()->GetCurrentDay() == 3
								  ? settings->GetSector3Name()
								  : settings->GetLevelToTravel();
		                          
	LoadLevel(LevelName);
	);
}

void UStartMenu::Play()
{
	CHECK_NULL(settings, "Level settings is null");

	const FString levelName = settings->GetLevelToTravel();
	if (levelName.IsEmpty())
	{
		LOG_ERROR(true, "Level to travel is null");
		return;
	}

	DisableAllWidgetInteractable();
	gi->OnLevelLoad().
	    AddUniqueDynamic(this, &UStartMenu::LoadMap);

	gi->CallbackOnLevelChange(ETypeChange::HubToGame);
	//LoadMap(); Peut �tre pas une bonne id�e de le call avent le Fade

	//GetWorld()->GetTimerManager().SetTimer(timerLoading, this, &UStartMenu::LoadMap, .1f, false, 1.0f);
}

void UStartMenu::Exit()
{
	hud->SetState(EStateUI::PlayerLobbyUI);
	onQuitMenu.Broadcast();
}

void UStartMenu::Refresh()
{
	Super::Refresh();
	onRefresh.Broadcast();
}

void UStartMenu::HandleInputChanged(bool isGamepad)
{
	Super::HandleInputChanged(isGamepad);
	if (isGamepad)
		playButton->SetFocus();
}

void UStartMenu::LoadLevel(const FString& _levelName)
{
	if (_levelName.IsEmpty())
	{
		LOG_ERROR(true, "Level to travel is null");
		return;
	}
	

	if (UOnlineManagerSubsystem* _manager = GetWorld()->GetGameInstance()->GetSubsystem<UOnlineManagerSubsystem>())
	{
		//_manager->StartServer();
		_manager->LoadMap(_levelName + "?listen");
	}
}

void UStartMenu::RegisterEvents()
{
	BIND_ACTION(playButton, OnClick, UStartMenu, Play);
	BIND_ACTION(backButton, OnClick, UStartMenu, Exit);
}

void UStartMenu::Init()
{
	Super::Init();
	gi = Cast<UCustomGameInstance>(GetWorld()->GetGameInstance());
}

void UStartMenu::DisableAllWidgetInteractable()
{
	widgetToDisabled.Append({playButton, backButton});
	Super::DisableAllWidgetInteractable();
}
