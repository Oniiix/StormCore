#include "StatusComponent.h"
#include "RobotHunter/CCC/Character/MainCharacter.h"
#include "RobotHunter/CCC/Character/Components/Status/SlowedStatus/SlowedStatus.h"
#include "RobotHunter/CCC/Character/Components/Status/ResistantStatus/ResistantStatus.h"
#include "RobotHunter/CCC/Character/Components/Status/StaggeredStatus/StaggeredStatus.h"
#include "RobotHunter/CCC/Character/Components/Status/DOTStatus/DOTStatus.h"
#include "RobotHunter/CCC/Character/Components/Status/ProtectedStatus/ProtectedStatus.h"
#include "RobotHunter/CCC/Character/Components/Status/StunnedStatus/StunnedStatus.h"
#include "RobotHunter/CCC/Character/Components/Status/CooldownModifierStatus/CooldownModifierStatus.h"
#include "RobotHunter/CCC/Character/Components/Status/DeactivatedStatus/DeactivatedStatus.h"
#include "RobotHunter/CCC/Character/Components/Status/FreezingStatus/FreezingStatus.h"
#include "RobotHunter/CCC/Character/Components/Status/BleedStatus/BleedStatus.h"

UStatusComponent::UStatusComponent()
{
	currentStatus = TMap<TEnumAsByte<EStatusType>, FStatusBySource>();
	currentResistance = TMap<UClass*, UStatus*>();
	statusToAdd = nullptr;
}

void UStatusComponent::AddStatusOfNewType()
{
	if (!statusToAdd)
		return;

	statusToAdd->SetIsApplied(true);
	statusToAdd->ApplyStatus(GetOwner());
	statusToAdd->StartStatusTimer();
	onStatusTypeAdded.Broadcast(statusToAdd->GetType());
	statusToAdd = nullptr;
}

void UStatusComponent::AddStatusOfExistingType()
{
	if (!statusToAdd)
		return;

	const EStatusType _type = statusToAdd->GetType();

	if (_type == EStatusType::DEACTIVATE)
	{
		statusToAdd->SetIsApplied(true);
		statusToAdd->ApplyStatus(GetOwner());
	}
	else
		RefreshStatusAppliedByType(currentStatus[_type].statusBySource, _type, statusToAdd->GetResistanceType());

	statusToAdd->StartStatusTimer();
	statusToAdd = nullptr;
}

void UStatusComponent::RefreshStatusAppliedByType(const TMap<UClass*, UStatus*>& _statusBySource, const EStatusType& _type, 
	const EStatusType& _resistanceType, UStatus* _currentAppliedStatus)
{
	if (_statusBySource.IsEmpty())
	{
		if (_currentAppliedStatus)
			_currentAppliedStatus->UnapplyStatus(GetOwner());

		return;
	}

	const bool _checkResistanceType = _type == EStatusType::RES_MODIF;
	const float _maxValue = GetStatusTypeMaxValue(_type, _resistanceType);

	UStatus* _status = nullptr;

	UStatus* _appliedStatus = _currentAppliedStatus;
	UStatus* _statusToApply = nullptr;

	for (TPair<UClass*, UStatus*> _pair : _statusBySource)
	{
		_status = _pair.Value;

		if (_status)
		{
			if (_checkResistanceType)
			{
				if (_status->GetResistanceType() != _resistanceType)
					continue;
			}
			
			if (!_appliedStatus && _status->GetIsApplied())
				_appliedStatus = _status;

			if (!_statusToApply && _status->CheckIfCanBeApplied(_maxValue))
				_statusToApply = _status;

			if (_appliedStatus && _statusToApply)
				break;
		}
	}

	if (_appliedStatus)
	{
		if (_statusToApply)
		{
			if (_appliedStatus != _statusToApply)
			{
				_appliedStatus->UnapplyStatus(GetOwner());
				_statusToApply->ApplyStatus(GetOwner());
			}
		}
		else
			_appliedStatus->UnapplyStatus(GetOwner());
	}
}

