// 


#include "GameFrame.h"

#include "BonusInformation.h"
#include "GameSectionSettings.h"
#include "RobotHunter/CCC/Character/MainCharacter.h"
#include "RobotHunter/Network/OnlineManagerSubsystem.h"
#include "RobotHunter/SaveSystem/GameplaySave.h"
#include "RobotHunter/SaveSystem/SaveManagerSubsystem.h"
#include "RobotHunter/UI/ATH/EndDay/EndDay.h"
#include "RobotHunter/UI/ATH/Game/NewGameATH.h"
#include "RobotHunter/UI/HUD/MainHUD.h"
#include "RobotHunter/UI/Menu/Pause/Network/PlayerSession.h"
#include "RobotHunter/UI/Template/Prefab/RowBonus/PrefabRowBonus.h"
#include "RobotHunter/Utils/ENUM_UI.h"

void UGameFrame::Init()
{
	Super::Init();
	GameInstance = GetGameInstance<UCustomGameInstance>();
	USaveManagerSubsystem* saveManager = GetGameInstance()->GetSubsystem<USaveManagerSubsystem>();
	if (GameInstance->GetGameStatus() == HUB || GameInstance->GetInTutorial() || (saveManager->GetCurrentSave() &&
		saveManager->GetCurrentSave()->
		             GetGameSave().day == 3))
	{
		backHUBBtn->RemoveFromParent();
	}
	else if (!GETFPC->HasAuthority())
	{
		backHUBBtn->SetIsEnabled(false);
	}
	const bool _isOnline = GameInstance->GetOnlineMode();

	players->SetVisibility(_isOnline ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	players->SetIsEnabled(_isOnline);

	USaveManagerSubsystem* _saveManager = GetWorld()->GetGameInstance()->GetSubsystem<USaveManagerSubsystem>();
	if (UGameplaySave* _save = _saveManager->GetCurrentSave())
	{
		FBonusSave _bonus = _save->GetBonusSave();
		for (const FBonusStruct& BonusStruct : _bonus.bonusStructs)
		{
			FBonusInformation s = FBonusInformation();
			s.SetBonus(BonusStruct);
			bonusStructList.Add(s);
		}
	}

	RefreshBonusList();
}

void UGameFrame::RegisterEvents()
{
	Super::RegisterEvents();
	BIND_ACTION(backBtn, OnClick, UGameFrame, Back);
	BIND_ACTION(quitBtn, OnClick, UGameFrame, Quit);
	BIND_ACTION(backHUBBtn, OnClick, UGameFrame, BackHUB);
	BIND_ACTION(OptionBtn, OnClick, UGameFrame, ShowOption);

	AMainCharacter* _character = Cast<AMainCharacter>(GETFPC->GetCharacter());
	CHECK_NULL(_character, "Character is null");

	if (GameInstance->GetGameStatus() != HUB)
		BIND_ACTION(_character->GetBonusComponent(), GetOnAddBonus, UGameFrame, AddBonus);
}

void UGameFrame::Refresh()
{
	Super::Refresh();
	players->Refresh();
	backBtn->SetFocus();
}


void UGameFrame::Back()
{
	if (AMainHUD* _hud = Cast<AMainHUD>(GETFPC->GetHUD()))
	{
		UPauseMenu* _pause = _hud->GetUI<UPauseMenu>(EStateUI::MainPause);
		_pause->GetOnHideMap().Broadcast();
	}

	hud->SetState(GameInstance->GetGameStatus() == HUB
		              ? EStateUI::PlayerLobbyUI
		              : EStateUI::PlayerGameUI);

	const TObjectPtr<AMainCharacter> _player = Cast<AMainCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());

	CHECK_NULL(_player, "player is null");

	_player->SetInPause(false);

	if (UNewGameAth* _ath = hud->GetCurrentUI<UNewGameAth>())
	{
		_ath->LoadReticle();
	}
}

void UGameFrame::QuitSession()
{
	GameInstance->OnLevelLoad().RemoveDynamic(this, &UGameFrame::QuitSession);
	//if (UOnlineManagerSubsystem* _onlineManager = GetWorld()->GetGameInstance()->GetSubsystem<
	//	UOnlineManagerSubsystem>())
	//{
	owner->DisableAllWidgetInteractable();
	//	_onlineManager->QuitServer();
	//}
	UGameplayStatics::OpenLevel(GetWorld(), FName(GameInstance->GetMainMenuLevel().GetAssetName()));
}


//void UGameFrame::QuitTutorial()
//{
//	if (mainLevel == "")
//	{
//		LOG_ERROR(true, "Main Level is null");
//		return;
//	}
//	owner->DisableAllWidgetInteractable();
//	GameInstance->OnLevelLoad().RemoveDynamic(this, &UGameFrame::QuitTutorial);
//	UGameplayStatics::OpenLevel(this, mainLevel);
//}

