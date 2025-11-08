// Fill out your copyright notice in the Description page of Project Settings.


#include "DummyButton.h"
#include "RobotHunter/AI/Component/Life/LifeComponent.h"

void ADummyButton::Interacted(AMainCharacter* _character)
{
	Super::Interacted(_character);

	const int _dummyCount = dummyTargets.Num();
	for (int i = 0; i < _dummyCount; i++)
	{
		if (dummyTargets[i])
		{
			dummyTargets[i]->SetActorLocation(initPositions[i]);
			if (ULifeComponent* _life = dummyTargets[i]->GetComponentByClass<ULifeComponent>())
				_life->RestoreLife(_life->GetMaxLife());
		}
	}
}

void ADummyButton::BeginPlay()
{
	Super::BeginPlay();
	const int _dummyCount = dummyTargets.Num();
	for (int i = 0; i < _dummyCount; i++)
	{
		if (dummyTargets[i])
			initPositions.Add(dummyTargets[i]->GetActorLocation());
	}
}