bool UStatusComponent::CheckIfResists(const EStatusType& _newStatusType)
{
	if (currentResistance.IsEmpty())
		return false;

	UStatus* _status = nullptr;
	float _resistanceValue = -1.0f;
	float _checkedValue = 0.0f;

	for (TPair<UClass*, UStatus*> _pair : currentResistance)
	{
		_status = _pair.Value;

		if (_status)
		{
			if (_status->GetResistanceType() == _newStatusType)
			{
				_checkedValue = _status->GetValue();

				if (_resistanceValue < _checkedValue)
					_resistanceValue = _checkedValue + resistanceBonus;
			}
		}
	}

	if (_resistanceValue <= 0.0f)
		return false;

	const float _probability = 100.0f / _resistanceValue;
	const float _rand = FMath::FRandRange(1.0f, _probability);

	if (_rand == _probability)
		return true;

	return false;
}

UStatus* UStatusComponent::CreateStatusFromType(AActor* _source, const FStatusParams& _params)
{
	UStatus* _newStatus = nullptr;

	switch (_params.type)
	{
	case EStatusType::SLOW:
	{
		_newStatus = CreateStatus<USlowedStatus>(_source, _params);
		break;
	}

	case EStatusType::RES_MODIF:
	{
		_newStatus = CreateStatus<UResistantStatus>(_source, _params);
		break;
	}

	case EStatusType::STAGGER:
	{
		_newStatus = CreateStatus<UStaggeredStatus>(_source, _params);
		break;
	}

	case EStatusType::DOT:
	{
		_newStatus = CreateStatus<UDOTStatus>(_source, _params);
		break;
	}

	case EStatusType::PROTECT:
	{
		_newStatus = CreateStatus<UProtectedStatus>(_source, _params);
		break;
	}

	case EStatusType::STUN:
	{
		_newStatus = CreateStatus<UStunnedStatus>(_source, _params);
		break;
	}

	case EStatusType::CD_MODIF:
	{
		if (Cast<AMainCharacter>(GetOwner()))
			_newStatus = CreateStatus<UCooldownModifierStatus>(_source, _params);

		break;
	}

	case EStatusType::DEACTIVATE:
	{
		if (Cast<AMainCharacter>(GetOwner()))
			_newStatus = CreateStatus<UDeactivatedStatus>(_source, _params);

		break;
	}

	case EStatusType::FREEZE:
	{
		_newStatus = CreateStatus<UFreezingStatus>(_source, _params);
		break;
	}

	case EStatusType::BLEED:
	{
		_newStatus = CreateStatus<UBleedStatus>(_source, _params);
		break;
	}

	default:
		break;
	}

	return _newStatus;
}

void UStatusComponent::RefreshStatus(AActor* _source, const FStatusParams& _params, const bool _sourceCooldownPhase)
{
	if (!_source)
		return;

	const EStatusType _type = _params.type;
	const bool _hasType = currentStatus.Contains(_type);
	UClass* _sourceClass = _source->GetClass();
	UStatus* _status = nullptr;

	if (_hasType)
	{
		TMap<UClass*, UStatus*>& _statusBySource = currentStatus[_type].statusBySource;
		const bool _hasClass = _statusBySource.Contains(_sourceClass);

		if (_hasClass)
		{
			_status = _statusBySource[_sourceClass];

			if (_status)
			{
				_status->SetParams(_params);
				_status->RefreshStatus(_sourceCooldownPhase);
			}
		}
	}
}

void UStatusComponent::RemoveStatus(AActor* _source, UStatus* _status)
{
	if (_status)
		RemoveStatus(_source, _status->GetType());
}

