// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/StateTreeConditionBlueprintBase.h"
#include "OperatorCondition.h"

#include "STCondition_Distance.generated.h"

#define CONDITION_DISTANCE_USE_LOG false

UCLASS()
class ROBOTHUNTER_API USTCondition_Distance : public UStateTreeConditionBlueprintBase
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess))
	TEnumAsByte<EOperatorCondition> operatorC = LESS_OR_EQUAL;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess))
	FVector newLocation;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess))
	TObjectPtr<AActor> actor = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess))
	float distance;

	UPROPERTY(EditAnywhere)
	bool debugMode = false;

	FTimerHandle timer;


private:
	virtual bool TestCondition(FStateTreeExecutionContext& Context) const override;
	bool TestOperator() const;
};
