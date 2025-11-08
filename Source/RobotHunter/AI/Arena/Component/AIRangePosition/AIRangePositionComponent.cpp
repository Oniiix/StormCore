// Fill out your copyright notice in the Description page of Project Settings.


#include "AIRangePositionComponent.h"
#include "RobotHunter/Utils/DebugUtils.h"
#include "RobotHunter/Utils/CustomMathBlueprintLibrary.h"
#include "RobotHunter/AI/Arena/Arena.h"
#include <NavigationSystem.h>

UAIRangePositionComponent::UAIRangePositionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UAIRangePositionComponent::SetAvailablePoint(const FPointRangeArena& _targetPoint, const bool _isAvailable)
{
	if (_targetPoint.index == -1)
		return;
	allPoints[_targetPoint.index].isAvailable = _isAvailable;
}

void UAIRangePositionComponent::BeginPlay()
{
	Super::BeginPlay();
	owner = Cast<AArena>(GetOwner());
	if (!owner)
		return;

	owner->GetCombatComponent()->OnPlayerEnterArena().AddDynamic(this, &UAIRangePositionComponent::PlayerDetected);

	allRanges.Add(ERangeAttack::Short, TArray<float>({shortRangeMin, shortRangeMax}));
	allRanges.Add(ERangeAttack::Mid, TArray<float>({middleRangeMin, middleRangeMax}));
	allRanges.Add(ERangeAttack::Long, TArray<float>({longRangeMin, longRangeMax}));

	SpawnPoint();
}

void UAIRangePositionComponent::SpawnPoint()
{
	const FVector& _arenaLocation = owner->GetActorLocation();
	const float _range = owner->GetBreakRange();

	const FVector& _initPoint = _arenaLocation + FVector(-_range, -_range, 0);
	const int _pointNumber = FMath::Floor((_range * 2) / gap);

	const float _diff = FVector::Dist(FVector(0, 0, heightMax), FVector(0, 0, heightMin));
	int _index = 0;
	for (int i = 0; i < _pointNumber; i++)
	{
		for (int j = 0; j < _pointNumber; j++)
		{
			FVector _pos = FVector(i * gap, j * gap, 0) + _initPoint;
			if (FVector::Dist2D(_pos, _arenaLocation) > _range)
				continue;
			const FVector& _lineTracePos = _pos + FVector(0, 0, heightMax);

			FHitResult _result;
			const bool _hit = UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(), _lineTracePos, _lineTracePos - FVector(0, 0, _diff), layerSnap, false, TArray<AActor*>(), EDrawDebugTrace::None, _result, true);
			if (!_hit)
				continue;

			_pos = _result.ImpactPoint;
			FPointRangeArena _point = FPointRangeArena(_pos, CheckCollision(_pos), _index);
			allPoints.Add(_point);
			_index++;
		}
	}
}

void UAIRangePositionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!playerDetected)
		return;

	for (AMainCharacter* _player : players)
	{
		const FVector& _posPlayer = _player->GetActorLocation();
		if (FVector::Distance(_posPlayer, initPlayersPosition[_player]) > distanceRefreshPoint)
		{
			RangePointPosition();
			onRefreshPoint.Broadcast();
		}
	}

	if (drawDebug)
		DrawDebug();
}

void UAIRangePositionComponent::DrawDebug()
{
	TArray<FPointRangeArena> _point;
	for (AMainCharacter* _player : players)
	{
		const TMap<TEnumAsByte<ERangeAttack>, FListPointRangeArena>& _range = rangePlayers[_player].mapRangePosition;

		//TArray<FPointRangeArena> _point = _range[ERangeAttack::Short].allPointInArena;
		//for (FPointRangeArena _pointLocation : _point)
		//	DrawDebugSphere(GetWorld(), _pointLocation.position, 30, 5, _pointLocation.isAvailable ? FColor::Blue : FColor::Black);

		_point = _range[ERangeAttack::Mid].allPointInArena;
		for (FPointRangeArena _pointLocation : _point)
			DrawDebugSphere(GetWorld(), _pointLocation.position, 30, 5, _pointLocation.isAvailable ? FColor::Red : FColor::Black);

		_point = _range[ERangeAttack::Long].allPointInArena;
		for (FPointRangeArena _pointLocation : _point)
			DrawDebugSphere(GetWorld(), _pointLocation.position, 30, 5, _pointLocation.isAvailable ? FColor::Green : FColor::Black);
	}
}

