// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/StateTreeConditionBlueprintBase.h"
#include "OperatorCondition.h"

#include "STCondition_DistanceActor.generated.h"

#define CONDITION_DISTANCE_ACTOR_USE_LOG false

UCLASS()
class ROBOTHUNTER_API USTCondition_DistanceActor : public UStateTreeConditionBlueprintBase
{
	GENERATED_BODY()
		UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess))
		TEnumAsByte<EOperatorCondition> operatorC = LESS_OR_EQUAL;
	UPROPERTY(EditAnywhere,BlueprintReadOnly,meta = (AllowPrivateAccess))
	TObjectPtr<AActor> actor = nullptr;
	UPROPERTY(EditAnywhere,BlueprintReadOnly,meta = (AllowPrivateAccess))
	TObjectPtr<AActor> player = nullptr;
	UPROPERTY(EditAnywhere,BlueprintReadOnly,meta = (AllowPrivateAccess))
	float distance = 50;

private:
	virtual bool TestCondition(FStateTreeExecutionContext& Context) const override;
	bool TestOperator() const;
};
