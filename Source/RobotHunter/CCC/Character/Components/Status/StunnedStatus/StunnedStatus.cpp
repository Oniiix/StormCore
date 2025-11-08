#include "StunnedStatus.h"
#include "RobotHunter/CCC/Character/MainCharacter.h"
#include "RobotHunter/AI/AI_Base.h"

void UStunnedStatus::ApplyStatus(AActor* _actor)
{
	Super::ApplyStatus(_actor);

	if (_actor)
	{
		float _duration = params.duration;

		FTimerManager& _tm = GetWorld()->GetTimerManager();

		if (_tm.IsTimerActive(statusTimer))
			_duration -= _tm.GetTimerElapsed(statusTimer);

		if (_actor->GetClass()->IsChildOf<AMainCharacter>())
		{
			AMainCharacter* _mc = Cast<AMainCharacter>(_actor);

			if (_mc)
				_mc->Stun(_duration);
		}
		
		if (_actor->GetClass()->IsChildOf<AAI_Base>())
		{
			AAI_Base* _ai = Cast<AAI_Base>(_actor);

			if (_ai)
				_ai->StunAI(_duration);
		}
	}
}

float UStunnedStatus::GetValue() const
{
	return 0.1f;
}
