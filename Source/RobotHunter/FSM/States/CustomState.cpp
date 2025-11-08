#include "CustomState.h"
#include "../Transitions/CustomTransition.h"
#include "../CustomFSM.h"
#include <Kismet/KismetSystemLibrary.h>

UCustomState::UCustomState()
{
	transitions = TArray<TSubclassOf<UCustomTransition>>();
	fsm = nullptr;
	runningTransitions = TArray<UCustomTransition*>();
}

void UCustomState::CheckForValidTransition()
{
	if (runningTransitions.IsEmpty())
	{
		return;
	}

	const int32 _num = runningTransitions.Num();

	for (size_t i = 0; i < _num; i++)
	{
		if (runningTransitions[i] && runningTransitions[i]->IsValidTransition())
		{
			if (fsm)
			{
				Exit();
				fsm->ChangeState(runningTransitions[i]->GetNextState());
				return;
			}
		}
	}
}

void UCustomState::InitRunningTransitions()
{
	const int32 _num = transitions.Num();

	for (size_t i = 0; i < _num; i++)
	{
		if (!transitions[i])
			continue;

		UCustomTransition* _transition = NewObject<UCustomTransition>(this, transitions[i]);

		if (_transition)
		{
			_transition->SetFSM(fsm);
			runningTransitions.Add(_transition);
		}
	}
}

void UCustomState::Enter()
{
	onEnter.Broadcast();
}

void UCustomState::Update()
{
	CheckForValidTransition();
	onUpdate.Broadcast();
}

void UCustomState::Exit()
{
	onExit.Broadcast();
}
