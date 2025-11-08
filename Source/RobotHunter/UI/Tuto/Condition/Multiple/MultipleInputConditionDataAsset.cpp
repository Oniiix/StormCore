#include "MultipleInputConditionDataAsset.h"
#include "EnhancedInputComponent.h"
#include "RobotHunter/CCC/Character/MainCharacter.h"



void UMultipleInputConditionDataAsset::InitCondition(AMainCharacter* _chara)
{
	Super::InitCondition(_chara);
	UEnhancedInputComponent* _input = Cast<UEnhancedInputComponent>(_chara->InputComponent);

	//TODO USimpleInputConditionDataAsset : remove when we duplicate data asset
	currentInputPressed = 0;
	pressedHandles.Empty();
	releasedHandles.Empty();
	//

	if (_input)
	{
		const int _inputActionsCount = inputActions.Num();
		for (int i = 0; i < _inputActionsCount; i++)
		{
			if (!inputActions[i]) continue;

			const int _currentHandle = pressedHandles.Num();

			//PRESS
			pressedHandles.Add(_input->BindActionValueLambda(inputActions[i], ETriggerEvent::Started, [&](const FInputActionValue& _value, const int _currentIndex, UEnhancedInputComponent* _inputContext) {
				currentInputPressed++;
				if (!pressedSimultaneously)
					_inputContext->RemoveBindingByHandle(pressedHandles[_currentIndex]);

				CheckValidation(_inputContext);
				}, _currentHandle, _input).GetHandle());

			if (!pressedSimultaneously) continue;


			//RELEASE
			releasedHandles.Add(_input->BindActionValueLambda(inputActions[i], ETriggerEvent::Completed, [&](const FInputActionValue& _value, const int _currentIndex) {
				currentInputPressed--;
				onValidateCondition.Broadcast(currentInputPressed / pressedHandles.Num());
				}, _currentHandle).GetHandle());
		}
	}
}

void UMultipleInputConditionDataAsset::CancelCondition()
{
	UEnhancedInputComponent* _input = Cast<UEnhancedInputComponent>(chara->InputComponent);

	currentInputPressed = 0;

	if (pressedSimultaneously)
	{
		const int _handlesCount = pressedHandles.Num();
		for (int i = 0; i < _handlesCount; i++)
		{
			_input->RemoveBindingByHandle(pressedHandles[i]);
			_input->RemoveBindingByHandle(releasedHandles[i]);
		}
		releasedHandles.Empty();
	}

	pressedHandles.Empty();

	Super::CancelCondition();
}

void UMultipleInputConditionDataAsset::CheckValidation(UEnhancedInputComponent* _inputContext)
{
	onValidateCondition.Broadcast(static_cast<float>(currentInputPressed) / static_cast<float>((pressedHandles.Num())));
	if (currentInputPressed >= pressedHandles.Num())
	{
		LOG_COLOR(MULTIPLE_INPUT_CONDITION_USE_LOG, "Success", FColor::Emerald);
		currentInputPressed = 0;

		if (pressedSimultaneously)
		{
			const int _handlesCount = pressedHandles.Num();
			for (int i = 0; i < _handlesCount; i++)
			{
				_inputContext->RemoveBindingByHandle(pressedHandles[i]);
				_inputContext->RemoveBindingByHandle(releasedHandles[i]);
			}
			releasedHandles.Empty();
		}

		pressedHandles.Empty();
	}
}
