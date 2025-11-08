#include "FreezingStatus.h"
#include "RobotHunter/Utils/DebugUtils.h"
#include "RobotHunter/CCC/Character/Components/Status/StatusComponent.h"
#include "RobotHunter/CCC/Character/MainCharacter.h"
#include "RobotHunter/AI/AI_Base.h"
#include "RobotHunter/Game/CustomGameState.h"

UFreezingStatus::UFreezingStatus() : UFreezingStatus(FStatusParams(EStatusType::DEFAULT_STATUS, 1.0f, 10, 0.1f)) {}

UFreezingStatus::UFreezingStatus(const FStatusParams& _params) : Super(_params)
{
	params.currentStack = 1;
	params.slowType = ESpeedModifiers::COLD;

	useMuliplierWhenAddingStack = false;
	stackDurationMultiplier = 1.0f;

	hasSpeedModifier = false;
	speedModifier = 0.0f;

	stunnedStatus = FStatusParams();
	protectedStatus = FStatusParams();
	resistantStatus = FStatusParams();
}

void UFreezingStatus::SlowAI(AAI_Base* _ai)
{
	if (_ai)
	{
		if (!hasSpeedModifier)
		{
			ACustomGameState* _gameState = Cast<ACustomGameState>(GetWorld()->GetGameState());

			if (_gameState)
			{
				speedModifier = _gameState->GetSpeedModifiers()[params.slowType] / 100.0f;
				hasSpeedModifier = true;
			}
		}
		
		UAIMovementComponent* _moveComp = _ai->GetAIMovementComponent();

		if (_moveComp)
			_moveComp->SetSpeedMultiplier(speedModifier, (params.currentStack / params.maxStack) * 100.0f);
	}
}

void UFreezingStatus::StackBehaviour()
{
	if (!canRemoveStack)
		AddStack();

	if (params.currentStack == params.maxStack)
	{
		MaxStackBehaviour();
		return;
	}

	Super::StackBehaviour();
}

void UFreezingStatus::MaxStackBehaviour()
{
	StatusEnded();

	if (statusComponent)
	{
		statusComponent->AddStatus(source, stunnedStatus);
		statusComponent->AddStatus(source, protectedStatus);
		statusComponent->AddStatus(source, resistantStatus);
	}
}

void UFreezingStatus::ApplyStatus(AActor* _actor)
{
	Super::ApplyStatus(_actor);

	if (_actor)
	{
		LOG_COLOR_TIME(FREEZING_STATUS_USE_LOG, _actor->GetName() + " [Freeze][Value * Stack] " +
			FString::SanitizeFloat(params.value) + " * " + FString::FromInt(params.currentStack)
			+ " = " + FString::SanitizeFloat(GetValue()), FColor::Red, 0.5f);

		if (_actor->GetClass()->IsChildOf<AMainCharacter>())
		{
			AMainCharacter* _mc = Cast<AMainCharacter>(_actor);

			if (_mc)
				_mc->ServerRpc_SetCurrentSpeedModifier(params.slowType, (params.currentStack / params.maxStack) * 100.0f);
		}

		if (_actor->GetClass()->IsChildOf<AAI_Base>())
		{
			AAI_Base* _ai = Cast<AAI_Base>(_actor);

			if (_ai)
				SlowAI(_ai);
		}
	}
}

void UFreezingStatus::UnapplyStatus(AActor* _actor)
{
	if (_actor)
	{
		if (_actor->GetClass()->IsChildOf<AMainCharacter>())
		{
			AMainCharacter* _mc = Cast<AMainCharacter>(_actor);

			if (_mc)
				_mc->ServerRpc_ResetCurrentSpeedModifier();
		}

		if (_actor->GetClass()->IsChildOf<AAI_Base>())
		{
			AAI_Base* _ai = Cast<AAI_Base>(_actor);

			if (_ai)
			{
				UAIMovementComponent* _moveComp = _ai->GetAIMovementComponent();

				if (_moveComp)
					_moveComp->ResetSpeedMultiplier();
			}
		}
	}

	Super::UnapplyStatus(_actor);
}

void UFreezingStatus::StartStatusTimer(const float _duration)
{
	const float _timerDuration = _duration == -1.0f ? params.stackDuration * stackDurationMultiplier : _duration;
	Super::StartStatusTimer(_timerDuration);
}

void UFreezingStatus::RefreshStatus(const bool _sourceCooldownPhase, AActor* _actor)
{
	float _timerDuration = 0.0f;
		
	if (!_sourceCooldownPhase)
	{
		if (!canRemoveStack)
			return;

		canRemoveStack = false;
		_timerDuration = useMuliplierWhenAddingStack ? params.stackDuration / stackDurationMultiplier : params.stackDuration;

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
		_timerDuration = params.stackDuration * stackDurationMultiplier;
	}

	ClearStatusTimer();
	StartStatusTimer(_timerDuration);
}

float UFreezingStatus::GetValue() const
{
	return params.value * params.currentStack;
}
