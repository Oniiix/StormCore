// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RobotHunter/AI/Animation/AIBaseAnimInstance.h"
#include "ProtectorAnimInstance.generated.h"

class AAI_Protector;

UCLASS()
class ROBOTHUNTER_API UProtectorAnimInstance : public UAIBaseAnimInstance
{
	GENERATED_BODY()

protected:
	void InitAnimInstance() override;
	void LaunchAnimation(const EAIAnimation& _animation) override;
};
