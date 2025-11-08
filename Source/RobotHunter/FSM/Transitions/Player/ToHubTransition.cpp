// Fill out your copyright notice in the Description page of Project Settings.


#include "ToHubTransition.h"
#include "../../../CCC/Character/MainCharacter.h"

bool UToHubTransition::IsValidTransition()
{
	RetrievePlayer();
	//LOG_COLOR_TIME(TO_HUB_TRANSITION,"GO TO HUB !", ORANGE, 2);
	const bool _HUBStatus = player->GetCustomGameInstance()->GetGameStatus() == EGameStatus::HUB;
	//if(_goodStatus && !player->GetInMenu())
	return _HUBStatus && !player->GetInMenu()  && !player->GetInPause() && !player->GetDisableInput();
}
