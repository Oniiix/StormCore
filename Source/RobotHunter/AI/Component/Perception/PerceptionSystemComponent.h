// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PerceptionSystemComponent.generated.h"

#define PERCEPTION_SYSTEM_COMPONENT_USE_LOG false
#define DELTA_TICK 0.1f

class AAI_Base;
class AMainCharacter;

/**
 * The base abstract class for perception component
 */
UCLASS(ABSTRACT, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class ROBOTHUNTER_API UPerceptionSystemComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDetected, AMainCharacter*, _player);
	FOnDetected onDetected;

#pragma region Properties
	/// Base
	UPROPERTY(EditAnywhere, Category = "Perception_Property", meta = (EditCondition = "!disablePerception", UMin = 100, ClampMin = 100))
	int rangePerception = 100;
	UPROPERTY(EditAnywhere, Category = "Perception_Property", meta = (EditCondition = "!disablePerception"))
	TArray<TEnumAsByte<EObjectTypeQuery>> layersDetection = {};
	UPROPERTY(EditAnywhere, Category = "Perception_Property")
	bool disablePerception = false;
	/// Debug
	UPROPERTY(EditAnywhere, Category = "Perception_Property", meta = (EditCondition = "!disablePerception"))
	bool drawDebug = false;

	AAI_Base* owner = nullptr;
	FTimerHandle timerTick;
	bool isInitializationGood = false;
#pragma endregion

#pragma region Methods
public:
	UPerceptionSystemComponent();
	/// Events
	FORCEINLINE FOnDetected& OnDetected() { return onDetected; }
	/// Getter
	FORCEINLINE int GetRange() const { return rangePerception; }
	FORCEINLINE FVector GetOwnerLocation() const { return GetOwner()->GetActorLocation(); }
	FORCEINLINE TArray<TEnumAsByte<EObjectTypeQuery>> GetLayersDetection() const { return layersDetection; }
	/// Setter
	FORCEINLINE void SetRangePerception(const int _rangePerception) { rangePerception = _rangePerception; }
	FORCEINLINE void SetLayersDetection(const TArray<TEnumAsByte<EObjectTypeQuery>>& _layersDetection) { layersDetection = _layersDetection; }
	FORCEINLINE void SetDisablePerception(const bool _disablePerception) { disablePerception = _disablePerception; }
	FORCEINLINE void SetDrawDebug(const bool _drawDebug) { drawDebug = _drawDebug; }

	virtual void InitPerception();
	/// <summary>
	/// Enable/disable the tracking system (via allowing tick)
	/// </summary>
	/// <param name="_enable"> enable or not </param>
	virtual void EnablePerception(const bool _enable);

protected:
	void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/// <summary>
	/// The behaviour of the perception
	/// </summary>
	UFUNCTION()
	virtual void PerceptionBehaviour();
	/// <summary>
	/// The component detected a player
	/// </summary>
	/// <param name="_playerDetected"> player detected </param>
	virtual	void Detected(AActor* _playerDetected);
	/// <summary>
	/// Draw the debug of perception
	/// </summary>
	virtual bool DrawDebug();
#pragma endregion
};