#if WITH_EDITOR
void UAIRangePositionComponent::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	const bool _rangeVariable = PropertyChangedEvent.GetPropertyName() == GET_MEMBER_NAME_CHECKED(UAIRangePositionComponent, shortRangeMin)
							 || PropertyChangedEvent.GetPropertyName() == GET_MEMBER_NAME_CHECKED(UAIRangePositionComponent, shortRangeMax)
							 || PropertyChangedEvent.GetPropertyName() == GET_MEMBER_NAME_CHECKED(UAIRangePositionComponent, middleRangeMin)
							 || PropertyChangedEvent.GetPropertyName() == GET_MEMBER_NAME_CHECKED(UAIRangePositionComponent, middleRangeMax)
							 || PropertyChangedEvent.GetPropertyName() == GET_MEMBER_NAME_CHECKED(UAIRangePositionComponent, longRangeMin)
							 || PropertyChangedEvent.GetPropertyName() == GET_MEMBER_NAME_CHECKED(UAIRangePositionComponent, longRangeMax);
	if (!_rangeVariable)
		return;

	if (shortRangeMin >= shortRangeMax - 100)
		shortRangeMin = shortRangeMax - 100;
	if (middleRangeMin >= middleRangeMax - 100)
		middleRangeMin = middleRangeMax - 100;
	if (longRangeMin >= longRangeMax - 100)
		longRangeMin = longRangeMax - 100;

	if (shortRangeMax > middleRangeMin)
		shortRangeMax = middleRangeMin;
	if (middleRangeMax > longRangeMin)
		middleRangeMax = longRangeMin;

}
#endif

void UAIRangePositionComponent::PlayerDetected(FUniqueNetIdRepl _playerNetId)
{
	AMainCharacter* _player = owner->GetCombatComponent()->GetCharacterByNetId(_playerNetId);
	if (_player)
		players.Add(_player);
	else
		LOG_ERROR(true, "Failed to get player by NetId in " + OWNER_NAME);

	nbrPlayer++;
	RangePointPosition();
	playerDetected = true;
}

void UAIRangePositionComponent::PlayerLost(FUniqueNetIdRepl _playerNetId)
{
	AMainCharacter* _player = owner->GetCombatComponent()->GetCharacterByNetId(_playerNetId);
	if (_player)
		players.Remove(_player);
	else
		LOG_ERROR(true, "Failed to get player by NetId in " + OWNER_NAME);

	nbrPlayer--;
}

void UAIRangePositionComponent::RangePointPosition()
{
	const int _countAllPoint = allPoints.Num();
	for (AMainCharacter* _player : players)
	{
		TMap<TEnumAsByte<ERangeAttack>, FListPointRangeArena> _mapRangePosition;
		const FVector& _playerLoc = _player->GetActorLocation();
		for (int i = 0; i < ERangeAttack::RangeAttack_Count; i++)
		{
			TArray<FPointRangeArena> _newRange;
			for (int j = 0; j < _countAllPoint; j++)
			{
				const ERangeAttack& _range = (ERangeAttack)i;
				if (UCustomMathBlueprintLibrary::IsFloatInRange(FVector::Distance(allPoints[j].position, _playerLoc), allRanges[_range].allRanges[0], allRanges[_range].allRanges[1]))
					_newRange.Add(allPoints[j]);
			}
			_mapRangePosition.Add((ERangeAttack)i, _newRange);
		}
		rangePlayers.Add(_player, _mapRangePosition);
		initPlayersPosition.Add(_player, _playerLoc);
	}

}

