// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RobotHunter/AI/AI_Base.h"
#include "AI_Protector.generated.h"

#define AI_PARALARVA_USE_LOG false

class ANest;

/// <summary>
/// The class of AI Protector entities
/// </summary>
UCLASS(HideDropdown)
class ROBOTHUNTER_API AAI_Protector : public AAI_Base
{
	GENERATED_BODY()

public:
	AAI_Protector();

private:
	void SetAI(AArena* _arena) override;
};
