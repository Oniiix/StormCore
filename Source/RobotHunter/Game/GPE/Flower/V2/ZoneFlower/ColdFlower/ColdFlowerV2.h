#pragma once

#include "CoreMinimal.h"
#include "RobotHunter/Game/GPE/Flower/V2/ZoneFlower/ZoneFlowerV2.h"
#include "RobotHunter/CCC/Character/Components/Status/StatusComponent.h"
#include "ColdFlowerV2.generated.h"

UCLASS()
class ROBOTHUNTER_API AColdFlowerV2 : public AZoneFlowerV2
{
	GENERATED_BODY()

	/** Multiplier applied to the duration of freeze stacks. */
	UPROPERTY(EditAnywhere, Category = "Custom Property|Modifiers", meta = (UIMin = 1.0f, ClampMin = 1.0f))
	float freezeStackDurationMultiplier;

	/** Determines whether the multiplier should be applied when adding a stack. */
	UPROPERTY(EditAnywhere, Category = "Custom Property|Modifiers")
	bool useMuliplierWhenAddingStack;
	
	/** Parameters for applying the freezing (FREEZING) status effect. */
	UPROPERTY(EditAnywhere, Category = "Custom Property|Status")
	FStatusParams freezingStatus;

	/** Parameters for applying the stunned (STUNNED) status when the maximum stack is reached. */
	UPROPERTY(EditAnywhere, Category = "Custom Property|Status|MaxStack")
	FStatusParams stunnedStatus;

	/** Parameters for applying the protected (PROTECTED) status when the maximum stack is reached. */
	UPROPERTY(EditAnywhere, Category = "Custom Property|Status|MaxStack")
	FStatusParams protectedStatus;

	/** Parameters for applying the resistant (RESISTANT) status when the maximum stack is reached. */
	UPROPERTY(EditAnywhere, Category = "Custom Property|Status|MaxStack")
	FStatusParams resistantStatus;

	/** Flag used to enable or disable modifiers like stack duration multipliers. */
	bool useModifiers;

public:
	/** Default constructor that initializes the component's properties. */
	AColdFlowerV2();

protected:
	/**
	 * Starts the active phase of the flower.
	 * During this phase, certain weather effects are triggered, or specific behaviors are applied.
	 */
	virtual void StartActivePhase() override;
	/**
	 * Starts the cooldown phase of the flower.
	 * During this phase, freezing statuses are refreshed for actors in the effect area.
	 */
	virtual void StartCooldownPhase() override;

	/**
	 * Applies the defined status effects (e.g., freezing, stack modifications) to all actors
	 * within the effect area.
	 */
	virtual void ApplyEffectToActorsInArea() override;

	/**
	 * Enables modifiers to allow applying additional behaviors such as duration multipliers.
	 */
	virtual void EnableModifiers() override;
	/**
	 * Disables modifiers to stop applying additional behaviors such as duration multipliers.
	 */
	virtual void DisableModifiers() override;
};
