// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RobotHunter/UI/Menu/Options/Data/Graphics/Module/GraphicsOptionBase.h"
#include "GlobalIllumination.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType, HideDropdown)
class ROBOTHUNTER_API UGlobalIlluminationOption final : public UGraphicsOptionBase
{
	GENERATED_BODY()

public:
	virtual void Init(const TObjectPtr<UOptionFrame>& _parent, const TObjectPtr<UGameUserSettings>& _gus,
	                  const TObjectPtr<UWorld>& _world, const
	                  TObjectPtr<USettingsSave>& _settings) override;
	virtual void Apply() override;
};
