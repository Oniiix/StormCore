// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RobotHunter/Utils/ENUM_AI.h"
#include "TrackingSystemComponent.generated.h"

#define TRACKING_SYSTEM_COMPONENT_USE_LOG false

class AMainCharacter;
class AAI_Base;
class UAIBaseAnimInstance;
class UDamageSystemComponent;

/**
 * The tracking system of AIs to handle combat behaviour
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class ROBOTHUNTER_API UTrackingSystemComponent : public UActorComponent
{
	GENERATED_BODY()

#pragma region Events
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTargetAcquire, AMainCharacter*, _target);
	FOnTargetAcquire onTargetAcquire;
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEndLookAtTarget);
	FOnEndLookAtTarget onEndLookAtTarget;
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDetection);
	FOnDetection onDetection;
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTargetLost);
	FOnTargetLost onTargetLost;
#pragma endregion Events

#pragma region Properties
	UPROPERTY(VisibleAnywhere, Category = "TrackingSystem_Property")
	TObjectPtr<AMainCharacter> target = nullptr;
	UPROPERTY(EditAnywhere, Category = "TrackingSystem_Property")
	bool disableTracking = false;
	UPROPERTY(EditAnywhere, Category = "TrackingSystem_Property")
	bool isLookingAtTarget = false;
	/// Debug
	UPROPERTY(EditAnywhere, Replicated, Category = "TrackingSystem_Property", meta = (EditCondition = "!disableTracking"))
	bool drawDebug = false;

	TObjectPtr<AAI_Base> owner = nullptr;
	TObjectPtr<UAIBaseAnimInstance> AIBaseAnimInstance = nullptr;
	TObjectPtr<UDamageSystemComponent> damageSystem = nullptr;
	bool isInitializationGood = false;
#pragma endregion Properties

#pragma region Methods
public:
	UTrackingSystemComponent();

	/// Event
	FORCEINLINE FOnDetection& OnDetection() { return onDetection; }
	FORCEINLINE FOnEndLookAtTarget& OnEndLookAtTarget() { return onEndLookAtTarget; }
	FORCEINLINE FOnTargetAcquire& OnTargetAcquire() { return onTargetAcquire; }
	FORCEINLINE FOnTargetLost& OnTargetLost() { return onTargetLost; }
	/// Setter
	FORCEINLINE void SetDisableTracking(const bool _disableTracking) { disableTracking = _disableTracking; }
	FORCEINLINE void SetDrawDebug(const bool _drawDebug) { drawDebug = _drawDebug; }
	void SetLookAtTarget(const bool _value);
	void SetTarget(AMainCharacter* _target);
	/// Getter
	FORCEINLINE AMainCharacter* GetTarget() const { return target; }
	FORCEINLINE FVector GetOwnerLocation() const { return GetOwner()->GetActorLocation(); }

	/// <summary>
	/// Initialize tracking system
	/// </summary>
	void InitTracking();
	/// <summary>
	/// IA lose the target
	/// </summary>
	void LoseTarget();	

private:
	void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/// <summary>
	/// Interpolation to look at current target
	/// </summary>
	/// <param name="_deltaTime"> Tick's delta time</param>
	void LookAtTarget(const float _deltaTime);
	
	UFUNCTION()
	void TakeDamage(AMainCharacter* _target);

	/// <summary>
	/// Draw debug of tracking system
	/// </summary>
	void DrawDebug();

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;
#pragma endregion
};
