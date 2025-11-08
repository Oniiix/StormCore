// Fill out your copyright notice in the Description page of Project Settings.


#include "ToPauseTransition.h"
#include "../../../CCC/Character/MainCharacter.h"

bool UToPauseTransition::IsValidTransition()
{
	RetrievePlayer();
	return player->GetInPause() && !player->GetDisableInput();
}