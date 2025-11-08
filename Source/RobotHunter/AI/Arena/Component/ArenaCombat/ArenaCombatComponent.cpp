// Fill out your copyright notice in the Description page of Project Settings.

#include "ArenaCombatComponent.h"
#include "RobotHunter/AI/Arena/Arena.h"
#include "RobotHunter/AI/Arena/Component/AIRangePosition/AIRangePositionComponent.h"
#include "RobotHunter/AI/AI_Base.h"
#include "RobotHunter/CCC/Character/MainCharacter.h"
#include "RobotHunter/Game/CustomGameMode.h"
#include "RobotHunter/Game/CustomGameState.h"
#include "GameFramework/PlayerState.h"
#include "RobotHunter/Game/GameMode/PrincipalGameMode.h"
#include "RobotHunter/AI/Arena/AIsAttackDataAsset.h"
#include "RobotHunter/Game/GPE/Nest/AlienDoor.h"

UArenaCombatComponent::UArenaCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

#if WITH_EDITOR
	PrimaryComponentTick.bStartWithTickEnabled = true;
#endif
}

void UArenaCombatComponent::InitArenaCombat()
{
	/// Check arena availability
	AArena* _owner = Cast<AArena>(GetOwner());
	if (!_owner)
	{
		LOG_ERROR(COMBAT_COMPONENT_USE_LOG, "Failed to cast owner in AArena on " + OWNER_NAME);
		return;
	}
	owner = _owner;

	trackingSphere = owner->GetSphere();
	if (!trackingSphere)
	{
		LOG_ERROR(COMBAT_COMPONENT_USE_LOG, "Failed to get sphere of " + OWNER_NAME + " in ArenaCombatComponent");
		return;
	}
	owner->OnFinishToCollectAI().AddUniqueDynamic(this, &UArenaCombatComponent::SetTrackingSphere);

	/// Check AIsAttack availability
	if (!AIsAttack)
	{
		LOG_ERROR(COMBAT_COMPONENT_USE_LOG, "The AIsAttack in ArenaCombatComponent of " + OWNER_NAME + " is null");
		return;
	}
	if (!AIsAttack->CheckAvailability())
	{
		LOG_ERROR(COMBAT_COMPONENT_USE_LOG, "The AIsAttack in ArenaCombatComponent of " + OWNER_NAME + " is invalid. Check it");
		return;
	}
	/// Check Copy
	AIsAttackInstance = DuplicateObject<UAIsAttackDataAsset>(AIsAttack, this);
	if (!AIsAttackInstance)
	{
		LOG_ERROR(COMBAT_COMPONENT_USE_LOG, "Failed to copy AIsAttack in ArenaCombatComponent of " + OWNER_NAME);
		return;
	}

	/// Set properties
	onAlert = false;
	trackingRange = owner->GetBreakRange() + additionalRangeForTrackingPlayer;

	/// Get player in game host side
	owner->GetArenaManager()->OnArenaManagerReady().AddUniqueDynamic(this, &UArenaCombatComponent::SetListPlayers);

	isInitializationGood = true;
}

void UArenaCombatComponent::GetShortAttacksForAI(const EAIType& _AIType, TArray<FAICQCAttack>& _listShortAttack, int& _indexCQCAttack)
{
	_listShortAttack = AIsAttackInstance->GetListShortRangeAttack();

	const int _listShortAttackCount = _listShortAttack.Num();
	for (int i = _listShortAttackCount - 1; i >= 0; i--)
	{
		/// Attack unavailable
		if (!_listShortAttack[i].listAIsAllowed.Contains(_AIType) || _listShortAttack[i].token <= 0)
		{
			_listShortAttack.RemoveAt(i);
			LOG_COLOR(COMBAT_COMPONENT_USE_LOG, "Remove attack", CYAN);
		}
		else
		{
			//LOG_COLOR(true, "INDEX => " + FString::FromInt(i), MAGENTA);
			//LOG_COLOR(true, "TOKEN => " + FString::FromInt(_listShortAttack[i].token), PINK);
			_indexCQCAttack = i;
		}
	}
}

