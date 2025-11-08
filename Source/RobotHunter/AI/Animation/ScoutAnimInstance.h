// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RobotHunter/AI/Animation/AIBaseAnimInstance.h"
#include "ScoutAnimInstance.generated.h"

class AAI_Scout;

UCLASS()
class ROBOTHUNTER_API UScoutAnimInstance : public UAIBaseAnimInstance
{
	GENERATED_BODY()

private:
	void InitAnimInstance() override;
};
