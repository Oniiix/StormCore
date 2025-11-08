#include "HostMenu.h"

#include "HostSettings.h"
#include "Components/EditableTextBox.h"
#include "Components/ScrollBox.h"
#include "Components/ScrollBoxSlot.h"
#include "GameFramework/PlayerState.h"
#include "RobotHunter/InputSystem/InputSubsystem.h"
#include "RobotHunter/Network/OnlineManagerSubsystem.h"
#include "RobotHunter/SaveSystem/GameplaySave.h"
#include "RobotHunter/SaveSystem/SaveManagerSubsystem.h"
#include "RobotHunter/UI/HUD/Settings/MenuSettings.h"
#include "RobotHunter/UI/HUD/MainHUD.h"
#include "RobotHunter/UI/Template/MainMenu/Host/ButtonSaveTemplate.h"
#include "RobotHunter/Utils/DebugUtils.h"

void UHostMenu::Init()
{
	Super::Init();
	saveManager = GetWorld()->GetGameInstance()->GetSubsystem<USaveManagerSubsystem>();
	GameInstance = GetWorld()->GetGameInstance<UCustomGameInstance>();

	addButton = CreateWidget<UButtonPrefab>(this, settings->GetTemplateCreateSave());
	CHECK_NULL(addButton, "Create save button is null");
	
}

void UHostMenu::Launch(int SaveSlot)
{
	DisableAllWidgetInteractable();
	saveManager->SetSelectedSave(SaveSlot);

	UGameplaySave* _save = saveManager->GetCurrentSave();
	//saveManager->GetCurrentSave()->SaveTime();
	GameInstance->SetInTutorial(_save->GetGameSave().isTutorial);
	saveManager->GetCurrentSave()->SetIsTutorial(_save->GetGameSave().isTutorial);

	if (_save->GetGameSave().isTutorial)
		GameInstance->OnLevelLoad().AddUniqueDynamic(this, &UHostMenu::LaunchMapTutorial);
	else
		GameInstance->OnLevelLoad().AddUniqueDynamic(this, &UHostMenu::LaunchMap);
	
	GameInstance->CallbackOnLevelChange(!_save->GetGameSave().isTutorial
		                                    ? ETypeChange::MainToHub
		                                    : ETypeChange::MainToIntro);	

	saveManager->SaveCurrent();
}

void UHostMenu::RegisterEvents()
{
	Super::RegisterEvents();
	BIND_ACTION(backButton, OnClick, UHostMenu, Back);
	BIND_ACTION(addButton, OnClick, UHostMenu, CreateSaveGame);

	onPlay.AddUniqueDynamic(this, &UHostMenu::Launch);
}

/*
void UHostMenu::HandleInputChanged(bool isGamepad)
{
	Super::HandleInputChanged(isGamepad);
	if (!isGamepad) return;
	
	if (UWidget* firstChild = saveList->GetScrollbox()->GetChildAt(0))
		firstChild->SetFocus();
}*/


void UHostMenu::LaunchMap()
{
	GameInstance->OnLevelLoad().RemoveDynamic(this, &UHostMenu::LaunchMap);

	//=========Pour le ONLINE==============

	/*const EOnlineAccessibilityState _state =
		GameInstance->GetOnlineMode()
			? static_cast<EOnlineAccessibilityState>(SessionSelector->GetCurrentIndex())
			: Private;*/

	//GetWorld()->GetGameInstance()->GetSubsystem<UOnlineManagerSubsystem>()->CreateServer(
	//	GetWorld()->GetFirstPlayerController()->PlayerState->GetPlayerName(), _state);
	
	//=====================================

	UGameplayStatics::OpenLevel(GetWorld(), FName(GameInstance->GetHubLevel().GetAssetName()));
}

void UHostMenu::Refresh()
{
	Super::Refresh();
	if (GetGameInstance()->GetSubsystem<UInputSubsystem>()->IsGamepad())
		if (UWidget* FirstChild = saveList->GetScrollbox()->GetChildAt(0))
			FirstChild->SetFocus();
	
	LoadSaves();

	OverlaySelector->SetVisibility(GameInstance->GetOnlineMode()
									   ? ESlateVisibility::Visible
									   : ESlateVisibility::Hidden);

	UpDownButtonOrder.Empty();
	LeftRightButtonOrder.Empty();
	for (UWidget* AllChild : saveList->GetScrollbox()->GetAllChildren())
	{
		if (INSTANCEOF(AllChild, UButtonPrefab))		
			UpDownButtonOrder.Add(Cast<UButtonPrefab>(AllChild));
	}
	LeftRightButtonOrder.Add(Cast<UButtonPrefab>(saveList->GetScrollbox()->GetChildAt(0)));
	UpDownButtonOrder.Add(backButton);
	LeftRightButtonOrder.Add(backButton);
}

void UHostMenu::Create(const int _slot)
{
	creatingSlot = _slot;
	CallPopup();
}

void UHostMenu::Back()
{
	hud->SetState(EStateUI::MainMain);
}