FPointRangeArena UAIRangePositionComponent::GetTargetPoint(const ERangeAttack& _rangeType, const FVector& _aiLocation, const AMainCharacter* _target, bool _canCheckLine, const float _offSetShoot)
{
	if (!_target || !rangePlayers.Contains(_target) || !rangePlayers[_target].mapRangePosition.Contains(_rangeType))
		return FPointRangeArena();

	const TArray<FPointRangeArena>& _allPointRange = rangePlayers[_target].mapRangePosition[_rangeType].allPointInArena;

	const int _sizeAllPointRange = _allPointRange.Num();

	float _distAI = TNumericLimits<int>::Max();

	int _index = -1;

	const FVector& _playerLocation = _target->GetActorLocation();
	const UWorld* _world = GetWorld();

	TArray<FPointRangeArena> _allPointShort;

	if (FVector::Distance(_aiLocation, _target->GetActorLocation()) < distanceAIPlayerMin)
	{
		const FVector& _centerRange = initPlayersPosition[_target];
		const float _minDist = allRanges[_rangeType].allRanges[0] + gap;
		for (int i = 0; i < _sizeAllPointRange; i++)
		{
			const FVector& _pointPosition = _allPointRange[i].position;
			if (FVector::Distance(_centerRange, _pointPosition) < _minDist)
				_allPointShort.Add(_allPointRange[i]);
		}
	}

	for (int i = 0; i < _sizeAllPointRange; i++)
	{
		if (!allPoints[_allPointRange[i].index].isAvailable)
			continue;
		if (_canCheckLine)
		{
			FHitResult _result;
			const bool _isLine = UKismetSystemLibrary::LineTraceSingleForObjects(_world, allPoints[_allPointRange[i].index].position + FVector(0, 0, _offSetShoot), _playerLocation,
																				layerObject, true, {}, EDrawDebugTrace::None, _result, true);
			if(_isLine)
				continue;
		}
		const float _distanceAI = FVector::Distance(_allPointRange[i].position, _aiLocation);

		if (_distanceAI < _distAI)
		{
			if (!_allPointShort.IsEmpty())
			{
				const int _sizeAllPointShort = _allPointShort.Num();
				bool _contain = false;
				for (int j = 0; j < _sizeAllPointShort; j++)
				{
					if (_allPointRange[i].position == _allPointShort[j].position)
					{
						_contain = true;
						break;
					}
				}
				if (!_contain)
					continue;
			}
			_distAI = _distanceAI;
			_index = i;
		}
	}

	if (_index == -1)
		return FPointRangeArena();


	const FPointRangeArena& _point = rangePlayers[_target].mapRangePosition[_rangeType].allPointInArena[_index];
	allPoints[_point.index].isAvailable = false;

	return _point;
}

FPointRangeArena UAIRangePositionComponent::GetPointToLocation(const FVector& _targetPosition, const ERangeAttack& _range, const AMainCharacter* _player, const bool _canLine, const float _offSetShoot)
{
	const TArray<FPointRangeArena>& _allPointRange = rangePlayers[_player].mapRangePosition[_range].allPointInArena;
	float _dist = TNumericLimits<int>::Max();
	FPointRangeArena _targetPoint;
	UWorld* _world = GetWorld();
	const FVector& _playerLocation = _player->GetActorLocation();

	for (FPointRangeArena _point : _allPointRange)
	{
		if (!_point.isAvailable)
			continue;
		if(_canLine)
		{
			FHitResult _result;
			const bool _isLine = UKismetSystemLibrary::LineTraceSingleForObjects(_world, _point.position + FVector(0, 0, _offSetShoot), _playerLocation,
				layerObject, true, {}, EDrawDebugTrace::None, _result, true);
			if (_isLine)
				continue;
		}
		const float _distanceBtwLocation = FVector::Dist(_point.position, _targetPosition);
		if (_distanceBtwLocation < _dist)
		{
			_dist = _distanceBtwLocation;
			_targetPoint = _point;
		}
	}
	if (_targetPoint.index == -1)
		return FPointRangeArena();
	allPoints[_targetPoint.index].isAvailable = false;
	return _targetPoint;
}

bool UAIRangePositionComponent::PointIsValid(const FPointRangeArena& _startPoint, const ERangeAttack& _range, const AMainCharacter* _target) const
{
	if (!_target)
	{
		LOG_ERROR(true, "UAIRangePositionComponent target null");
		return false;
	}
	const TArray<FPointRangeArena> _list = rangePlayers[_target].mapRangePosition[_range].allPointInArena;
	for (FPointRangeArena _point : _list)
		if (_point.index == _startPoint.index)
			return true;

	return false;
}

