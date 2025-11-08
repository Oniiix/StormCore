#include "FSMComponent.h"

UFSMComponent::UFSMComponent()
{
	fsm = nullptr;
	runningFSM = nullptr;
}

void UFSMComponent::Update()
{
	if (runningFSM)
		runningFSM->Update();
}

void UFSMComponent::Stop()
{
	if (runningFSM)
		runningFSM->Stop();
}

void UFSMComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	Update();
}

void UFSMComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	Stop();
}

void UFSMComponent::Start()
{
	if (fsm)
	{
		runningFSM = NewObject<UCustomFSM>(this, fsm);

		if (runningFSM)
		{
			runningFSM->SetOwner(this);
			runningFSM->Start();
		}
	}
}
