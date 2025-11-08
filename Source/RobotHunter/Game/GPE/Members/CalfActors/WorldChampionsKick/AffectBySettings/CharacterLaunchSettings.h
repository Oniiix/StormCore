// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../LaunchSettings.h"
#include "CharacterLaunchSettings.generated.h"

/**
 * 
 */
UCLASS()
class ROBOTHUNTER_API UCharacterLaunchSettings : public ULaunchSettings
{
	GENERATED_BODY()
	
	TObjectPtr<ACharacter> owner = nullptr;

public:
	virtual void Init(AActor* _owner) override;
	virtual void Launch(const FVector& _dir) override;
};