TArray<FPointRangeArena> UAIRangePositionComponent::CheckCollisionAllPoint(const bool _checkCover)
{
	TArray<FPointRangeArena> _listCover;
	for (FPointRangeArena _point : allPoints)
	{
		_point.isAvailable = CheckCollision(_point.position);
		if (_point.isAvailable && _checkCover)
			CheckCover(_point, _listCover);
	}
	return _listCover;
}

bool UAIRangePositionComponent::CheckCollision(const FVector& _pos) const
{
	TArray<AActor*> _item;
	FHitResult _result;
	if (!PointIsNavigable(_pos))
		return false;
	return !UKismetSystemLibrary::SphereOverlapActors(GetWorld(), _pos, 30, layerObject, nullptr, TArray<AActor*>(), _item);
}

void UAIRangePositionComponent::CheckCover(const FPointRangeArena& _point, TArray<FPointRangeArena>& _listPoint) const
{
	TArray<AActor*> _item;
	const bool _hit = UKismetSystemLibrary::SphereOverlapActors(GetWorld(), _point.position, distanceMaxCover, layerCover, nullptr, TArray<AActor*>(), _item);
	if (_hit)
		_listPoint.Add(_point);
}

bool UAIRangePositionComponent::PointIsNavigable(const FVector& _position)const
{
	UNavigationSystemV1* _nav = UNavigationSystemV1::GetCurrent(GetWorld());
	if (!_nav)
		return false;

	FNavLocation _navLocation;
	return _nav->ProjectPointToNavigation(_position, _navLocation);
}

ERangeAttack UAIRangePositionComponent::GetAICurrentRangeOnTarget(const FVector& _AILocation, const AMainCharacter* _player) const
{
	ERangeAttack _range = ERangeAttack::RangeAttack_Count;
	if (!_player || !initPlayersPosition.Contains(_player))
		return _range;

	const float _distanceAIToTarget = FVector::Dist2D(_AILocation, initPlayersPosition[_player]);
	for (int i = 0; i < ERangeAttack::RangeAttack_Count; i++)
	{
		const ERangeAttack& _currentRange = (ERangeAttack)i;
		if (_distanceAIToTarget < allRanges[_currentRange].allRanges[1])
		{
			_range = _currentRange;
			break;
		}
	}
	return _range;
}

FVector UAIRangePositionComponent::GetPositionRandomAround(const FVector& _pointPosition, const float _length) const
{
	TArray<FPointRangeArena> _listPoint;
	for (FPointRangeArena _point : allPoints)
	{
		if (!_point.isAvailable || !PointIsNavigable(_pointPosition))
			continue;
		if (FVector::Dist(_point.position, _pointPosition) > _length)
			continue;
		_listPoint.Add(_point);
	}
	const float _num = _listPoint.Num();
	if (_num == 0)
		return _pointPosition;
	int _rand = FMath::FRandRange(0, _num - 1);
	
	return _listPoint[_rand].position;
}

bool UAIRangePositionComponent::IsAIInOfRange(const FVector& _AILocation, const AMainCharacter* _player, const ERangeAttack& _range) const
{
	if (!_player || !initPlayersPosition.Contains(_player))
		return false;
	return FVector::Dist2D(_AILocation, initPlayersPosition[_player]) < allRanges[_range].allRanges[1];
}

//TArray<FPointRangeArena> UAIRangePositionComponent::GetPointAround(const FVector& _actorPosition, const FVector& _boundsActor, FBox _box)
//{
//	TArray< FPointRangeArena> _points;
//	for (FPointRangeArena _point : allPoints)
//	{
//		if (!_point.isAvailable)
//			continue;
//		const FVector& _dist = UCustomMathBlueprintLibrary::CustomDistance(_actorPosition, _point.position);
//		const float _distTest = _box.ComputeSquaredDistanceToPoint(_point.position);
//		if (_distTest <= gap)
//		{
//			_points.Add(_point);
//			UE_LOG(LogTemp, Warning, TEXT("%f"), _distTest);
//			DrawDebugString(GetWorld(), _point.position, FString::SanitizeFloat(_distTest),nullptr,FColor::Black,50);
//		}
//	}
//
//	return _points;
//}
