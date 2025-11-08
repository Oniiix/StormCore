


#include "TimeConditionDataAsset.h"

void UTimeConditionDataAsset::InitCondition(AMainCharacter* _chara)
{
	Super::InitCondition(_chara);
	isRunning = true;
	currentTime = 0.0f;
}

void UTimeConditionDataAsset::CancelCondition()
{
	Super::CancelCondition();
	currentTime = 0.0f;
	isRunning = false;
}

void UTimeConditionDataAsset::Tick(float DeltaTime)
{
	currentTime += DeltaTime;
	if (currentTime >= duration)
	{
		currentTime = 0.0f;
		isRunning = false;
		onValidateCondition.Broadcast(1.0f);
		return;
	}
	onValidateCondition.Broadcast(currentTime / duration);
}