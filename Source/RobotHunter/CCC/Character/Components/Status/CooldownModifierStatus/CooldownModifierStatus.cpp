#include "CooldownModifierStatus.h"
#include "RobotHunter/CCC/Character/MainCharacter.h"

UCooldownModifierStatus::UCooldownModifierStatus() : Super()
{
	params.currentStack = 1;
}

UCooldownModifierStatus::UCooldownModifierStatus(const FStatusParams& _params) : Super(_params)
{
	params.currentStack = 1;
}

void UCooldownModifierStatus::RefreshStatus(const bool _sourceCooldownPhase, AActor* _actor)
{
	if (_sourceCooldownPhase)
		RemoveStack();
	else
		AddStack();

	if (isApplied && statusComponent)
		ApplyStatus(statusComponent->GetOwner());
}

void UCooldownModifierStatus::ApplyStatus(AActor* _actor)
{
	Super::ApplyStatus(_actor); 
	
	if (_actor)
	{
		if (_actor->GetClass()->IsChildOf<AMainCharacter>())
		{
			AMainCharacter* _mc = Cast<AMainCharacter>(_actor);

			if (_mc)
			{
				TArray<AMembersActor*> _members = _mc->GetUsingMembers();

				if (_members.IsEmpty())
					return;

				const int _num = _members.Num();
				AMembersActor* _member = nullptr;

				for (size_t i = 0; i < _num; i++)
				{
					_member = _members[i];

					if (_member)
						_member->ModifyCooldownMultiplier(params.value * params.currentStack);
				}

				LOG_COLOR_TIME(CD_MODIF_USE_LOG, "[CD MODIF] => " + FString::SanitizeFloat(params.value)
					+ " * " + FString::SanitizeFloat(params.currentStack)
					+ " = " + FString::SanitizeFloat(params.value * params.currentStack),
					FColor::Green, 1.0f);
			}
		}
	}
}
