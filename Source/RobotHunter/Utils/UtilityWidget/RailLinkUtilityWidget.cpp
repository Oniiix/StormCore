#include "RobotHunter/Utils/UtilityWidget/RailLinkUtilityWidget.h"
#if WITH_EDITOR
#include "Selection.h"
#endif
#include "RobotHunter/Splines/Rail/RailActor.h"
#include "RobotHunter/Game/GPE/Transmitter/TargetActor/RailsTargetActor.h"

#if WITH_EDITOR
void URailLinkUtilityWidget::LinkRailsToTarget(const TArray<ARailActor*> _rails, ARailsTargetActor* _target, const bool _isDepartureRail)
{
	if (_rails.IsEmpty() || !_target)
		return;

	const int _num = _rails.Num();

	for (size_t i = 0; i < _num; i++)
	{
		_target->AddRail(_rails[i], _isDepartureRail);

		if (_isDepartureRail)
			_rails[i]->SetReturnTarget(_target);
		else
			_rails[i]->SetDepartureTarget(_target);
	}
}

void URailLinkUtilityWidget::LinkReturnRailsToTarget()
{
	TArray<AActor*> _selectedActors = GetSelectedActors();
	LinkRailsToTarget(GetRailsFromSelectedActors(_selectedActors), GetTargetFromSelectedActors(_selectedActors), false);
}

void URailLinkUtilityWidget::LinkDepartureRailsToTarget()
{
	TArray<AActor*> _selectedActors = GetSelectedActors();
	LinkRailsToTarget(GetRailsFromSelectedActors(_selectedActors), GetTargetFromSelectedActors(_selectedActors));
}

TArray<AActor*> URailLinkUtilityWidget::GetSelectedActors()
{
	USelection* _selection = GEditor->GetSelectedActors();
	TArray<AActor*> _actors = TArray<AActor*>();
	AActor* _actor = nullptr;

	for (FSelectionIterator _it = FSelectionIterator(*_selection); _it; ++_it)
	{
		_actor = Cast<AActor>(*_it);

		if (_actor)
			_actors.Add(_actor);
	}

	return _actors;
}

TArray<ARailActor*> URailLinkUtilityWidget::GetRailsFromSelectedActors(const TArray<AActor*> _selectedActors)
{
	TArray<ARailActor*> _rails = TArray<ARailActor*>();

	if (_selectedActors.IsEmpty())
		return _rails;

	const int _num = _selectedActors.Num();
	ARailActor* _rail = nullptr;

	for (size_t i = 0; i < _num; i++)
	{
		_rail = Cast<ARailActor>(_selectedActors[i]);
		
		if (_rail)
			_rails.Add(_rail);
	}

	return _rails;
}

ARailsTargetActor* URailLinkUtilityWidget::GetTargetFromSelectedActors(const TArray<AActor*> _selectedActors)
{
	if (_selectedActors.IsEmpty())
		return nullptr;

	const int _num = _selectedActors.Num();
	ARailsTargetActor* _target = nullptr;

	for (size_t i = 0; i < _num; i++)
	{
		_target = Cast<ARailsTargetActor>(_selectedActors[i]);

		if (_target)
			break;
	}

	return _target;
}

bool URailLinkUtilityWidget::Initialize()
{
	const bool _initialized = Super::Initialize();

	if (returnLinkButton)
		returnLinkButton->OnClicked.AddDynamic(this, &URailLinkUtilityWidget::LinkReturnRailsToTarget);

	if (departureLinkButton)
		departureLinkButton->OnClicked.AddDynamic(this, &URailLinkUtilityWidget::LinkDepartureRailsToTarget);

	return _initialized;
}
#endif