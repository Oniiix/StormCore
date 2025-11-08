// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseReticle.h"

#include "RobotHunter/Game/GPE/Members/ThighActors/OffRoadRollerdrives/OffRoadRollerdrivesActor.h"

void UBaseReticle::Init()
{
	Super::Init();
	//LOG_DEBUG("TEST")
	reticle->SetOpacity(1.0f);
	reticle->SetVisibility(ESlateVisibility::Visible);
}

void UBaseReticle::SetMaterial(AThighActor* _actor)
{
	reticle->SetOpacity(0.0f);
	reticle->SetVisibility(ESlateVisibility::Hidden);

	member = _actor;
	isSprint = INSTANCEOF(member, AOffRoadRollerdrivesActor);
	cooldownSprintDash->SetBrushFromMaterial(isSprint ? sprintMaterial : dashMaterial);
	BindEvent();
}

bool UBaseReticle::HasAlreadyTheMaterial(AThighActor* _actor)
{
	return member && isSprint == INSTANCEOF(member, AOffRoadRollerdrivesActor);
}

void UBaseReticle::BindEvent()
{
	if (!member) return;

	BIND_ACTION(member, OnPress, UBaseReticle, StartPressed);
	BIND_ACTION(member, OnCooldownStart, UBaseReticle, StartReload);
	if (isSprint)
	{
		if (AOffRoadRollerdrivesActor* _member = Cast<AOffRoadRollerdrivesActor>(member))
		{
			//BIND_ACTION(_member, OnCooldownEnd, UBaseReticle, HideCooldown);
			//BIND_ACTION(_member, OnCooldownStart, UBaseReticle, StartCooldown);
			BIND_ACTION(_member, OnEndStaminaReload, UBaseReticle, EndReload);
			BIND_ACTION(_member, OnUseStamina, UBaseReticle, UpdateValue);
		}
	}
	else
		BIND_ACTION(member, OnCooldownEnd, UBaseReticle, EndReload);
}

void UBaseReticle::HideForce() const
{
	cooldownSprintDash->SetOpacity(0.0f);
}

void UBaseReticle::ShowForce() const
{
	cooldownSprintDash->SetOpacity(1.0f);
}

void UBaseReticle::SetCooldown(const float _cooldown)
{
	//isReloading = true;
	//cooldown = _cooldown;
	//bWaitingToHide = false;
	//hideDelayElapsed = 0.0f;
	//if (!isShowed)
	//	onShow.Broadcast();
}

void UBaseReticle::SetCooldownValue(float _stamina, float _maxStamina)
{
	//TODO voir après pour le sprint	
	//if (!isShowed)
	//	onShow.Broadcast();
	//cooldown = _maxStamina;
	//
	//bWaitingToHide = false;
	//hideDelayElapsed = 0.0f;
	//
	//if (_stamina != _maxStamina)
	//	isFull = false;
	//

	cooldownSprintDash->GetDynamicMaterial()->SetScalarParameterValue("cooldown", _stamina / _maxStamina);
}

void UBaseReticle::EndReload()
{
	onHide.Broadcast();
	if (!isSprint)
	{
		isReloading = false;
		onUpdateCooldownValue.Broadcast(1.0f);
	}
	//isFull = true;
	//bWaitingToHide = true;
}

void UBaseReticle::StartReload(const float _max)
{
	if (isSprint)
		onUpdateCooldownValue.Broadcast(0.0f);
	else
		isReloading = true;
}

void UBaseReticle::UpdateValue(const float _value, const float _max)
{
	onUpdateCooldownValue.Broadcast(_value / _max);
}

void UBaseReticle::StartPressed()
{
	if (!isSprint)
	{
		onUpdateCooldownValue.Broadcast(0.0f);
		onShow.Broadcast();
	}
	else if (!isShowed)
		onShow.Broadcast();
}

void UBaseReticle::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (!isSprint && isReloading)
		onUpdateCooldownValue.Broadcast(member->GetCooldownRatio());
}
