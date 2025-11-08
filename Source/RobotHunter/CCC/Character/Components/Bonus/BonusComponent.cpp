#include "BonusComponent.h"
#include "RobotHunter/CCC/Character/MainCharacter.h"
#include "RobotHunter/Utils/DebugUtils.h"

UBonusComponent::UBonusComponent()
{
	bonus = TMap<TEnumAsByte<EBonusType>, FBonusStruct>();

	lostSpawnPercent = 5.0f;

	bleedDuration = 5.0f;
	bleedRate = 1.0f;

	player = nullptr;
	lifeComponent = nullptr;

	baseMaxLife = 100.0f;
}


void UBonusComponent::GetPlayerStatusComponent()
{
	if (player && !statusComponent)
		statusComponent = player->GetStatusComponent();
}

void UBonusComponent::UpdateBonus(FBonusStruct& _bonus)
{
	_bonus.stack += 1;

	if (_bonus.modifierFromStackCurve)
		_bonus.currentBonus = _bonus.modifierFromStackCurve->GetFloatValue(_bonus.stack);
	else
	{
		_bonus.currentBonus += _bonus.currentModifier;
		_bonus.currentModifier *= _bonus.modifierEfficiency;
	}

	_bonus.spawnPercent -= lostSpawnPercent;
}


void UBonusComponent::AddSpeedBonus(const FBonusStruct& _bonus) const
{
	if (player)
		player->SetSpeedBonus(_bonus.currentBonus);
}

void UBonusComponent::AddSpeedAimBonus(const FBonusStruct& _bonus) const
{
	if (player)
		player->SetSpeedAimBonus(_bonus.currentBonus);
}


void UBonusComponent::AddProtectionBonus(const FBonusStruct& _bonus) const
{
	if (lifeComponent)
		lifeComponent->SetProtectionBonus(_bonus.currentBonus);
}

void UBonusComponent::AddResistanceBonus(const FBonusStruct& _bonus) const
{
	if (statusComponent)
		statusComponent->SetResistanceBonus(_bonus.currentBonus);
}

void UBonusComponent::AddCooldownBonus(const FBonusStruct& _bonus) const
{
	if (player)
	{
		TArray<AMembersActor*> _members = player->GetUsingMembers();

		if (_members.IsEmpty())
			return;

		const int _num = _members.Num();

		ARightArmActor* _rightArm = nullptr;
		AMembersActor* _member = nullptr;

		for (size_t i = 0; i < _num; i++)
		{
			_member = _members[i];

			if (_member)
			{
				_rightArm = Cast<ARightArmActor>(_member);

				if (!_rightArm)
					_member->ModifyCooldownBonus(_bonus.currentBonus);
			}
		}
	}
}


void UBonusComponent::AddLifeBonus(const FBonusStruct& _bonus) const
{
	if (lifeComponent)
	{
		const float _currentMaxLife = lifeComponent->GetMaxLife();
		const float _newMaxLife = baseMaxLife * _bonus.currentBonus;
		lifeComponent->SetMaxLife(_newMaxLife);

		const float _currentLife = lifeComponent->GetCurrentLife();
		const float _diff = _newMaxLife - _currentMaxLife;
		lifeComponent->SetStartLife(_newMaxLife);
		lifeComponent->SetLife(_currentLife + _diff, _diff, nullptr);
	}
}

void UBonusComponent::AddHealBonus(const FBonusStruct& _bonus) const
{
	if (lifeComponent)
		lifeComponent->SetHealBonus(_bonus.currentBonus);
}

void UBonusComponent::AddRegenBonus(const FBonusStruct& _bonus) const
{
	if (lifeComponent)
		lifeComponent->SetRegenLife(_bonus.currentBonus);
}

void UBonusComponent::AddReviveBonus(const FBonusStruct& _bonus) const
{
	if (lifeComponent)
		lifeComponent->SetReviveLifePercent(_bonus.currentBonus);
}


void UBonusComponent::AddWeaponDamageBonus(const FBonusStruct& _bonus) const
{
	if (player)
	{
		TArray<AMembersActor*> _members = player->GetUsingMembers();

		if (!_members.IsEmpty())
		{
			const int _num = _members.Num();
			AMembersActor* _member = nullptr;

			ARightArmActor* _rightArm = nullptr;
			ALeftArmActor* _leftArm = nullptr;

			for (size_t i = 0; i < _num; i++)
			{
				_member = _members[i];

				if (_member)
				{
					_rightArm = Cast<ARightArmActor>(_member);
					_leftArm = Cast<ALeftArmActor>(_member);

					if (_rightArm || _leftArm)
						_member->SetBonusDamage(_bonus.currentBonus);
				}
			}
		}
	}
}

