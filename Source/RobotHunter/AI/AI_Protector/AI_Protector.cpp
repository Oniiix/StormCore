// Fill out your copyright notice in the Description page of Project Settings.

#include "AI_Protector.h"
#include "RobotHunter/AI/Settings/ParalarvaSettingsDataAsset.h"
#include <Net/UnrealNetwork.h>

AAI_Protector::AAI_Protector() : AAI_Base(EAIType::Paralarva)
{
}

void AAI_Protector::SetAI(AArena* _arena)
{
	UParalarvaSettingsDataAsset* _paralarvaSettings = Cast<UParalarvaSettingsDataAsset>(settings);
	if (!_paralarvaSettings)
	{
		LOG_ERROR(AI_PARALARVA_USE_LOG, "Failed to cast settings in Paralarva' settings");
		return;
	}

	Super::SetAI(_arena);
}