void UHostMenu::LoadSaves()
{
	CHECK_NULL(saveManager, "SaveManagerSubsystem is null");

	const int numberSaves = saveManager->GetSaves().Num();

	CHECK_NULL(settings, "settings is null");

	TObjectPtr<UScrollBox> scrollBox = saveList->GetScrollbox();


	if (numberSaves == 0)
	{
		scrollBox->ClearChildren();
		scrollBox->AddChild(addButton);
		UScrollBoxSlot* _slot = Cast<UScrollBoxSlot>(addButton->Slot);
		addButton->SetParent(this);
		_slot->SetHorizontalAlignment(HAlign_Fill);
		_slot->SetVerticalAlignment(VAlign_Fill);
		return;
	}

	scrollBox->ClearChildren();

	const TSubclassOf<UButtonSaveTemplate> templateWidget = settings->GetTemplateSave();
	CHECK_NULL(templateWidget, "template of button save is null");

	for (int i = 0; i < numberSaves; i++)
	{
		if (UButtonSaveTemplate* _save = CreateWidget<UButtonSaveTemplate>(this, templateWidget))
		{
			scrollBox->AddChild(_save);
			_save->Setup(scrollBox->GetChildrenCount(), this);
			UScrollBoxSlot* _slot = Cast<UScrollBoxSlot>(_save->Slot);
			_slot->SetPadding(FMargin(0, 10));
		}
	}
	scrollBox->AddChild(addButton);
	UScrollBoxSlot* _slot = Cast<UScrollBoxSlot>(addButton->Slot);
	_slot->SetHorizontalAlignment(HAlign_Fill);
	_slot->SetVerticalAlignment(VAlign_Fill);
}

void UHostMenu::CallPopup() const
{
	if (AMainHUD* _hud = Cast<AMainHUD>(GETFPC->GetHUD()))
	{
		CHECK_NULL(popupIntro, "Popup Intro asset is null");
		UPopupConfirmation* _popup = _hud->GetPopup(_hud->GetKeyFromPopupData(popupIntro));
		_popup->Show();
	}
}

void UHostMenu::SaveCurrentSlot(const bool _isTutorial)
{
	saveManager->CreateSave();
	DisableAllWidgetInteractable();

	UGameplaySave* _save = saveManager->GetCurrentSave();
	//_save->SaveTime();
	_save->SetIsTutorial(_isTutorial);
	_save->SaveTuto(_isTutorial);
	_save->SaveGameplay(nullptr, !_isTutorial);
	saveManager->SaveCurrent();
	DisableAllWidgetInteractable();
}

void UHostMenu::LaunchMapTutorial()
{
	GameInstance->OnLevelLoad().RemoveDynamic(this, &UHostMenu::LaunchMapTutorial);

	//=========Pour le ONLINE==============

	//const EOnlineAccessibilityState _state =
	//	GameInstance->GetOnlineMode()
	//		? static_cast<EOnlineAccessibilityState>(SessionSelector->GetCurrentIndex())
	//		: Private;

	//GetWorld()->GetGameInstance()->GetSubsystem<UOnlineManagerSubsystem>()->CreateServer(
	//	GetWorld()->GetFirstPlayerController()->PlayerState->GetPlayerName(), _state,
	//	worldIntro.ToString() + "?listen");

	//=====================================

	UGameplayStatics::OpenLevel(GetWorld(), worldIntro);
}

void UHostMenu::PlayTutorial()
{
	SaveCurrentSlot(true);
	GameInstance->SetInTutorial(true);

	GameInstance->CallbackOnLevelChange(ETypeChange::MainToIntro);
	GameInstance->OnLevelLoad().AddUniqueDynamic(this, &UHostMenu::LaunchMapTutorial);
}

void UHostMenu::SkipTutorial()
{
	SaveCurrentSlot(false);

	GameInstance->SetInTutorial(false);
	GameInstance->CallbackOnLevelChange(ETypeChange::MainToHub);
	GameInstance->OnLevelLoad().AddUniqueDynamic(this, &UHostMenu::LaunchMap);
}

void UHostMenu::DisableAllWidgetInteractable()
{
	widgetToDisabled.Append({SessionSelector, saveList, backButton});
	widgetToDisabled.Append(saveList->GetScrollbox()->GetAllChildren());
	Super::DisableAllWidgetInteractable();
}

/*
void UHostMenu::RefreshColorsSaves()
{
	TObjectPtr<UScrollBox> scrollBox = saveList->GetScrollbox();

	const int sizeChild = scrollBox->GetChildrenCount();
	for (int i = 0; i < sizeChild; i++)
	{
		if (UButtonSaveTemplate* _button = Cast<UButtonSaveTemplate>(scrollBox->GetChildAt(i)))
		{
			_button->RefreshColor();
		}
	}
}*/

void UHostMenu::CreateSaveGame()
{
	CHECK_NULL(saveManager, "Save manager is null");


	Create(saveManager->GetNumberSave());
}
