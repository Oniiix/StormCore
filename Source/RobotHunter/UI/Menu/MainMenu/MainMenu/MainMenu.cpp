#include "MainMenu.h"


#include "RobotHunter/UI/HUD/MainHUD.h"
#include "RobotHunter/Utils/DebugUtils.h"
#include "RobotHunter/Network/OnlineManagerSubsystem.h"
#include <RobotHunter/InputSystem/InputSubsystem.h>

void UMainMenu::RegisterEvents()
{
	Super::RegisterEvents();
	BIND_ACTION(solo, OnClick, UMainMenu, ShowSoloMenu);
	BIND_ACTION(options, OnClick, UMainMenu, ShowOptionsMenu);
	BIND_ACTION(credit, OnClick, UMainMenu, ShowCreditsMenu);
	BIND_ACTION(quit, OnClick, UMainMenu, QuitGame);
	//BIND_ACTION(GetGameInstance()->GetSubsystem<UInputSubsystem>(), OnInputControllerChange, UMainMenu,
	//            HandleInputChanged);
}

void UMainMenu::Refresh()
{
	Super::Refresh();
	//if (GetGameInstance()->GetSubsystem<UInputSubsystem>()->IsGamepad())
	//	MainButton->SetFocus();
}
/*
void UMainMenu::HandleInputChanged(bool isGamepad)
{
	Super::HandleInputChanged(isGamepad);
	if (isGamepad && currentButtonSelected)	
		currentButtonSelected->SetFocus();	
}*/

void UMainMenu::ShowHostMenu()
{
	GetWorld()->GetGameInstance<UCustomGameInstance>()->SetOnlineMode(true);
	hud->SetState(EStateUI::MainHost);
}

void UMainMenu::ShowJoinMenu()
{
	GetWorld()->GetGameInstance<UCustomGameInstance>()->SetOnlineMode(true);
	hud->SetState(EStateUI::MainServer);
}

void UMainMenu::ShowOptionsMenu()
{
	hud->SetState(EStateUI::MainOptions);
}

void UMainMenu::ShowCreditsMenu()
{
	//checkf(HUD, TEXT("HUD is nullptr in %s"), __func__);
	hud->SetState(EStateUI::MainCredit);
}

void UMainMenu::QuitGameHandle()
{
	UKismetSystemLibrary::QuitGame(this, GetWorld()->GetFirstPlayerController(), EQuitPreference::Quit, true);
}

void UMainMenu::QuitGame()
{
	UCustomGameInstance* GameInstance = Cast<UCustomGameInstance>(GetWorld()->GetGameInstance());
	GameInstance->CallbackOnLevelChange(ETypeChange::Quit);
	GameInstance->OnLevelLoad().AddUniqueDynamic(this, &UMainMenu::QuitGameHandle);
}

void UMainMenu::ShowSoloMenu()
{
	GetWorld()->GetGameInstance<UCustomGameInstance>()->SetOnlineMode(false);
	hud->SetState(EStateUI::MainHost);
}

void UMainMenu::Init()
{
	Super::Init();
	UpDownButtonOrder =	{ solo, options, credit, quit };

	//UOnlineManagerSubsystem* _oms = GetGameInstance()->GetSubsystem<UOnlineManagerSubsystem>();
	//if (_oms && _oms->IsInSession())
	//{
	//	_oms->QuitServer();
	//}
}