void UBonusComponent::AddKickDamageBonus(const FBonusStruct& _bonus) const
{
	if (player)
	{
		TArray<AMembersActor*> _members = player->GetUsingMembers();

		if (!_members.IsEmpty())
		{
			const int _num = _members.Num();
			AMembersActor* _member = nullptr;

			ACalfActor* _calf = nullptr;

			for (size_t i = 0; i < _num; i++)
			{
				_member = _members[i];

				if (_member)
				{
					_calf = Cast<ACalfActor>(_member);

					if (_calf)
						_member->SetBonusDamage(_bonus.currentBonus);
				}
			}
		}
	}
}


void UBonusComponent::AddCounterBonus(const FBonusStruct& _bonus) const
{
	if (lifeComponent)
		lifeComponent->SetCounterBonus(_bonus.currentBonus);
}


void UBonusComponent::AddBleedBonus(const FBonusStruct& _bonus) const
{
	if (player)
	{
		TArray<AMembersActor*> _members = player->GetUsingMembers();

		if (!_members.IsEmpty())
		{
			const int _num = _members.Num();
			AMembersActor* _member = nullptr;

			for (size_t i = 0; i < _num; i++)
			{
				_member = _members[i];

				if (_member)
					_member->SetBleedBonusParams(_bonus.currentBonus, bleedDuration, bleedRate);
			}
		}
	}

	if (lifeComponent)
		lifeComponent->SetBleedBonusParams(_bonus.currentBonus, bleedDuration, bleedRate);
}


void UBonusComponent::AddLifeStealBonus(const FBonusStruct& _bonus) const
{
	if (lifeComponent)
		lifeComponent->SetLifeStealBonus(_bonus.currentBonus);
}


float UBonusComponent::GetMaxSpawnPercent() const
{
	float _maxPercent = 0.0f;
	const int _count = BonusTypeCount;
	EBonusType _type = DefaultBonus;

	for (int i = 1; i < _count; i++)
	{
		_type = (EBonusType)i;

		if (bonus.Contains(_type))
			_maxPercent += bonus[_type].spawnPercent;
	}

	return _maxPercent;
}


FBonusStruct UBonusComponent::GetRandomBonusFromProbabilities(const float _rand, float& _maxPercent,
                                                              TMap<float, FBonusStruct>& _probabilities) const
{
	FBonusStruct _bonus = FBonusStruct();

	float _min = 0.0f;
	float _max = 0.0f;

	bool _removePercent = false;
	float _percentRemoved = 0.0f;

	for (TPair<float, FBonusStruct>& _pair : _probabilities)
	{
		if (_removePercent)
		{
			_pair.Key -= _percentRemoved;
			continue;
		}

		_bonus = _pair.Value;
		_min = _pair.Key;
		_max = _min + (_bonus.spawnPercent / _maxPercent) * 100.0f;

		if (!_removePercent && _rand > _min && _rand <= _max)
			_removePercent = true;
	}

	_maxPercent -= (_percentRemoved);
	_probabilities.Remove(_min);
	return _bonus;
}

FBonusStruct UBonusComponent::GetRandomBonus() const
{
	const TArray<FBonusStruct> _randomBonus = GetRandomBonus(1);
	return _randomBonus.IsEmpty() ? FBonusStruct() : _randomBonus[0];
}

TArray<FBonusStruct> UBonusComponent::GetRandomBonus(const int _numberOfTypes) const
{
	TArray<FBonusStruct> _randomBonus = TArray<FBonusStruct>();

	if (!bonus.IsEmpty())
	{
		float _maxPercent = GetMaxSpawnPercent();
		TMap<float, FBonusStruct> _probabilities = TMap<float, FBonusStruct>();
		const int _count = BonusTypeCount;

		FBonusStruct _bonus = FBonusStruct();
		EBonusType _checkType = DefaultBonus;
		float _currentPercent = 0.0f;
		float _previousPercent = 0.0f;

		for (int i = 1; i < _count; i++)
		{
			_checkType = (EBonusType)i;

			if (bonus.Contains(_checkType))
			{
				_bonus = bonus[_checkType];
				_currentPercent = (_bonus.spawnPercent / _maxPercent) * 100.0f;

				if (_currentPercent > 0.0f)
				{
					_probabilities.Add(_previousPercent, _bonus);
					_previousPercent += _currentPercent;
				}
			}
		}

		float _rand = FMath::FRandRange(0.01f, 100.0f);

		for (int i = 0; i < _numberOfTypes; i++)
		{
			_bonus = GetRandomBonusFromProbabilities(_rand, _maxPercent, _probabilities);
			_randomBonus.Add(_bonus);
			_rand = FMath::FRandRange(0.01f, 100.0f);
		}
	}

	return _randomBonus;
}