void UGameFrame::Quit()
{
	GameInstance->CallbackOnLevelChange(GameInstance->GetInTutorial()
		                                    ? ETypeChange::IntroToMain
		                                    : GameInstance->GetGameStatus() == HUB
		                                    ? ETypeChange::HubToMain
		                                    : ETypeChange::GameToMain);

	//if (!GameInstance->GetInTutorial())
	//	GameInstance->OnLevelLoad().AddUniqueDynamic(this, &UGameFrame::QuitSession);
	//else
	GameInstance->OnLevelLoad().AddUniqueDynamic(this, &UGameFrame::QuitSession);
}


void UGameFrame::BackHUB()
{
	const FString levelName = settings->GetLevelToHub();
	if (levelName.IsEmpty())
	{
		LOG_ERROR(true, "Level to travel is null");
		return;
	}

	hud->SetState(EStateUI::DayEnd);
	UEndDay* endDay = hud->GetCurrentUI<UEndDay>();
	CHECK_NULL(endDay, "End day UI is null");

	endDay->Callback_OnDayNext();
	/*
		owner->DisableAllWidgetInteractable();
	
		GameInstance->OnLevelLoad().AddUniqueDynamic(this, &UGameFrame::LoadMap);
	
		GameInstance->CallbackOnLevelChange(ETypeChange::GameToHub);
	
		USaveManagerSubsystem* _saveManager = GetWorld()->GetGameInstance()->GetSubsystem<USaveManagerSubsystem>();
		CHECK_NULL(_saveManager, "Save manager is null");
		AMainCharacter* _character = Cast<AMainCharacter>(GETFPC->GetPawn());
		CHECK_NULL(_character, "Character is null");
		UGameplaySave* _save = _saveManager->GetCurrentSave();
		CHECK_NULL(_save, "Pas de save ! Lance depuis le main menu fdp !");
		_save->SaveBonus(_character);
		_character->GetDayManager()->UpdateDay();
		_save->SaveGameplay(_character);
		_saveManager->SaveCurrent(false);*/
}

void UGameFrame::ShowOption()
{
	owner->ShowOption();
}

void UGameFrame::AddBonus(const FBonusStruct& Bonus)
{
	if (FBonusInformation* ExistingBonus = bonusStructList.FindByPredicate(
		[&](const FBonusInformation& Info) { return Info.IsSame(Bonus); }))
	{
		ExistingBonus->RefreshCurrentBonus(Bonus);
	}
	else
	{
		FBonusInformation infos;
		infos.SetBonus(Bonus);
		bonusStructList.Add(MoveTemp(infos));
	}

	RefreshBonusList();
}


void UGameFrame::DisableAllWidgetInteractable()
{
	widgetToDisabled.Append({quitBtn, backBtn, backHUBBtn, OptionBtn});
	Super::DisableAllWidgetInteractable();
}

void UGameFrame::HandleInputChanged(bool isGamepad)
{
	Super::HandleInputChanged(isGamepad);
	if (isGamepad)
		backBtn->SetFocus();
}

void UGameFrame::RefreshBonusList()
{
	bool bHasUnlockBonus = false;

	for (const FBonusInformation& BonusStructList : bonusStructList)
	{
		bHasUnlockBonus = BonusStructList.stack != 0;
		if (bHasUnlockBonus) break;
	}


	NoBonusText->SetVisibility(bHasUnlockBonus ? ESlateVisibility::Hidden : ESlateVisibility::Visible);
	NoBonusText->SetIsEnabled(bHasUnlockBonus);

	BonusList->SetVisibility(!bHasUnlockBonus ? ESlateVisibility::Hidden : ESlateVisibility::Visible);
	BonusList->SetIsEnabled(bHasUnlockBonus);

	BonusList->ClearChildren();
	if (!bHasUnlockBonus) return;


	for (const FBonusInformation& StructList : bonusStructList)
	{
		if (StructList.stack == 0) continue;

		CHECK_NULL(rowPrefabBonus, "Bonus prefab reference is null");
		UPrefabRowBonus* _bonus = CreateWidget<UPrefabRowBonus>(this, rowPrefabBonus);
		CHECK_NULL(_bonus, "Bonus prefab is null");
		BonusList->AddChild(_bonus);
		_bonus->SetBonus(StructList);

		/*
		SET_TEXT_STR(_bonus->GetTextBonus()->GetText(),
		             bonusData->GetNameFormated( Bonus.Key, Bonus.Value.stack));
		SET_TEXT_STR(_bonus->GetDescriptionBonus()->GetText(),
		             bonusData->GetDescriptionFormated( Bonus.Key, Bonus.Value.multiplier));*/
	}
}

bool UGameFrame::IsHUB() const
{
	return GameInstance->GetGameStatus() == HUB;
}
