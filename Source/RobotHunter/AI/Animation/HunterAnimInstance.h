// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RobotHunter/AI/Animation/AIBaseAnimInstance.h"
#include "HunterAnimInstance.generated.h"

class AAI_Hunter;

UCLASS()
class ROBOTHUNTER_API UHunterAnimInstance : public UAIBaseAnimInstance
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI_Animation", meta = (AllowPrivateAccess))
	bool onAcidShot = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI_Animation", meta = (AllowPrivateAccess))
	bool onDirectShot = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI_Animation", meta = (AllowPrivateAccess))
	bool onDodge = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI_Animation", meta = (AllowPrivateAccess))
	int dodgeDirection = false;

private:
	void LaunchAnimation(const EAIAnimation& _animation) override;
	void EndAnimation() override;

	void InitAnimInstance() override;

public:
	void Dodge(const int _dodgeRight);
};