void UStatusComponent::RemoveStatus(AActor* _source, const EStatusType& _statusType)
{
	if (!_source || currentStatus.IsEmpty())
		return;

	const EStatusType _type = _statusType;
	const bool _hasType = currentStatus.Contains(_type);

	if (_hasType)
	{
		const UClass* _sourceClass = _source->GetClass();
		TMap<UClass*, UStatus*>& _statusBySource = currentStatus[_type].statusBySource;
		const bool _hasClass = _statusBySource.Contains(_sourceClass);

		if (_hasClass)
		{
			UStatus* _currentStatus = _statusBySource[_sourceClass];
			_statusBySource.Remove(_sourceClass);

			if (_currentStatus)
			{
				if (_currentStatus->GetIsApplied())
					RefreshStatusAppliedByType(_statusBySource, _type, _currentStatus->GetResistanceType(), _currentStatus);
			}

			if (_statusBySource.IsEmpty())
			{
				currentStatus.Remove(_type);
				onStatusTypeRemoved.Broadcast(_type);
			}
		}
	}
}

void UStatusComponent::AddResistance(UClass* _source, UStatus* _resistance)
{
	if (!_source || !_resistance)
		return;

	currentResistance.Add(_source, _resistance);
}

void UStatusComponent::RemoveResistance(UClass* _source)
{
	if (!_source || !currentResistance.Contains(_source))
		return;

	currentResistance.Remove(_source);
}

float UStatusComponent::GetStatusTypeMaxValue(const EStatusType& _type, const EStatusType& _resistanceType)
{
	if (currentStatus.IsEmpty())
		return 0.0f;

	const bool _checkResistanceType = _type == EStatusType::RES_MODIF;
	const bool _hasType = currentStatus.Contains(_type);
	float _maxValue = -1.0f;
	UStatus* _status = nullptr;

	if (_hasType)
	{
		const TMap<UClass*, UStatus*> _statusBySource = currentStatus[_type].statusBySource;
		float _currentPairValue = -1.0f;

		for (TPair<UClass*, UStatus*> _pair : _statusBySource)
		{
			_status = _pair.Value;

			if (_status)
			{
				if (_checkResistanceType)
				{
					if (_status->GetResistanceType() != _resistanceType)
						continue;
				}

				_currentPairValue = _status->GetValue();
			}

			if (_currentPairValue > _maxValue)
				_maxValue = _currentPairValue;
		}
	}

	return _maxValue;
}

UStatus* UStatusComponent::AddStatus(AActor* _source, const FStatusParams& _params, const bool _delayApplication)
{
	if (!_source)
		return nullptr;

	const EStatusType _type = _params.type;

	if (CheckIfResists(_type))
		return nullptr;

	const bool _hasType = currentStatus.Contains(_type);
	UClass* _sourceClass = _source->GetClass(); 
	UStatus* _status = nullptr;

	if (_hasType)
	{
		if (_type == EStatusType::STAGGER)
			return nullptr;

		TMap<UClass*, UStatus*>& _statusBySource = currentStatus[_type].statusBySource;
		const bool _hasClass = _statusBySource.Contains(_sourceClass);

		if (_hasClass)
		{
			_status = _statusBySource[_sourceClass];

			if (_status)
			{
				_status->RefreshStatus(false, GetOwner());

				if (_type == EStatusType::BLEED)
					_status->SetValue(_params.value);
			}

			//C'est normal, ne pas modifier
			return nullptr;
		}

		_status = CreateStatusFromType(_source, _params);

		if (_status)
		{
			_statusBySource.Add(_sourceClass, _status);
			statusToAdd = _status;

			if (_delayApplication)
			{
				FTimerHandle _delayTimer;
				GetWorld()->GetTimerManager().SetTimer(_delayTimer, this, &UStatusComponent::AddStatusOfExistingType, 0.1f, false);
				return _status;
			}

			AddStatusOfExistingType();
		}

		return _status;
	}

	_status = CreateStatusFromType(_source, _params);

	if (_status)
	{
		currentStatus.Add(_type, FStatusBySource(_sourceClass, _status));
		statusToAdd = _status;

		if (_delayApplication)
		{
			FTimerHandle _delayTimer;
			GetWorld()->GetTimerManager().SetTimer(_delayTimer, this, &UStatusComponent::AddStatusOfNewType, 0.1f, false);
			return _status;
		}

		AddStatusOfNewType();
	}

	return _status;
}
