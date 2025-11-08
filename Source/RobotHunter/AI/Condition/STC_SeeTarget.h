// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/StateTreeConditionBlueprintBase.h"
#include "STC_SeeTarget.generated.h"

#define CONDITION_SEE_TARGET_USE_LOG false

UCLASS()
class ROBOTHUNTER_API USTC_SeeTarget : public UStateTreeConditionBlueprintBase
{
	GENERATED_BODY()
private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess))
		TObjectPtr<AActor> actor = nullptr;
protected:
	virtual bool TestCondition(FStateTreeExecutionContext& Context) const override;
};
