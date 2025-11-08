// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "RobotHunter/AI/Animation/AIBaseAnimInstance.h"
#include "TitanAnimInstance.generated.h"

UCLASS()
class ROBOTHUNTER_API UTitanAnimInstance : public UAIBaseAnimInstance
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI_Animation", meta = (AllowPrivateAccess))
	bool isInShield = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI_Animation", meta = (AllowPrivateAccess))
	float shieldBlendAlpha = 0.f;
	UPROPERTY(EditAnywhere, Category = "AI_Animation", meta = (Umin = 0.1f, ClampMin = 0.1f))
	float speedShieldBlendAlpha = 5.f;

private:
	void InitAnimInstance() override;
	void LaunchAnimation(const EAIAnimation& _animation) override;
	void NativeUpdateAnimation(float DeltaSeconds) override;

	UFUNCTION()
	void EnableShield();
	UFUNCTION()
	void DisableShield();
};
