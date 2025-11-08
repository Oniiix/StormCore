// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../ParaboleProjectile.h"
#include "Engine/TriggerSphere.h"
#include "TriggerProjectileSphere.h"
#include "BlastProjectile.generated.h"

UCLASS(HideDropdown)
class ROBOTHUNTER_API ABlastProjectile : public AParaboleProjectile
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, Category = "BlastProjectile_Property")
	float radiusBlast = 5;
	UPROPERTY(EditAnywhere, Category = "BlastProjectile_Property")
	float damage = 5;

	UPROPERTY(EditAnywhere, Category = "BlastProjectile_Property")
	TSubclassOf<ATriggerProjectileSphere> triggerSphere = nullptr;
};
