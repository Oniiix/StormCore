#include "DeactivatedStatus.h"
#include "RobotHunter/CCC/Character/MainCharacter.h"

UDeactivatedStatus::UDeactivatedStatus() : Super(params)
{
	deactivatedMember = nullptr;
}

void UDeactivatedStatus::ApplyStatus(AActor* _actor)
{
	if (_actor)
	{
		if (_actor->GetClass()->IsChildOf<AMainCharacter>())
		{
			AMainCharacter* _mc = Cast<AMainCharacter>(_actor);

			if (_mc)
			{
				TArray<AMembersActor*> _members = _mc->GetUsingMembers();

				if (_members.IsEmpty())
				{
					isApplied = false;
					return;
				}

				int _num = _members.Num();

				TArray<AMembersActor*> _activeMembers = TArray<AMembersActor*>();
				AMembersActor* _member = nullptr;

				for (size_t i = 0; i < _num; i++)
				{
					_member = _members[i];

					if (_member)
						if (!_member->GetHasDeactivatedStatus())
							_activeMembers.Add(_member);
				}

				if (_activeMembers.IsEmpty())
				{
					isApplied = false;
					return;
				}

				_num = _activeMembers.Num();
				const int _randIndex = FMath::RandRange(0, _num - 1);
				_member = _activeMembers[_randIndex];

				_member->SetHasDeactivatedStatus(true);
				deactivatedMember = _member;
				LOG_COLOR_TIME(DEACTIVATED_STATUS_USE_LOG, "Deactivated: " + deactivatedMember->GetName(), FColor::Red, 1.0f);
			}
		}
	}
}

void UDeactivatedStatus::UnapplyStatus(AActor* _actor)
{
	if (deactivatedMember)
	{
		deactivatedMember->SetHasDeactivatedStatus(false);
		LOG_COLOR_TIME(DEACTIVATED_STATUS_USE_LOG, "Reactivated: " + deactivatedMember->GetName(), FColor::Green, 1.0f);
		deactivatedMember = nullptr;
	}

	Super::UnapplyStatus(_actor);
}

float UDeactivatedStatus::GetValue() const
{
	return 0.1f;
}
