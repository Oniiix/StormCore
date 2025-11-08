#include "RailsTargetActor.h"
#include "RobotHunter/Utils/DebugUtils.h"
#include "RobotHunter/Game/Interactable/Handcar/StopHandcarActor.h"
#include "../../Handcar/CustomHandcarActor.h"
#include "RobotHunter/Network/Component/NetworkGPEComponent.h"


ARailsTargetActor::ARailsTargetActor()
{
#if WITH_EDITOR
	PrimaryActorTick.bStartWithTickEnabled = true;
#endif

	lifeComponent = CreateDefaultSubobject<ULifeComponent>("Life");
	AddOwnedComponent(lifeComponent);

	useDebug = true;

	railType = nullptr;

	returnRails = TArray<ARailActor*>();
	departureRails = TArray<ARailActor*>();

	startingIndex = 1;

	intersectionWay = ERailWay::DefaultRailWay;
	autoModeNextRails = TMap<AStopHandcarActor*, ARailActor*>();

	networkComp = nullptr;


#pragma region Detection
	detectionSphere = CreateDefaultSubobject<UBoxComponent>("DetectionSphere");
	detectionSphere->SetupAttachment(RootComponent);
	detectionSphere->SetGenerateOverlapEvents(true);

	detectedRails = TArray<ARailActor*>();
#pragma endregion


	//isReverse = false;
	currentIndexReturn = 0;
	currentIndexDeparture = 0;

	//isLinkedToHandcar = false;
}

void ARailsTargetActor::DrawDebugRails(const TArray<ARailActor*>& _rails, const int _currentIndex, const bool _useDepartureStop)
{
	if (_rails.IsEmpty())
		return;

	const FVector _actorLoc = GetActorLocation();
	FColor _debugColor = FColor();

	const int _num = _rails.Num();

	ARailActor* _rail = nullptr;
	FVector _railLoc = FVector();

	bool _isDepartureTarget = false;
	bool _isReturnTarget = false;
	bool _railChangeWay = false;

	for (int i = 0; i < _num; i++)
	{
		_rail = _rails[i];

		if (_rail)
		{
			_isDepartureTarget = _rail->GetDepartureTarget() == this;
			_isReturnTarget = _rail->GetReturnTarget() == this;

			_debugColor = (!_isDepartureTarget && !_isReturnTarget) ? FColor::Red :
				i == _currentIndex ? FColor::Green : FColor::Blue;

			_railChangeWay = Cast<AChangeWayRailActor>(_rail) != nullptr;

			_railLoc = _rail->GetLocationAtPointIndex(_rail->GetStopIndex(_isDepartureTarget && _railChangeWay ? true : _isReturnTarget && _railChangeWay ? false : _useDepartureStop));
			DrawDebugLine(GetWorld(), _actorLoc, _railLoc, _debugColor);
		}
	}
}

void ARailsTargetActor::UpdateCurrentIndex(TArray<ARailActor*> _rails, int& _currentIndex, const bool _increment, const bool _clamp0)
{
	const int _num = _rails.Num();
	const int _newIndex = _increment ? _currentIndex + 1 : _currentIndex - 1;
	_currentIndex = _newIndex <= (_num - 1) && _newIndex >= 0 ? _newIndex :
		_clamp0 ? 0 : _currentIndex;

	/*if (_rails[_currentIndex]->GetIsLocked())
	{
		if (_num == 1)
		{
			_currentIndex = -1;
			return;
		}

		UpdateCurrentIndex(_rails, _currentIndex, _increment, _clamp0);
	}*/

	if (_rails == returnRails)
	{
		if (!HasAuthority())
		{
			if (networkComp)
				networkComp->ServerRPC_UpdateCurrentIndex(this, _currentIndex, false);
		}
		else
			OnRep_UpdateIndexReturn(_currentIndex);
	}

	if (_rails == departureRails)
	{
		if (!HasAuthority())
		{
			if (networkComp)
				networkComp->ServerRPC_UpdateCurrentIndex(this, _currentIndex, true);
		}
		else
			OnRep_UpdateIndexDeparture(_currentIndex);
	}
}

void ARailsTargetActor::UpdateIndexes()
{
	const bool _hasDepartureIntersection = intersectionWay == ERailWay::PositiveRailWay;
	const bool _hasReturnIntersection = intersectionWay == ERailWay::NegativeRailWay;

	if (_hasDepartureIntersection)
	{
		const int _num = departureRails.Num();
		startingIndex = startingIndex > _num ? _num : startingIndex;
		currentIndexDeparture = startingIndex - 1;
	}
	else if (_hasReturnIntersection)
	{
		const int _num = returnRails.Num();
		startingIndex = startingIndex > _num ? _num : startingIndex;
		currentIndexReturn = startingIndex - 1;
	}

	UpdateRailIsBlocked(false, false);
}

