#include "MainHUD.h"

#include "RobotHunter/CCC/CustomPlayerController.h"
#include "RobotHunter/Game/GameMode/MainMenuGamemode.h"
#include "RobotHunter/UI/Frame/BaseFrame.h"
#include "RobotHunter/UI/Menu/Hub/StockMenu/NewStock.h"
#include "RobotHunter/Utils/DebugUtils.h"
#include "Settings/FocusSystem.h"
#include "Settings/MenuSettings.h"


void AMainHUD::BeginPlay()
{
	Super::BeginPlay();

	if (focusSystem)
	{
		focusSystem->Setup(this);
	}
	if (menuSettings)
	{
		menuSettings->Setup(this);
		SetState(menuSettings->GetStartUI());
	}
	for (const TTuple<FString, TObjectPtr<UPopupData>>& PopupData : PopupDatas)
	{
		UPopupConfirmation* _popup = CreatePopup(PopupData.Key);
		CHECK_NULL(_popup, "[MAIN HUD] Popup " + PopupData.Key + " is null");
		_popup->Setup(PopupData.Value);
		_popup->AddToViewport();
		_popup->HideWithoutEvent();
	}

	Cast<UCustomGameInstance>(GetWorld()->GetGameInstance())->OnLevelLoaded().Broadcast();
}

UUI* AMainHUD::SetState(const EStateUI _state)
{
	if (currentUI == _state)
	{
		return nullptr;
	}
	if (currentUI != EStateUI::None)
		GetUI(currentUI)->ExitUI();
	currentUI = _state;
	const int _index = menuSettings->GetIndexFromState(_state);
	if (_index == -1)
	{
		LOG_ERROR(MAIN_HUD_USE_LOG, "[" + GET_ENUM_STRING(_state) + "] State not found in list of UI.");
		return nullptr;
	}

	if (focusSystem)
	{
		focusSystem->SetFocusTo(menuSettings->IsATH(_state) ? EFocusMode::GAME : EFocusMode::UI);
	}

	if (const TObjectPtr<UWidgetSwitcher> switcher = menuSettings->GetCurrentFrame()->GetSwitcher())
	{
		const TObjectPtr<UUI> targetUI = Cast<UUI>(switcher->GetWidgetAtIndex(_index));
		if (targetUI)
		{
			targetUI->Refresh();

			if (_state == EStateUI::HubStock)
			{
				if (UNewStock* _stock = Cast<UNewStock>(targetUI))
				{
					_stock->OnShowStock().Broadcast();
				}
			}
		}
		switcher->SetActiveWidgetIndex(_index);
		return targetUI;
	}
	return nullptr;
}

void AMainHUD::Regenerate()
{
	menuSettings->Setup(this);
	SetState(EStateUI::Loading);
	LOG_TIME(true, "Regenerate method", 30);
	LOG_TIME(true, GET_ENUM_STRING(GetState()), 30);
}

void AMainHUD::HideUI()
{
	UUI* _ui = GetCurrentUI();
	CHECK_NULL(_ui, "Current UI is null");

	_ui->SetVisibility(ESlateVisibility::Hidden);
}

void AMainHUD::ShowUI()
{
	UUI* _ui = GetCurrentUI();
	CHECK_NULL(_ui, "Current UI is null");

	_ui->SetVisibility(ESlateVisibility::Visible);
}

void AMainHUD::ClosePopup(const FString& _namePopup)
{
	UPopupConfirmation* _popup = GetPopup(_namePopup);
	CHECK_NULL(_popup, "Popup is null");
	_popup->Hide();
}

UPopupConfirmation* AMainHUD::CreatePopup(const FString& _key)
{
	UPopupConfirmation* _popup = CreateWidget<UPopupConfirmation>(GetWorld(), popupPrefab);
	CHECK_NULL_WITH_RETURN(_popup, "Popup confirmation is null", nullptr);
	Popups.Add(_key, _popup);
	return _popup;
}

void AMainHUD::OnLoadFinish(EStateUI StateUI)
{
	SetState(StateUI);
}

void AMainHUD::RestartGame()
{
	AMainMenuGamemode* _gm = Cast<AMainMenuGamemode>(GetWorld()->GetAuthGameMode());
	CHECK_NULL(_gm, "Gamemode is null");

	_gm->SetLang();
	onLevelReload.Broadcast();
	//UKismetSystemLibrary::QuitGame(GetWorld(), GETFPC, EQuitPreference::Quit,true);
	UGameplayStatics::OpenLevel(GetWorld(), FName(UGameplayStatics::GetCurrentLevelName(GetWorld())));
}

void AMainHUD::RestoreLanguage()
{
	AMainMenuGamemode* _gm = Cast<AMainMenuGamemode>(GetWorld()->GetAuthGameMode());
	CHECK_NULL(_gm, "Gamemode is null");

	_gm->RestoreLang();
}


#if WITH_EDITOR
void AMainHUD::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	/*for (const TTuple<FString, TObjectPtr<UPopupData>>& PopupData : PopupDatas)
	{
		PopupData.Value->ClearOptions();
	}*/
}
#endif
