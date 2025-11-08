#include "LeverActor.h"

ALeverActor::ALeverActor()
{
#if WITH_EDITOR
	PrimaryActorTick.bStartWithTickEnabled = true;
#endif

	useDebug = true;

	railType = nullptr;

	returnRails = TArray<ARailActor*>();
	departureRails = TArray<ARailActor*>();

	//isReverse = false;
	currentIndexReturn = 0;
	currentIndexDeparture = 0;

	isLinkedToHandcar = false;
}

void ALeverActor::DrawDebugRails(const TArray<ARailActor*>& _rails, const int _currentIndex, const bool _useDepartureStop)
{
	/*if (_rails.IsEmpty())
		return;

	const FVector _actorLoc = GetActorLocation();
	FColor _debugColor = FColor();

	const int _num = _rails.Num();

	ARailActor* _rail = nullptr;
	FVector _railLoc = FVector();

	bool _isDepartureLever = false;
	bool _isReturnLever = false;
	bool _railChangeWay = false;

	for (int i = 0; i < _num; i++)
	{
		_rail = _rails[i];

		if (_rail)
		{
			_isDepartureLever = _rail->GetDepartureLever() == this;
			_isReturnLever = _rail->GetReturnLever() == this;

			_debugColor = (!_isDepartureLever && !_isReturnLever) ? FColor::Red :
				i == _currentIndex ? FColor::Green : FColor::Blue;

			_railChangeWay = Cast<AChangeWayRailActor>(_rail) != nullptr;

			_railLoc = _rail->GetLocationAtPointIndex(_rail->GetStopIndex(_isDepartureLever && _railChangeWay ? true : _isReturnLever && _railChangeWay ? false : _useDepartureStop));
			DrawDebugLine(GetWorld(), _actorLoc, _railLoc, _debugColor);
		}
	}*/
}

void ALeverActor::UpdateCurrentIndex(TArray<ARailActor*> _rails, int& _currentIndex)
{
	const int _num = _rails.Num();
	_currentIndex = ((_currentIndex + 1) <= (_num - 1)) ? _currentIndex + 1 : 0;

	if (_rails[_currentIndex]->GetIsLocked())
	{
		if (_num == 1)
		{
			_currentIndex = -1;
			return;
		}

		UpdateCurrentIndex(_rails, _currentIndex);
	}
}

void ALeverActor::UpdateRailIsBlocked_Internal(TArray<ARailActor*>& _rails, int& _currentIndex, const bool _updateIndex)
{
	/*if (_rails.IsEmpty())
		return;

	const int _num = _rails.Num();
	bool _isBlocked = false;

	if (_updateIndex)
		UpdateCurrentIndex(_rails, _currentIndex);

	for (int i = 0; i < _num; i++)
	{
		if (_rails[i])
		{
			_isBlocked = (i == _currentIndex) ? false : true;
			_rails[i]->SetIsBlocked(this, _isBlocked);
		}
	}

	if (_currentIndex == -1)
		_currentIndex = 0;

	onUpdateRailIsBlocked.Broadcast();*/
}

void ALeverActor::UpdateRailIsBlocked(const bool _updateIndex)
{
	UpdateRailIsBlocked_Internal(departureRails, currentIndexDeparture, _updateIndex);
	UpdateRailIsBlocked_Internal(returnRails, currentIndexReturn, _updateIndex);
}

void ALeverActor::RefreshRails(TArray<ARailActor*>& _toRefreshRails)
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

void ALeverActor::SpawnRail_Internal(const bool _isDepartureRail)
{
	/*if (!railType)
		return;

	ARailActor* _rail = GetWorld()->SpawnActor<ARailActor>(railType);
	const FVector _actorLoc = GetActorLocation();

	if (!_isDepartureRail)
		_rail->SetDepartureLever(this);
	else
		_rail->SetReturnLever(this);

	_rail->SetActorLocation(FVector(_actorLoc.X + 10.0, _actorLoc.Y + 10.0, 0.0));
	AddRail(_rail, _isDepartureRail);*/
}

void ALeverActor::SpawnChangeWayRail_Internal(const bool _isDepartureRail)
{
	/*if (!changeWayRailType)
		return;

	AChangeWayRailActor* _rail = GetWorld()->SpawnActor<AChangeWayRailActor>(changeWayRailType);
	const FVector _actorLoc = GetActorLocation();

	if (!_isDepartureRail)
		_rail->SetDepartureLever(this);
	else
		_rail->SetReturnLever(this);

	_rail->SetActorLocation(FVector(_actorLoc.X + 10.0, _actorLoc.Y + 10.0, 0.0));
	AddRail(_rail, _isDepartureRail);*/
}

void ALeverActor::BeginPlay()
{
	Super::BeginPlay();
	RefreshRails(departureRails);
	RefreshRails(returnRails);
	UpdateRailIsBlocked(false);
}

void ALeverActor::DrawDebug()
{
	DrawDebugRails(departureRails, currentIndexDeparture, false);
	DrawDebugRails(returnRails, currentIndexReturn, true);
}

void ALeverActor::Interacted(AMainCharacter* _character)
{
	UpdateRailIsBlocked();
}

void ALeverActor::SpawnDepartureRail()
{
	SpawnRail_Internal(true);
}

//void ALeverActor::SpawnChangeWayDepartureRail()
//{
//	SpawnChangeWayRail_Internal(true);
//}

void ALeverActor::SpawnReturnRail()
{
	SpawnRail_Internal(false);
}

//void ALeverActor::SpawnChangeWayReturnRail()
//{
//	SpawnChangeWayRail_Internal(false);
//}

void ALeverActor::AddRail(ARailActor* _rail, const bool _isDepartureRail)
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

ARailActor* ALeverActor::GetNextRail(const bool _isOnReturn) const
{
	if (_isOnReturn)
		return GetRailFromIndex(currentIndexReturn, false);
	else
		return GetRailFromIndex(currentIndexDeparture, true);
}
