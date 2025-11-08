// Fill out your copyright notice in the Description page of Project Settings.


#include "CamouflageComponent.h"

UCamouflageComponent::UCamouflageComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	protectionStatus = FStatusParams(EStatusType::PROTECT, 100.0f, -1.0f);
	stunResistantStatus = FStatusParams(EStatusType::RES_MODIF, 100.0f, -1.0f, EStatusType::STUN);
	staggerResistantStatus = FStatusParams(EStatusType::RES_MODIF, 100.0f, -1.0f, EStatusType::STAGGER);
}

void UCamouflageComponent::BeginPlay()
{
	Super::BeginPlay();

	ULifebarWidgetComponent* _lifeBar = GetOwner()->GetComponentByClass<ULifebarWidgetComponent>();

	if (_lifeBar)
		lifeBar = _lifeBar;

	statusComp = GetOwner()->GetComponentByClass<UStatusComponent>();
	movementComponent = GetOwner()->GetComponentByClass<UAIMovementComponent>();
}

void UCamouflageComponent::TransformMode()
{
	if (lifeBar)
		lifeBar->SetHiddenInGame(true);
	
	if (statusComp)
	{
		statusComp->AddStatus(GetOwner(), protectionStatus);
		statusComp->AddStatus(GetOwner(), stunResistantStatus);
		statusComp->AddStatus(GetOwner(), staggerResistantStatus);
	}

	movementComponent->SetCurrentSpeedRun(speedCamouflage);

	onTransform.Broadcast();
}

void UCamouflageComponent::DisableMesh()
{
	if (lifeBar)
		lifeBar->SetHiddenInGame(false);


	if (statusComp)
	{
		statusComp->RemoveStatus(GetOwner(), protectionStatus.type);
		statusComp->RemoveStatus(GetOwner(), stunResistantStatus.type);
		statusComp->RemoveStatus(GetOwner(), staggerResistantStatus.type);
	}

	movementComponent->ResetWalkSpeed();
	onDisableTransform.Broadcast();
}

void UCamouflageComponent::UpdateCamouflageStatusValue(const EStatusType& _type, const float _value)
{
	FStatusParams _empty = FStatusParams();
	FStatusParams& _status = protectionStatus.type == _type ? protectionStatus
		: stunResistantStatus.type == _type ? stunResistantStatus
		: staggerResistantStatus.type == _type ? staggerResistantStatus
		: _empty;

	if (_status.type == EStatusType::DEFAULT_STATUS)
		return;

	_status.value = _value;

	if (statusComp)
		statusComp->RefreshStatus(GetOwner(), _status, false);
}

