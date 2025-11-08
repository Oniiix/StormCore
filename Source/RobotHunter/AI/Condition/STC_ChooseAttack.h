// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/StateTreeConditionBlueprintBase.h"
#include "STC_ChooseAttack.generated.h"


UCLASS()
class ROBOTHUNTER_API USTC_ChooseAttack : public UStateTreeConditionBlueprintBase
{
	GENERATED_BODY()

	//UPROPERTY(EditAnywhere, Category = "Protector")
	//TEnumAsByte <EChoseAttack>  chooseAttack;

protected:
	virtual bool TestCondition(FStateTreeExecutionContext& Context) const override;
};