TArray<FAIDistanceAttack> UArenaCombatComponent::GetDistanceAttacksForAI(const EAIType& _AIType, const ERangeAttack& _range)
{
	TArray<FAIDistanceAttack> _listDistanceAttack;
	switch (_range)
	{
	case ERangeAttack::Short:
		break;
	case ERangeAttack::Mid:
		_listDistanceAttack = AIsAttackInstance->GetListMidRangeAttack();
		break;
	case ERangeAttack::Long:
		_listDistanceAttack = AIsAttackInstance->GetListLongRangeAttack();
		break;
	}

	const int _listDistanceAttackCount = _listDistanceAttack.Num();
	for (int i = _listDistanceAttackCount - 1; i >= 0; i--)
	{
		/// Attack unavailable
		if (!_listDistanceAttack[i].listAIsAllowed.Contains(_AIType) || _listDistanceAttack[i].token <= 0)
			_listDistanceAttack.RemoveAt(i);
	}

	return _listDistanceAttack;
}

void UArenaCombatComponent::GetTokenOnCQCAttack(const int _index, const FAICQCAttack& _CQCData)
{
	//LOG_COLOR(true, "Get CQC Token at index " + FString::FromInt(_index), GREEN);

	AIsAttackInstance->GetToken(ERangeAttack::Short, _index);
	float _delay = _CQCData.generationDelay + RANDOM_FLOAT_RANGE(-_CQCData.randomDeviation, _CQCData.randomDeviation);
	StartGenerateAttackTokenTimer(ERangeAttack::Short, _index, _delay);
}

void UArenaCombatComponent::GetTokenOnDistanceAttack(const ERangeAttack& _range, const int _index, const FAIDistanceAttack& _distanceData)
{
	AIsAttackInstance->GetToken(_range, _index);
	float _delay = _distanceData.generationDelay + RANDOM_FLOAT_RANGE(-_distanceData.randomDeviation, _distanceData.randomDeviation);
	StartGenerateAttackTokenTimer(_range, _index, _delay);
}

void UArenaCombatComponent::StartGenerateAttackTokenTimer(const ERangeAttack& _range, const int _index, const float _delay)
{
	FTimerHandle _timerGenerateAttackToken;
	FTimerDelegate _DelegateGenerateAttackToken;
	_DelegateGenerateAttackToken.BindUFunction(AIsAttackInstance, FName("GenerateToken"), _range, _index);
	GetWorld()->GetTimerManager().SetTimer(_timerGenerateAttackToken, _DelegateGenerateAttackToken, _delay, false);
}

void UArenaCombatComponent::SetListPlayers()
{
	/// No networking
	ACharacter* _player = GetWorld()->GetFirstPlayerController()->GetCharacter();
	if (!_player)
	{
		LOG_ERROR(COMBAT_COMPONENT_USE_LOG, "Failed to get FirstPlayerController Character");
		return;
	}
	FUniqueNetIdRepl _playerId = _player->GetPlayerState()->GetUniqueId();
	if (!_playerId.IsValid())
	{
		LOG_ERROR(COMBAT_COMPONENT_USE_LOG, "Failed to get player's UniqueNetId");
		return;
	}
	listPlayersId.Add(_playerId);
	mapAIsOnPlayer.Add(_playerId, FAIsOnPlayer());

	LOG_COLOR(COMBAT_COMPONENT_USE_LOG, "Players set on arenaCombat of " + OWNER_NAME, BLUE);

	onFinishInitialization.Broadcast();
}

void UArenaCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	DrawDebug();
}

bool UArenaCombatComponent::ShouldTickIfViewportsOnly() const
{
	return true;
}

