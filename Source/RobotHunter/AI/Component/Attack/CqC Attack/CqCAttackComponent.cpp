#include "CqCAttackComponent.h"
#include "GameFramework/Character.h"
#include <Components/BoxComponent.h>
#include <Components/SphereComponent.h>
#include <Components/CapsuleComponent.h>
#include "RobotHunter/Utils/DebugUtils.h"

UCqCAttackComponent::UCqCAttackComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

TObjectPtr<UShapeComponent> UCqCAttackComponent::GetBoxCQC()
{
	if (!trigger)
		return trigger = CREATE_NEWOBJECT(this, UBoxComponent, "HitBox");
	return trigger;
}

void UCqCAttackComponent::Change()
{
	if (!trigger)
	{
		switch (shapeType)
		{
		case EShapeType::SHAPE_NONE:
			return;
		case EShapeType::BOX:
			trigger = CREATE_NEWOBJECT(this, UBoxComponent, "HitBox");
			break;
		case EShapeType::SPHERE:
			trigger = CREATE_NEWOBJECT(this, USphereComponent, "HitSphere");
			break;
		case EShapeType::CAPSULE:
			trigger = CREATE_NEWOBJECT(this, UCapsuleComponent, "HitCapsule");
			break;
		}

		ACharacter* _owner = Cast<ACharacter>(GetOwner());

		if (_owner)
		{
			FAttachmentTransformRules AttachmentRules = FAttachmentTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::KeepRelative, EAttachmentRule::KeepWorld, true);
			trigger->AttachToComponent(_owner->GetMesh(), AttachmentRules, CQC_SOCKET);
		}

		trigger->SetFlags(RF_Public | RF_Transactional);
		GetOwner()->AddInstanceComponent(trigger);
		GetOwner()->AddOwnedComponent(trigger);
		trigger->RegisterComponent();
		trigger->SetCollisionProfileName(FName("IgnoreAll"), true);
		trigger->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;
		trigger->SetGenerateOverlapEvents(false);
	}
}

#if WITH_EDITOR
void UCqCAttackComponent::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if (PropertyChangedEvent.GetPropertyName() == GET_MEMBER_NAME_CHECKED(UCqCAttackComponent, shapeType))
	{
		if (trigger)
		{
			trigger->DestroyComponent();
			trigger = nullptr;
		}

		Change();
		Modify();
	}
}
#endif
