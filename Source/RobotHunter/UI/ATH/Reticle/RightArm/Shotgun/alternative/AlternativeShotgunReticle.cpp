// 


#include "AlternativeShotgunReticle.h"
#include "RobotHunter/Game/GPE/Members/RightArmActors/CadenceShotgun/CadenceShotgunActor.h"

void UAlternativeShotgunReticle::InitWithDelay()
{
	ARightArmActor* _arm = GetRightMember();
	CHECK_NULL(_arm, "Right arm is null");

	shotgun = Cast<ACadenceShotgunActor>(_arm);
	CHECK_NULL(shotgun, "shotgun is null");

	rotationSpeed = 180 / shotgun->GetReductionDuration();

	Super::InitWithDelay();
}

void UAlternativeShotgunReticle::ResetCycleFailed()
{
	ResetCycle(false);
}

void UAlternativeShotgunReticle::ResetCycleSuccess(const FVector& Vector, const FVector& Vector1)
{
	ResetCycle(true);
}


void UAlternativeShotgunReticle::NativeConstruct()
{
	Super::NativeConstruct();
	rotation->SetRenderTransformAngle(currentAngle);
}

void UAlternativeShotgunReticle::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	if (on)
	{
		UpdateAngleValue(InDeltaTime);
		rotation->SetRenderTransformAngle(currentAngle);
	}
}

void UAlternativeShotgunReticle::RegisterEvents()
{
	Super::RegisterEvents();
	//shotgun->OnSh
	BIND_ACTION(shotgun, OnFailed, UAlternativeShotgunReticle, ResetCycleFailed);
	BIND_ACTION(shotgun, OnShoot, UAlternativeShotgunReticle, ResetCycleSuccess);
}

void UAlternativeShotgunReticle::ResetCycle(bool success)
{
	currentAngle = 0.0f;
	if (success)
	{
		PlayAnimation(AnimSuccess);
	}
	else
	{
		PlayAnimation(AnimFail);
	}
}

void UAlternativeShotgunReticle::UpdateAngleValue(const float InDeltaTime)
{
	currentAngle = currentAngle + rotationSpeed * InDeltaTime;
	if (currentAngle >= EndCycleAngle)
	{
		ResetCycle(false);
	}
}
