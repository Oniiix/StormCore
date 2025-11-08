// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LaunchSettings.generated.h"

/**
 * 
 */
UCLASS(abstract, NotBlueprintable)
class ROBOTHUNTER_API ULaunchSettings : public UObject
{
	GENERATED_BODY()

public:
	virtual void Init(AActor* _owner);
	virtual void Launch(const FVector& _dir) PURE_VIRTUAL(UAffectWCKSettings::Affect(), );
};
