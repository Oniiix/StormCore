#include "TimeInputConditionDataAsset.h"
#include "EnhancedInputComponent.h"
#include "RobotHunter/CCC/Character/MainCharacter.h"



void UTimeInputConditionDataAsset::InitCondition(AMainCharacter* _chara)
{
	Super::InitCondition(_chara);
	UEnhancedInputComponent* _input = Cast<UEnhancedInputComponent>(_chara->InputComponent);


	//TODO USimpleInputConditionDataAsset : remove when we duplicate data asset
	handle = -1;
	currentPressedTime = 0.0f;
	//

	if (_input && inputAction)
		handle = _input->BindAction(inputAction, ETriggerEvent::Triggered, this, &UTimeInputConditionDataAsset::InputPressed).GetHandle();
}

void UTimeInputConditionDataAsset::CancelCondition()
{
	UEnhancedInputComponent* _input = Cast<UEnhancedInputComponent>(chara->InputComponent);

	_input->RemoveBindingByHandle(handle);

	handle = -1;
	currentPressedTime = 0.0f;

	Super::CancelCondition();
}

void UTimeInputConditionDataAsset::InputPressed(const FInputActionValue& _value)
{
	currentPressedTime += chara->GetWorld()->GetDeltaSeconds();
	onValidateCondition.Broadcast(currentPressedTime/ pressedTime);
	if (currentPressedTime >= pressedTime)
	{
		LOG_COLOR(TIME_INPUT_CONDITION_USE_LOG, "Success", FColor::Emerald);

		UEnhancedInputComponent* _input = Cast<UEnhancedInputComponent>(chara->InputComponent);

		_input->RemoveBindingByHandle(handle);

		handle = -1;
		currentPressedTime = 0.0f;
	}
}