void ARailsTargetActor::UpdateRailIsBlocked_Internal(TArray<ARailActor*>& _rails, int& _currentIndex, const bool _incrementIndex, const bool _updateIndex, const bool _clamp0)
{
	if (_rails.IsEmpty())
		return;

	const int _num = _rails.Num();
	bool _isBlocked = false;

	if (_updateIndex)
		UpdateCurrentIndex(_rails, _currentIndex, _incrementIndex, _clamp0);

	ARailActor* _rail = nullptr;

	for (int i = 0; i < _num; i++)
	{
		_rail = _rails[i];

		if (_rail)
		{
			_isBlocked = (i == _currentIndex) ? false : true;
			_rail->SetIsBlocked(this, _isBlocked);

			if (_isBlocked)
			{
				if (detectedRails.Contains(_rail))
				{
					detectedRails.Remove(_rail);
					_rail->GetRailNotDetectedAnymore().Broadcast();
				}
			}
		}
	}

	if (_currentIndex == -1)
		_currentIndex = 0;

	onUpdateRailIsBlocked.Broadcast();
}

void ARailsTargetActor::UpdateRailIsBlocked(const bool _incrementIndex, const bool _updateIndex, const bool _clamp0)
{
	UpdateRailIsBlocked_Internal(departureRails, currentIndexDeparture, _incrementIndex, _updateIndex, _clamp0);
	UpdateRailIsBlocked_Internal(returnRails, currentIndexReturn, _incrementIndex, _updateIndex, _clamp0);
}

void ARailsTargetActor::RefreshRails(TArray<ARailActor*>& _toRefreshRails)
{
	TArray<ARailActor*> _rails = TArray<ARailActor*>();
	const int _num = _toRefreshRails.Num();

	for (int i = 0; i < _num; i++)
	{
		ARailActor* _rail = _toRefreshRails[i];

		if (_rail)
			_rails.Add(_rail);
	}

	_toRefreshRails = _rails;
}

void ARailsTargetActor::SpawnRail_Internal(const bool _isDepartureRail)
{
	if (!railType)
		return;

	ARailActor* _rail = GetWorld()->SpawnActor<ARailActor>(railType);
	const FVector _actorLoc = GetActorLocation();

	if (!_isDepartureRail)
		_rail->SetDepartureTarget(this);
	else
		_rail->SetReturnTarget(this);

	_rail->SetActorLocation(FVector(_actorLoc.X + 10.0, _actorLoc.Y + 10.0, 0.0));
	AddRail(_rail, _isDepartureRail);
}

void ARailsTargetActor::SpawnChangeWayRail_Internal(const bool _isDepartureRail)
{
	if (!changeWayRailType)
		return;

	AChangeWayRailActor* _rail = GetWorld()->SpawnActor<AChangeWayRailActor>(changeWayRailType);
	const FVector _actorLoc = GetActorLocation();

	if (!_isDepartureRail)
		_rail->SetDepartureTarget(this);
	else
		_rail->SetReturnTarget(this);

	_rail->SetActorLocation(FVector(_actorLoc.X + 10.0, _actorLoc.Y + 10.0, 0.0));
	AddRail(_rail, _isDepartureRail);
}


void ARailsTargetActor::OnRep_UpdateIndexReturn(const int _index)
{
	currentIndexReturn = _index;
}

void ARailsTargetActor::OnRep_UpdateIndexDeparture(const int _index)
{
	currentIndexDeparture = _index;
}


void ARailsTargetActor::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor->GetClass()->IsChildOf<ACustomHandcarActor>())
	{
		ACustomHandcarActor* _handcar = Cast<ACustomHandcarActor>(OtherActor);

		if (_handcar)
		{
			CheckRailsOnHandcarPath(_handcar);

			/*if (_handcar->GetInAutoMode())
			{
				AStopHandcarActor* _nextStop = _handcar->GetNextStop();

				if (autoModeNextRails.Contains(_nextStop))
				{
					ARailActor* _nextRail = autoModeNextRails[_nextStop];
					const bool _isOnReturn = _handcar->GetIsOnReturn();

					if (_isOnReturn)
						currentIndexReturn = returnRails.IndexOfByKey(_nextRail);
					else
						currentIndexDeparture = departureRails.IndexOfByKey(_nextRail);

					UpdateRailIsBlocked(true, false);
				}
			}*/

			_handcar->AddCurrentTarget(this);
		}
	}
}