AMainCharacter* UArenaCombatComponent::GetCharacterByNetId(FUniqueNetIdRepl _id)
{
	TArray<APlayerState*> _listPlayerState = GetWorld()->GetGameState()->PlayerArray;
	const int _listPlayerStateCount = _listPlayerState.Num();
	for (int i = 0; i < _listPlayerStateCount; i++)
	{
		APlayerState* _playerState = _listPlayerState[i];
		if (_playerState->GetUniqueId() == _id)
			return Cast<AMainCharacter>(_playerState->GetPawn());
	}
	return nullptr;
}

void UArenaCombatComponent::ArenaAlert()
{
	if (!isInitializationGood || onAlert || listPlayerInRange.IsEmpty())
		return;

	LOG_COLOR(COMBAT_COMPONENT_USE_LOG, "Alert on " + OWNER_NAME, RED);
	onAlert = true;

	const int _listPlayerInRangeCount = listPlayerInRange.Num();
	for (int i = 0; i < _listPlayerInRangeCount; i++)
	{
		listPlayerInRange[i]->PlayerIsAttacked();
	}
	owner->FillMissingAIs();

	/// Alert all AIs in arena
	TMap<uint32, AAI_Base*> _mapAIs = owner->GetMapAIs();
	for (TPair<uint32, AAI_Base*>& _element : _mapAIs)
	{
		if (!_element.Value)
		{
			LOG_WARNING(COMBAT_COMPONENT_USE_LOG, "Failed to get AI in ArenaCombatComponent of " + OWNER_NAME);
			continue;
		}

		AlertAI(_element.Value);
	}
}

void UArenaCombatComponent::AlertAI(AAI_Base* _AI)
{
	/// Warn AI to start to fight
	FUniqueNetIdRepl _playerId = _AI->StartToFight();

	/// Update AI's playerThreat
	//UpdateAIsOnPlayer(_playerId, _AI, true);
}

void UArenaCombatComponent::AIDie(AAI_Base* _AI)
{
	/// Update AI's playerThreat 
	//AMainCharacter* _target = _AI->GetTrackingSystemComponent()->GetTarget();
	//if (!_target)
	//{
	//	LOG_WARNING(COMBAT_COMPONENT_USE_LOG, "Failed to get target of " + _AI->ACTOR_NAME);
	//}
	//else
	//{
	//	FUniqueNetIdRepl _targetId = _target->GetPlayerState()->GetUniqueId();
	//	UpdateAIsOnPlayer(_targetId, _AI, false);
	//}
}

void UArenaCombatComponent::UpdateAIsOnPlayer(FUniqueNetIdRepl _playerId, AAI_Base* _AI, const bool _add)
{
	//if (!mapAIsOnPlayer.Contains(_playerId))
	//{
	//	LOG_ERROR(COMBAT_COMPONENT_USE_LOG, "Player NetId is not register in mapAIsOnPlayer of " + OWNER_NAME);
	//	return;
	//}
	//if (!_AI)
	//{
	//	LOG_ERROR(COMBAT_COMPONENT_USE_LOG, "AI is null to update AIs on player of " + OWNER_NAME);
	//	return;
	//}

	//mapAIsOnPlayer[_playerId].Update(_AI->GetType(), _add);
	//TArray<FUniqueNetIdRepl> _listPlayersId;
	//TArray<FAIsOnPlayer> _listAIsOnPlayer;
	//mapAIsOnPlayer.GenerateKeyArray(_listPlayersId);
	//mapAIsOnPlayer.GenerateValueArray(_listAIsOnPlayer);
	//onUpdateAIsOnPlayer.Broadcast(_listPlayersId, _listAIsOnPlayer);
}

void UArenaCombatComponent::OnAIChangeTarget(FUniqueNetIdRepl _oldTargetId, FUniqueNetIdRepl _newTargetId, AAI_Base* _AI)
{
	//if (mapAIsOnPlayer.Contains(_oldTargetId))
	//	UpdateAIsOnPlayer(_oldTargetId, _AI, false);
	//if (mapAIsOnPlayer.Contains(_newTargetId))
	//	UpdateAIsOnPlayer(_newTargetId, _AI, true);
}

