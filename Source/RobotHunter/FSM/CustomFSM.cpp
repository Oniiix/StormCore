#include "CustomFSM.h"
#include "Components/FSMComponent.h"

UCustomFSM::UCustomFSM()
{
	initialState = nullptr;
	currentState = nullptr;
	//allStates = TMap<FString, UCustomState*>();
	owner = nullptr;
}

void UCustomFSM::CreateState(const FString& _key, TSubclassOf<UCustomState> _state)
{
	currentState = NewObject<UCustomState>(this, _state);
	
	if (currentState)
	{
		currentState->SetFSM(this);
		currentState->InitRunningTransitions();
		allStates.Add(_key, currentState);
	}
}

void UCustomFSM::Start()
{
	ChangeState(initialState);
}

void UCustomFSM::Update()
{
	if (currentState)
		currentState->Update();
}

void UCustomFSM::Stop()
{
	if (currentState)
	{
		currentState->Exit();
		currentState = nullptr;
	}
}

void UCustomFSM::ChangeState(TSubclassOf<UCustomState> _state)
{
	if (!_state)
		return;

	const FString _stateKey = _state->GetName();

	if (!allStates.Contains(_stateKey))
		CreateState(_stateKey, _state);
	else
		currentState = allStates[_stateKey];

	if (currentState)
		currentState->Enter();
}
