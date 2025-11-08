// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CustomPlayerTransition.h"
#include "ToPauseTransition.generated.h"

/**
 * 
 */
UCLASS()
class ROBOTHUNTER_API UToPauseTransition : public UCustomPlayerTransition
{
	GENERATED_BODY()
	
public:
	bool IsValidTransition();

};
