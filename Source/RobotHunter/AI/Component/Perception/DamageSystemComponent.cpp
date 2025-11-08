// Fill out your copyright notice in the Description page of Project Settings.

#include "DamageSystemComponent.h"
#include "RobotHunter/AI/AI_Base.h"
#include "RobotHunter/CCC/Character/MainCharacter.h"
#include "RobotHunter/AI/Component/Life/LifeComponent.h"
#include "Components/CapsuleComponent.h"
#include <Net/UnrealNetwork.h>

UDamageSystemComponent::UDamageSystemComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UDamageSystemComponent::InitPerception()
{
	Super::InitPerception();

	weakPoint = owner->GetWeakPoint();
	if (!weakPoint)
	{
		LOG_ERROR(DAMAGE_COMPONENT_USE_LOG, "Failed to get Weakpoint on " + OWNER_NAME);
		return;
	}
	FAttachmentTransformRules _attachmentRules = FAttachmentTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::KeepRelative, EAttachmentRule::KeepWorld, true);
	const bool _attachSucces = weakPoint->AttachToComponent(owner->GetMesh(), _attachmentRules, WEAK_POINT);
	if (!_attachSucces)
	{
		LOG_WARNING(DAMAGE_COMPONENT_USE_LOG, "Failed to attach weakPoit on socket");
	}

	ULifeComponent* _lifeComponent = owner->GetLifeComponent();
	if (!_lifeComponent)
	{
		LOG_ERROR(DAMAGE_COMPONENT_USE_LOG, "Failed to get LifeComponent on " + OWNER_NAME);
		return;
	}
	_lifeComponent->OnLifeChangeByDamage().AddUniqueDynamic(this, &UDamageSystemComponent::TakeDamage);

	isInitializationGood = true;
}

void UDamageSystemComponent::TakeDamage(AActor* _owner, const int _currentLife, const int _damage, AActor* _damageSource)
{
	AMainCharacter* _player = Cast<AMainCharacter>(_damageSource);
	if (_player)
	{
		onDetected.Broadcast(_player);
	}
	else
	{
		/// ?
	}
}