void ARailsTargetActor::OnComponentEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && OtherActor->GetClass()->IsChildOf<ACustomHandcarActor>())
	{
		ACustomHandcarActor* _handcar = Cast<ACustomHandcarActor>(OtherActor);

		if (_handcar)
		{
			if (!detectedRails.IsEmpty())
			{
				const int _num = detectedRails.Num();

				for (int i = 0; i < _num; i++)
					detectedRails[i]->GetRailNotDetectedAnymore().Broadcast();

				detectedRails.Empty();
			}

			_handcar->RemoveCurrentTarget(this);
		}
	}
}

void ARailsTargetActor::RailIsOnHandcarPath(ARailActor* _rail)
{
	if (!_rail)
		return;

	_rail->GetOnPathRailDetected().Broadcast();
	detectedRails.Add(_rail);
}

void ARailsTargetActor::CheckRailsOnHandcarPath(ACustomHandcarActor* _handcar)
{
	if (!_handcar)
		return;

	ARailActor* _rail = _handcar->GetCurrentRail();
	RailIsOnHandcarPath(_rail);
	_rail = GetNextRail(_handcar->GetIsOnReturn());
	RailIsOnHandcarPath(_rail);
}

void ARailsTargetActor::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* _controller = GetWorld()->GetFirstPlayerController();

	if (_controller)
		networkComp = _controller->GetComponentByClass<UNetworkGPEComponent>();

	RefreshRails(departureRails);
	RefreshRails(returnRails);
	UpdateIndexes();

	if (lifeComponent)
		lifeComponent->OnLifeChangeByDamage().AddUniqueDynamic(this, &ARailsTargetActor::ActivateTarget);

	if (detectionSphere)
	{
		detectionSphere->OnComponentBeginOverlap.AddUniqueDynamic(this, &ARailsTargetActor::OnComponentBeginOverlap);
		detectionSphere->OnComponentEndOverlap.AddUniqueDynamic(this, &ARailsTargetActor::OnComponentEndOverlap);
	}
}

void ARailsTargetActor::DrawDebug()
{
	DrawDebugRails(departureRails, currentIndexDeparture, false);
	DrawDebugRails(returnRails, currentIndexReturn, true);
}

#if WITH_EDITOR
void ARailsTargetActor::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	const bool _canUpdateIndexes = PropertyChangedEvent.GetPropertyName() == GET_MEMBER_NAME_CHECKED(ARailsTargetActor, startingIndex)
		|| PropertyChangedEvent.GetPropertyName() == GET_MEMBER_NAME_CHECKED(ARailsTargetActor, intersectionWay);

	if (_canUpdateIndexes)
		UpdateIndexes();

	Super::PostEditChangeProperty(PropertyChangedEvent);
}
#endif

void ARailsTargetActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ARailsTargetActor, currentIndexReturn);
	DOREPLIFETIME(ARailsTargetActor, currentIndexDeparture);
}

void ARailsTargetActor::ActivateTarget(AActor* _owner, const int _currentLife, const int _damage, AActor* _damageSource)
{
	lifeComponent->RestoreLife(-_damage);

		UpdateRailIsBlocked(true);
}

void ARailsTargetActor::SpawnDepartureRail()
{
	SpawnRail_Internal(true);
}

void ARailsTargetActor::SpawnReturnRail()
{
	SpawnRail_Internal(false);
}

void ARailsTargetActor::AddRail(ARailActor* _rail, const bool _isDepartureRail)
{
	if (_rail)
	{
		if (_isDepartureRail)
		{
			RefreshRails(departureRails);
			departureRails.Add(_rail);
		}
		else
		{
			RefreshRails(returnRails);
			returnRails.Add(_rail);
		}
	}
}

ARailActor* ARailsTargetActor::GetNextRail(const bool _isOnReturn, AStopHandcarActor* _nextStop)
{
	const bool _onIntersectionWay = (_isOnReturn && intersectionWay == ERailWay::NegativeRailWay) || (!_isOnReturn && intersectionWay == ERailWay::PositiveRailWay);

	if (_nextStop && _onIntersectionWay)
	{
		if (autoModeNextRails.Contains(_nextStop))
		{
			ARailActor* _nextRail = autoModeNextRails[_nextStop];

			if (_isOnReturn)
				currentIndexReturn = returnRails.IndexOfByKey(_nextRail);
			else
				currentIndexDeparture = departureRails.IndexOfByKey(_nextRail);

			UpdateRailIsBlocked(true, false);
			return _nextRail;
		}
	}
	else
	{
		if (_isOnReturn)
			return GetRailFromIndex(currentIndexReturn, false);
		else
			return GetRailFromIndex(currentIndexDeparture, true);
	}

	return nullptr;
}
