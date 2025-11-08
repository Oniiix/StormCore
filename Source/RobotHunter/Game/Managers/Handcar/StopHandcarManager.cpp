#include "StopHandcarManager.h"

UStopHandcarManager::UStopHandcarManager()
{
	stops = TMap<FString, AStopHandcarActor*>();
}

void UStopHandcarManager::AddStop(AStopHandcarActor* _stop)
{
	if (_stop)
	{
		const FString _name = _stop->GetName();

		if (!stops.Contains(_name))
			stops.Add(_name, _stop);
	}
}

void UStopHandcarManager::CallHandcar(AStopHandcarActor* _stop)
{
	if (_stop->GetHandcar())
		return;

	if (handcar)
		handcar->AddCallingStop(_stop);
}
