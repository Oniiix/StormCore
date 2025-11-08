#include "Status.h"
#include "RobotHunter/CCC/Character/Components/Status/StatusComponent.h"
#include "RobotHunter/Utils/DebugUtils.h"

UStatus::UStatus() : UStatus(FStatusParams(EStatusType::DEFAULT_STATUS, 0.1f, -1.0f)) {}

UStatus::UStatus(const FStatusParams& _params)
{
	statusComponent = nullptr;
	source = nullptr;

	isApplied = false;
	params = _params;
	statusTimer = FTimerHandle();
}

void UStatus::StatusEnded()
{
	if (!statusComponent)
		return;

	statusComponent->RemoveStatus(source, this);
}

void UStatus::ClearStatusTimer()
{
	FTimerManager& _tm = GetWorld()->GetTimerManager();

	if (_tm.IsTimerActive(statusTimer))
		_tm.ClearTimer(statusTimer);
}

bool UStatus::CheckIfCanBeApplied(const float _maxValue)
{
	if (_maxValue == -2.0f && statusComponent)
		isApplied = GetValue() >= statusComponent->GetStatusTypeMaxValue(params.type, params.resistanceType);
	else if (_maxValue > 0.0f)
		isApplied = GetValue() >= _maxValue;
	else
		isApplied = false;

	return isApplied;
}

void UStatus::ApplyStatus(AActor* _actor)
{
	if (!isApplied)
		return;
}

void UStatus::UnapplyStatus(AActor* _actor)
{
	isApplied = false;
}

void UStatus::RefreshStatus(const bool _sourceCooldownPhase, AActor* _actor)
{
	if (params.duration <= 0.0f)
		return;

	FTimerManager& _tm = GetWorld()->GetTimerManager();
	_tm.ClearTimer(statusTimer);
	_tm.SetTimer(statusTimer, this, &UStatus::StatusEnded, params.duration, false);
}

void UStatus::StartStatusTimer(const float _duration)
{
	const float _timerDuration = _duration == -1.0f ? params.duration : _duration;

	if (_timerDuration <= 0.0f)
		return;

	GetWorld()->GetTimerManager().SetTimer(statusTimer, this, &UStatus::StatusEnded, _timerDuration, false);
}

float UStatus::GetValue() const
{
	return params.value;
}
