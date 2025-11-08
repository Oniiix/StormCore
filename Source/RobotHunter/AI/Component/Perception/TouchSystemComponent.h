// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PerceptionSystemComponent.h"
#include "TouchSystemComponent.generated.h"

#define TOUCH_SYSTEM_COMPONENT_USE_LOG false

/**
 * The touch system of AI to detect too close player 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class ROBOTHUNTER_API UTouchSystemComponent : public UPerceptionSystemComponent
{
	GENERATED_BODY()

public:
	void InitPerception() override;

private:
	/// <summary>
	/// The behaviour of touch perception
	/// </summary>
	void PerceptionBehaviour() override;
	/// <summary>
	/// Draw the debug of touch perception
	/// </summary>
	bool DrawDebug() override;
};
