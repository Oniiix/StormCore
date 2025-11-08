#include "StackingStatus.h"
#include "RobotHunter/CCC/Character/Components/Status/StatusComponent.h"
#include "RobotHunter/Utils/DebugUtils.h"

UStackingStatus::UStackingStatus() : UStackingStatus(FStatusParams(EStatusType::DEFAULT_STATUS, 1.0f, 1, 0.1f)) {}

UStackingStatus::UStackingStatus(const FStatusParams& _params) : Super(_params)
{
	params.currentStack = params.maxStack;

	canRemoveStack = true;
}

void UStackingStatus::AddStack()
{
	const int _newStack = params.currentStack + 1;
	params.currentStack = _newStack < params.maxStack ? _newStack : params.maxStack;
}

void UStackingStatus::RemoveStack()
{
	const int _newStack = params.currentStack - 1;
	params.currentStack = _newStack > 0 ? _newStack : 0;
}

void UStackingStatus::StackBehaviour()
{
	if (canRemoveStack)
		RemoveStack();

	if (params.currentStack == 0)
	{
		StatusEnded();
		return;
	}

	if (isApplied && statusComponent)
		ApplyStatus(statusComponent->GetOwner());

	if (!canRemoveStack)
		canRemoveStack = true;

	StartStatusTimer();
}

void UStackingStatus::RefreshStatus(const bool _sourceCooldownPhase, AActor* _actor)
{
	float _timerDuration = 0.0f;

	if (!_sourceCooldownPhase)
	{
		if (!canRemoveStack)
			return;

		canRemoveStack = false;

		FTimerManager& _tm = GetWorld()->GetTimerManager();

		if (_tm.IsTimerActive(statusTimer))
		{
			_timerDuration -= _tm.GetTimerElapsed(statusTimer);
			_timerDuration = _timerDuration <= 0.0f ? 0.1f : _timerDuration;
		}
	}
	else
	{
		canRemoveStack = true;
		_timerDuration = params.stackDuration;
	}

	ClearStatusTimer();
	StartStatusTimer(_timerDuration);
}

void UStackingStatus::StartStatusTimer(const float _duration)
{
	const float _timerDuration = _duration == -1.0f ? params.stackDuration : _duration;

	if (_timerDuration <= 0.0f)
		return;

	GetWorld()->GetTimerManager().SetTimer(statusTimer, this, &UStackingStatus::StackBehaviour, _timerDuration, false);
}