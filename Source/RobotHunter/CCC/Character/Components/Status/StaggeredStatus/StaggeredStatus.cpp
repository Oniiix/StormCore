#include "StaggeredStatus.h"
#include "RobotHunter/CCC/Character/MainCharacter.h"
#include "RobotHunter/AI/AI_Base.h"
#include "Kismet/KismetMathLibrary.h"

UStaggeredStatus::UStaggeredStatus()
{
	actorToMove = nullptr;
	valueMoved = 0.0f;
	moveTimer = FTimerHandle();
}

void UStaggeredStatus::MoveActor()
{
	if (actorToMove)
	{
		const float _valueToMove = params.value / (params.duration * 10.0f);
		const FVector _direction = UKismetMathLibrary::GetDirectionUnitVector(source->GetActorLocation(), actorToMove->GetActorLocation());

		actorToMove->AddActorWorldOffset(_direction * _valueToMove);
		valueMoved += _valueToMove;
		LOG_COLOR_TIME(true, FString::SanitizeFloat(valueMoved), FColor::Red, 0.1f);

		if (valueMoved >= params.value)
		{
			GetWorld()->GetTimerManager().ClearTimer(moveTimer);
			actorToMove = nullptr;
		}
	}
}

void UStaggeredStatus::ApplyStatus(AActor* _actor)
{
	Super::ApplyStatus(_actor);

	if (_actor)
	{
		FTimerManager& _tm = GetWorld()->GetTimerManager();

		if (_tm.IsTimerActive(statusTimer))
			return;

		float _duration = params.duration;
		bool _launchTimer = false;

		if (_actor->GetClass()->IsChildOf<AMainCharacter>())
		{
			AMainCharacter* _mc = Cast<AMainCharacter>(_actor);

			if (_mc)
			{
				_mc->Stun(_duration);
				actorToMove = _mc;
				_launchTimer = true;
			}
		}

		if (_actor->GetClass()->IsChildOf<AAI_Base>())
		{
			AAI_Base* _ai = Cast<AAI_Base>(_actor);

			if (_ai)
			{
				_ai->StunAI(_duration);
				actorToMove = _ai;
				_launchTimer = true;
			}
		}

		if (_launchTimer)
			_tm.SetTimer(moveTimer, this, &UStaggeredStatus::MoveActor, 0.1f, true);
	}
}

void UStaggeredStatus::UnapplyStatus(AActor* _actor)
{
	FTimerManager& _tm = GetWorld()->GetTimerManager();

	if (_tm.IsTimerActive(moveTimer))
	{
		_tm.ClearTimer(moveTimer);
		actorToMove = nullptr;
	}

	Super::UnapplyStatus(_actor);
}

void UStaggeredStatus::RefreshStatus(const bool _sourceCooldownPhase, AActor* _actor) { /*no refresh*/ }

float UStaggeredStatus::GetValue() const
{
	return 0.1f;
}
