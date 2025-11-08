#include "SlowedStatus.h"
#include "RobotHunter/CCC/Character/MainCharacter.h"
#include "RobotHunter/AI/AI_Base.h"
#include "RobotHunter/Game/CustomGameState.h"

USlowedStatus::USlowedStatus() : Super(params)
{
	hasSpeedModifier = false;
	speedModifier = 0.0f;
}


void USlowedStatus::CheckAndSlowAI(AActor* _actor)
{
	if (_actor)
	{
		if (_actor->GetClass()->IsChildOf<AAI_Base>())
		{
			AAI_Base* _ai = Cast<AAI_Base>(_actor);

			if (_ai)
				SlowAI(_ai);
		}
	}
}

void USlowedStatus::SlowAI(AAI_Base* _ai)
{
	if (_ai)
	{
		if (params.useSlowType && !hasSpeedModifier)
		{
			ACustomGameState* _gameState = Cast<ACustomGameState>(GetWorld()->GetGameState());

			if (_gameState)
			{
				speedModifier = _gameState->GetSpeedModifiers()[params.slowType] / 100.0f;
				hasSpeedModifier = true;
			}
		}
		else if (!params.useSlowType)
			speedModifier = (params.value / 100.0f);

		UAIMovementComponent* _moveComp = _ai->GetAIMovementComponent();

		if (_moveComp)
			_moveComp->SetSpeedMultiplier(speedModifier, 100.0f);
	}
}


void USlowedStatus::ApplyStatus(AActor* _actor)
{
	Super::ApplyStatus(_actor);
	
	if (_actor)
	{
		if (_actor->GetClass()->IsChildOf<AMainCharacter>())
		{
			AMainCharacter* _mc = Cast<AMainCharacter>(_actor);

			if (_mc)
				_mc->ServerRpc_SetCurrentSpeedModifier(params.slowType, 100.0f);
		}

		CheckAndSlowAI(_actor);
	}
}

void USlowedStatus::UnapplyStatus(AActor* _actor)
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


void USlowedStatus::RefreshStatus(const bool _sourceCooldownPhase, AActor* _actor)
{
	CheckAndSlowAI(_actor);
	Super::RefreshStatus(_sourceCooldownPhase, _actor);
}
