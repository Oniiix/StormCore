#include "ClockActor.h"
#include "RobotHunter/Utils/DebugUtils.h"

void AClockActor::BeginPlay()
{
	Super::BeginPlay();
	if (hoursToActivate.Num() == 0)
		return;
	SortHoursToActivate();
	if (timeManager->GetRealHour() == hoursToActivate[0].X)
		BIND_ACTION(timeManager, OnUpdateTime, AClockActor, CheckTime);

	BIND_ACTION(timeManager, OnNewHourStart, AClockActor, CheckHour);
}

void AClockActor::CheckTime(const int _hour, const int _minute)
{
	if (hoursToActivate[0] == FIntVector2(_hour, _minute))
		ClockActivate();
}

void AClockActor::CheckHour(const int _hour)
{
	if (_hour == hoursToActivate[0].X)
		BIND_ACTION(timeManager, OnUpdateTime, AClockActor, CheckTime);
	else
		timeManager->OnUpdateTime().RemoveDynamic(this, &AClockActor::CheckTime);
}


void AClockActor::ClockActivate()
{
	Activate();
	hoursToActivate.RemoveAt(0);

	if (hoursToActivate.Num() == 0)
	{
		timeManager->OnUpdateTime().RemoveDynamic(this, &AClockActor::CheckTime);
		timeManager->OnNewHourStart().RemoveDynamic(this, &AClockActor::CheckHour);
	}
	else
	{
		CheckTime(timeManager->GetRealHour(), timeManager->GetInGameMinutes());
		CheckHour(timeManager->GetRealHour());
	}
}


void AClockActor::SortHoursToActivate()
{
	TArray<FIntVector2> _tempList = hoursToActivate;

	const int _size = _tempList.Num();
	for (int i = 0; i < _size; i++)
	{
		FIntVector2 _currentHour = _tempList[i];
		int _currentIndex = i;
		for (int j = i + 1; j < _size; j++)
		{
			if (IsHourInferior(_tempList[j], _currentHour))
			{
				_currentHour = _tempList[j];
				_currentIndex = j;
			}
		}
		_tempList.Swap(i, _currentIndex);
	}
	hoursToActivate = _tempList;
}

bool AClockActor::IsHourInferior(const FIntVector2& _a, const FIntVector2& _b)
{
	if (_a.X < _b.X)
		return true;
	else if (_b.X < _a.X)
		return false;
	else
		return _a.Y < _b.Y;
}