void UArenaCombatComponent::SetTrackingSphere()
{
	trackingSphere->OnComponentBeginOverlap.AddUniqueDynamic(this, &UArenaCombatComponent::TrackingSphereBeginOverlap);
	trackingSphere->OnComponentEndOverlap.AddUniqueDynamic(this, &UArenaCombatComponent::TrackingSphereEndOverlap);

	trackingSphere->SetGenerateOverlapEvents(true);
	trackingSphere->SetSphereRadius(trackingRange, false);
	trackingSphere->SetCollisionProfileName(sphereCollisionProfilePlayer.Name, true);
}

void UArenaCombatComponent::TrackingSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!isInitializationGood)
		return;

	/// New player detected
	AMainCharacter* _player = Cast<AMainCharacter>(OtherActor);
	if (_player && !listPlayerInRange.Contains(_player))
	{
		LOG_COLOR(COMBAT_COMPONENT_USE_LOG, _player->ACTOR_NAME + " enter " + OWNER_NAME, CYAN);

		_player->SetCurrentArena(owner);

		listPlayerInRange.Add(_player);
		APlayerState* _playerState = _player->GetPlayerState();
		if (!_playerState)
			return;
		FUniqueNetIdRepl _playerNetId = _playerState->GetUniqueId();
		onPlayerEnterArena.Broadcast(_playerNetId);
	}
}

void UArenaCombatComponent::TrackingSphereEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (!isInitializationGood)
		return;

	/// Know player
	AMainCharacter* _player = Cast<AMainCharacter>(OtherActor);
	if (_player && listPlayerInRange.Contains(_player))
	{
		LOG_COLOR(COMBAT_COMPONENT_USE_LOG, _player->ACTOR_NAME + " exit " + OWNER_NAME, CYAN);

		AAlienDoor* _exitDoor = owner->GetExitGate();
		if (_exitDoor)
			_exitDoor->HideWidget();

		_player->SetCurrentArena(nullptr);

		listPlayerInRange.Remove(_player);
		APlayerState* _playerState = _player->GetPlayerState();
		if (!_playerState)
			return;
		FUniqueNetIdRepl _playerNetId = _playerState->GetUniqueId();
		onPlayerExitArena.Broadcast(_playerNetId);

		/// Inform all AIs
		TMap<uint32, AAI_Base*> _mapAIs = owner->GetMapAIs();
		for (TPair<uint32, AAI_Base*>& _element : _mapAIs)
		{
			if (!_element.Value)
			{
				LOG_WARNING(COMBAT_COMPONENT_USE_LOG, "Failed to get AI in ArenaCombatComponent of " + OWNER_NAME);
				continue;
			}

			/// Update target or lose it
			if (onAlert)
			{
				if (listPlayerInRange.IsEmpty())
				{
					AMainCharacter* _target = _element.Value->GetTrackingSystemComponent()->GetTarget();
					if (!_target)
						return;
					FUniqueNetIdRepl _targetId = _target->GetPlayerState()->GetUniqueId();
					_element.Value->GetTrackingSystemComponent()->LoseTarget();
					_element.Value->GetAttackComponent()->ResetAttack();
				}
			}
		}

		/// Disable alert if no more player in range
		if (listPlayerInRange.IsEmpty())
		{
			const int _listPlayersCount = listPlayersId.Num();
			for (int i = 0; i < _listPlayersCount; i++)
			{
				/// Get MainCharacter by his NetId
				_playerNetId = listPlayersId[i];
				mapAIsOnPlayer[_playerNetId].Empty();
			}

			onAlert = false;
		}
	}
}

void UArenaCombatComponent::DrawDebug()
{
	if (!drawDebug)
		return;

	// Draw additionalRangeForTrackingPlayer circle
	AArena* _owner = Cast<AArena>(GetOwner());
	if (_owner)
	{
		const int _range = _owner->GetBreakRange() + additionalRangeForTrackingPlayer;
		DRAW_CIRCLE(_owner->GetActorLocation(), _range, debugColor, FVector::ForwardVector, FVector::RightVector);
	}
}

