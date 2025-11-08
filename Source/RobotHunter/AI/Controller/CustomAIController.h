// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "AIController.h"
#include "CustomAIController.generated.h"

#define AI_CONTROLLER_USE_DEBUG false

UCLASS()
class ROBOTHUNTER_API ACustomAIController : public AAIController
{
	GENERATED_BODY()

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnReachedDestination);
	FOnReachedDestination onReachedDestination;

public:
	FORCEINLINE FOnReachedDestination& OnReachedDestination() { return onReachedDestination; }

private:
	void OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result) override;
};
