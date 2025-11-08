#include "SimpleInputConditionDataAsset.h"
#include "EnhancedInputComponent.h"
#include "RobotHunter/CCC/Character/MainCharacter.h"

void USimpleInputConditionDataAsset::InitCondition(AMainCharacter* _chara)
{
	Super::InitCondition(_chara);
	UEnhancedInputComponent* _input = Cast<UEnhancedInputComponent>(_chara->InputComponent);

	currentPressedNumber = 0;
	handle = -1;

	if (_input && inputAction)
	{
		handle = _input->BindAction(inputAction, ETriggerEvent::Started, this,
		                            &USimpleInputConditionDataAsset::InputPressed).GetHandle();
	}
}

void USimpleInputConditionDataAsset::CancelCondition()
{
	UEnhancedInputComponent* _input = Cast<UEnhancedInputComponent>(chara->InputComponent);
	_input->RemoveBindingByHandle(handle);

	currentPressedNumber = 0;
	handle = -1;

	Super::CancelCondition();
}

void USimpleInputConditionDataAsset::InputPressed(const FInputActionValue& _value)
{
	if (_value.Get<bool>())
	{
		currentPressedNumber++;
		onValidateCondition.Broadcast(static_cast<float>(currentPressedNumber) / static_cast<float>(numberOfPressedRequire));
		if (currentPressedNumber >= numberOfPressedRequire)
		{
			LOG_COLOR(SIMPLE_INPUT_CONDITION_USE_LOG, "Success", FColor::Emerald);

			UEnhancedInputComponent* _input = Cast<UEnhancedInputComponent>(chara->InputComponent);
			_input->RemoveBindingByHandle(handle);

			currentPressedNumber = 0;
			handle = -1;
		}
	}
}
