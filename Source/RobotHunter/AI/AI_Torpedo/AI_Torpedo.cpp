// Fill out your copyright notice in the Description page of Project Settings.


#include "AI_Torpedo.h"
#include "RobotHunter/AI/Arena/Arena.h"
#include <Net/UnrealNetwork.h>

AAI_Torpedo::AAI_Torpedo() : AAI_Base(EAIType::Torpedo)
{
}

void AAI_Torpedo::Emerging()
{
	isEmerging = true;
	On_Emerging();
}

void AAI_Torpedo::On_Emerging()
{
	INVOKE(onEmerging)
}

void AAI_Torpedo::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AAI_Torpedo, isEmerging);
}