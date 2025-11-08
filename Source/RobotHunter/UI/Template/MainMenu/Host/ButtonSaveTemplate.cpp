// Fill out your copyright notice in the Description page of Project Settings.


#include "ButtonSaveTemplate.h"

#include "Kismet/KismetTextLibrary.h"
#include "RobotHunter/SaveSystem/GameplaySave.h"
#include "RobotHunter/UI/Menu/MainMenu/HostMenu/HostMenu.h"
#include "RobotHunter/SaveSystem/SaveManagerSubsystem.h"
#include "RobotHunter/UI/Menu/MainMenu/HostMenu/HostSettings.h"
#include "RobotHunter/Utils/DebugUtils.h"

void UButtonSaveTemplate::Setup(const int _index, UHostMenu* _hostMenu/*, UHostSettings* _settings*/)
{
	hostMenu = _hostMenu;
	indexSave = _index;
	//settings = _settings;

	saveManager = GetWorld()->GetGameInstance()->GetSubsystem<USaveManagerSubsystem>();
	CHECK_NULL(saveManager, "SaveManager is null");

	UGameplaySave* _save = saveManager->GetSave(indexSave - 1);
	CHECK_NULL(_save, "Save is null");

	/*SET_TEXT_STR(textBase->GetText(), settings->GetFormatedSaveText(indexSave));
	SET_TEXT_STR(money->GetText(), settings->GetFormatedMoneyText(_save->GetPlayerSave().money));
	SET_TEXT_STR(sectorUnlock->GetText(), settings->GetFormatedSectorText(_save->GetPlayerSave().lastSector));*/

	const FDateTime _date = _save->GetPlayerSave().date;

	SET_TEXT_STR(textBase->GetText(), FORMAT(textFormat,
		             FORMAT(saveDateFormat.ToString(), CONV_INT(_date.GetMonth(), 2), CONV_INT(_date.GetDay(), 2), _date
			             .
			             GetYear())));
	SET_TEXT_STR(textHour->GetText(), FORMAT(textFormat,
		             FORMAT(hourDateFormat.ToString(), CONV_INT(_date.GetHour(), 2), CONV_INT(_date.GetMinute(), 2),
			             CONV_INT(_date.GetSecond(), 2))));
	SET_TEXT_STR(saveNumber->GetText(),
	             FORMAT(textFormat, FORMAT(saveNumberFormat.ToString(), CONV_INT( indexSave, 2))));
	//RefreshColor();
}

void UButtonSaveTemplate::RefreshColor()
{
	CHECK_NULL(saveManager, "SaveManager is null");


	//TODO ButtonMemberTemplate - remake change style of button

	/*CHECK_NULL(!settings->GetSelectedSave(), "Settings of style of selected save is null");
	CHECK_NULL(!settings->GetNormalSave(), "Settings of style of normal save is null");*/

	/*selectSave->SetStyle(saveManager->GetIndexOfSave() + 1 == indexSave
		                     ? settings->GetSelectedSave()->GetStyle()
		                     : settings->GetNormalSave()->GetStyle());*/
}

void UButtonSaveTemplate::RegisterEvents()
{
	Super::RegisterEvents();

	BIND_ACTION(deleteBtn, OnClick, UButtonSaveTemplate, DeleteSave);
	BIND_ACTION_PARAM(buttonBase, OnClicked, UButtonSaveTemplate, SelectSave);
}

void UButtonSaveTemplate::DeleteSave()
{
	CHECK_NULL(saveManager, "SaveManager is null");

	if (saveManager->DeleteSave(indexSave - 1))
	{
		hostMenu->Refresh();
	}
}

void UButtonSaveTemplate::SelectSave()
{
	CHECK_NULL(saveManager, "SaveManager is null");

	hostMenu->OnPlay().Broadcast(indexSave - 1);
	//hostMenu->RefreshColorsSaves();
}
