// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StateTreeComponent.h"
#include "StateTreeEvents.h"
#include "StateTreeExecutionContext.h"
#include "RobotHunter/Utils/ENUM_AI.h"
#include "AIStateTreeComponent.generated.h"

#define STATE_TREE_COMPONENT_USE_LOG false 

/**
 * StateTree of AIs
 */
 UCLASS()
class ROBOTHUNTER_API UAIStateTreeComponent : public UStateTreeComponent
{
	GENERATED_BODY()

#pragma region Properties
	UPROPERTY(EditAnywhere, Category = "AIStateTreeComponent_Property", meta = (UMin = 0, ClampMin = 0))
	float stunTime = 3.f;

#pragma region StateTreeEvents
	/// Player
	UPROPERTY(EditAnywhere, Category = "AIStateTreeComponent_Property|StateTreeEvent|Player", BlueprintReadOnly, meta = (AllowPrivateAccess))
	FStateTreeEvent eventOnTargetAcquire;
	UPROPERTY(EditAnywhere, Category = "AIStateTreeComponent_Property|StateTreeEvent|Player", BlueprintReadOnly, meta = (AllowPrivateAccess))
	FStateTreeEvent eventOnTargetLost;
	/// AI
	UPROPERTY(EditAnywhere, Category = "AIStateTreeComponent_Property|StateTreeEvent|AI", BlueprintReadOnly, meta = (AllowPrivateAccess))
	FStateTreeEvent eventOnDead;
	UPROPERTY(EditAnywhere, Category = "AIStateTreeComponent_Property|StateTreeEvent|AI", BlueprintReadOnly, meta = (AllowPrivateAccess))
	FStateTreeEvent eventOnStopMove;
	UPROPERTY(EditAnywhere, Category = "AIStateTreeComponent_Property|StateTreeEvent|AI", BlueprintReadOnly, meta = (AllowPrivateAccess))
	FStateTreeEvent eventOnStun;
	UPROPERTY(EditAnywhere, Category = "AIStateTreeComponent_Property|StateTreeEvent|AI", BlueprintReadOnly, meta = (AllowPrivateAccess))
	FStateTreeEvent eventOnEndAnimation;
	UPROPERTY(EditAnywhere, Category = "AIStateTreeComponent_Property|StateTreeEvent|AI|Trapper", BlueprintReadOnly, meta = (AllowPrivateAccess))
	FStateTreeEvent eventOnDodge;
	UPROPERTY(EditAnywhere, Category = "AIStateTreeComponent_Property|StateTreeEvent|AI|Trapper", BlueprintReadOnly, meta = (AllowPrivateAccess))
	FStateTreeEvent eventStartCamouflage;
#pragma endregion 
#pragma endregion

#pragma region Methods
public:
	UAIStateTreeComponent();

	FORCEINLINE void SetStateTreeRef(FStateTreeReference _stateTreeRef) { StateTreeRef = _stateTreeRef; }

	/// <summary>
	/// Initialize component
	/// </summary>
	void InitStateTreeComponent();

private:
	// Player
	/// <summary>
	/// Send eventOnTargetAcquire in State Tree 
	/// </summary>
	/// <param name="_player"> Player detected </param>
	UFUNCTION()
	void TargetAcquire(AMainCharacter* _player);
	/// <summary>
	/// Send eventOnTargetLost in StateTree 
	/// </summary>
	UFUNCTION()
	void TargetLost();

	// AI
	/// <summary>
	/// Send eventOnDead in StateTree 
	/// </summary>
	UFUNCTION()
	void AIDead(AAI_Base* _AI);
	/// <summary>
	/// Send eventOnStun in StateTree 
	/// </summary>
	UFUNCTION()
	void StunAI(const float _stunTime);
	/// <summary>
	/// Send eventOnEndAttack in StateTree 
	/// </summary>
	UFUNCTION()
	void EndAnimation();

	/// <summary>
	/// Send eventOnStopMove in StateTree 
	/// </summary>
	UFUNCTION()
	void StopMovement();

public:
	void DodgeEnable();
	void StartCamouflage();
#pragma endregion
};