void UBonusComponent::BeginPlay()
{
	Super::BeginPlay();

	for (const TPair<EBonusType, FBonusStruct>& _pair : bonus)
	{
		bonus[_pair.Key].currentModifier = _pair.Value.baseModifier;
		bonus[_pair.Key].type = _pair.Key;
	}
}


void UBonusComponent::AddBonus(const EBonusType& _bonusType, const bool _updateBonus)
{
	if (bonus.Contains(_bonusType))
	{
		FBonusStruct& _bonus = bonus[_bonusType];

		if (_updateBonus && _bonusType != RegenBonus)
			UpdateBonus(_bonus);

		switch (_bonusType)
		{
		case SpeedBonus:
			{
				AddSpeedBonus(_bonus);
				break;
			}

		case SpeedAimBonus:
			{
				AddSpeedAimBonus(_bonus);
				break;
			}

		case ProtectionBonus:
			{
				AddProtectionBonus(_bonus);
				break;
			}

		case ResistanceBonus:
			{
				AddResistanceBonus(_bonus);
				break;
			}

		case CooldownBonus:
			{
				AddCooldownBonus(_bonus);
				break;
			}

		case LifeBonus:
			{
				AddLifeBonus(_bonus);
				break;
			}

		case HealBonus:
			{
				AddHealBonus(_bonus);
				break;
			}

		case RegenBonus:
			{
				if (_updateBonus)
				{
					if (_bonus.modifierFromStackCurve)
					{
						_bonus.stack += 1;
						_bonus.currentBonus = _bonus.modifierFromStackCurve->GetFloatValue(_bonus.stack);
					}
					else
					{
						_bonus.currentBonus = _bonus.currentBonus + (_bonus.baseModifier * FMath::Pow(
							_bonus.baseModifier, _bonus.stack));
						_bonus.stack += 1;
					}

					_bonus.spawnPercent -= lostSpawnPercent;
				}

				AddRegenBonus(_bonus);
				break;
			}

		case ReviveBonus:
			{
				AddReviveBonus(_bonus);
				break;
			}

		case WeaponDamageBonus:
			{
				AddWeaponDamageBonus(_bonus);
				break;
			}

		case KickDamageBonus:
			{
				AddKickDamageBonus(_bonus);
				break;
			}

		case CounterBonus:
			{
				AddCounterBonus(_bonus);
				break;
			}

		case BleedBonus:
			{
				AddBleedBonus(_bonus);
				break;
			}

		case LifeStealBonus:
			{
				AddLifeStealBonus(_bonus);
				break;
			}

		default:
			break;
		}

		if (_bonus.type != EBonusType::DefaultBonus && _bonus.type != EBonusType::BonusTypeCount)
			onAddBonus.Broadcast(_bonus);
	}
}

void UBonusComponent::SetPlayer(AMainCharacter* _player)
{
	player = _player;

	if (player && !lifeComponent)
	{
		lifeComponent = player->GetLifeComponent();

		if (lifeComponent)
			baseMaxLife = lifeComponent->GetMaxLife();
	}
}

void UBonusComponent::AddRandomBonus()
{
	const FBonusStruct _randomBonus = GetRandomBonus();
	AddBonus(_randomBonus.type);
}

void UBonusComponent::AddRandomBonus(const int _numberOfBonus)
{
	const TArray<FBonusStruct> _randomBonus = GetRandomBonus(_numberOfBonus);

	if (!_randomBonus.IsEmpty())
	{
		for (const FBonusStruct _bonus : _randomBonus)
			AddBonus(_bonus.type);
	}
}


void UBonusComponent::LoadBonus(const TArray<FBonusStruct>& _bonus)
{
	const int _count = _bonus.Num();
	EBonusType _bonusType = DefaultBonus;

	for (int i = 0; i < _count; i++)
	{
		_bonusType = _bonus[i].type;

		if (bonus.Contains(_bonusType))
		{
			bonus[_bonusType] = _bonus[i];
			AddBonus(_bonusType, false);
		}
	}
}

void UBonusComponent::RefreshMembersDamageBonus()
{
	AddBonus(CooldownBonus, false);
	AddBonus(WeaponDamageBonus, false);
	AddBonus(KickDamageBonus, false);
	AddBonus(BleedBonus, false);
}
