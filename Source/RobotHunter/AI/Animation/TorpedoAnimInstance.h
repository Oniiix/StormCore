// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RobotHunter/AI/Animation/AIBaseAnimInstance.h"
#include "TorpedoAnimInstance.generated.h"

class AAI_Torpedo;

UCLASS()
class ROBOTHUNTER_API UTorpedoAnimInstance : public UAIBaseAnimInstance
{
	GENERATED_BODY()

protected:
	void InitAnimInstance() override;
};
