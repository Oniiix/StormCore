// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CustomPlayerTransition.h"
#include "ToHubTransition.generated.h"

#define TO_HUB_TRANSITION false

UCLASS()
class ROBOTHUNTER_API UToHubTransition : public UCustomPlayerTransition
{
	GENERATED_BODY()

public:
	virtual bool IsValidTransition()override;

};
