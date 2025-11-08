// 


#include "InputSubsystem.h"

#include "CommonInputSubsystem.h"
#include "EnhancedInputSubsystems.h"
#include "GameplayTagContainer.h"
#include "RobotHunter/Utils/DebugUtils.h"
#include "UserSettings/EnhancedInputUserSettings.h"

void UInputSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	CALLBACK_LAMBDA_AFTER_TIME(2.0f, LoadInput(); RegisterEvents(););
}

void UInputSubsystem::LoadInput()
{
	const UEnhancedInputLocalPlayerSubsystem* EISubsystem = GETFPC->GetLocalPlayer()->
	                                                                    GetSubsystem<
		                                                              UEnhancedInputLocalPlayerSubsystem>();
	if (UEnhancedInputUserSettings* UserSettings = EISubsystem->GetUserSettings())
	{
		/*LOG_DEBUG(GETFPC->GetLocalPlayer() ? "nullptr" : "not null");*/
		UserSettings->LoadOrCreateSettings(GETFPC->GetLocalPlayer());

		//get all profiles (we only have one)
		for (const TPair<FGameplayTag, TObjectPtr<UEnhancedPlayerMappableKeyProfile>>& ProfilePair : UserSettings->
		     GetAllSavedKeyProfiles())
		{
			const TObjectPtr<UEnhancedPlayerMappableKeyProfile>& Profile = ProfilePair.Value;
			if (!Profile)
			{
				LOG_ERROR(true, "Profile is null");
				return;
			}
			const TMap<FName, FKeyMappingRow>& _mappingRow = Profile->GetPlayerMappingRows();
			for (const TPair<FName, FKeyMappingRow>& RowPair : _mappingRow)
			{
				const int _size = RowPair.Value.Mappings.Num();
				//get every ActionMapping in mapping
				for (int i = 0; i < _size; i++)
				{
					FPlayerKeyMapping _mapping = RowPair.Value.Mappings.Get(FSetElementId::FromInteger(0));

					const FString _mappingName = _mapping.GetMappingName().ToString().ToLower();
					const FKey& _key = _mapping.GetCurrentKey();
					keys.Add(_mappingName, _key);

					//TODO ne pas supppr cette ligne, sinon ça marche plus 
				}
			}
		}
	}
	onLoadKey.Broadcast(false);
}
/*
void UInputSubsystem::SetCurrentFocused(UButtonPrefab* _button)
{
	currentButtonFocused = _button;
}*/

void UInputSubsystem::SetInputImage(const TObjectPtr<UKeyImageDataAsset>& Object)
{
	keyImageDataAsset = Object;
}

void UInputSubsystem::HandleInputChanged(ECommonInputType CommonInput)
{
	isGamepad = CommonInput == ECommonInputType::Gamepad;
	/*if (!isGamepad && currentButtonFocused)
		currentButtonFocused->Callback_OnUnHover();*/

	onInputControllerChange.Broadcast(CommonInput == ECommonInputType::Gamepad);
}

void UInputSubsystem::RegisterEvents()
{
	if (UCommonInputSubsystem* CommonInputSystem = UCommonInputSubsystem::Get(GETFPC->GetLocalPlayer()))
		CommonInputSystem->OnInputMethodChangedNative.AddUObject(this, &UInputSubsystem::HandleInputChanged);
}

FKey UInputSubsystem::FindKey(const FString& Keymapping)
{
	if (keys.Contains(Keymapping))
		return keys[Keymapping];
	return FKey();
}

void UInputSubsystem::ReplaceKey(const FString& _keyMapping, const FKey& _key)
{
	if (keys.Contains(_keyMapping))
	{
		//LOG_DEBUG("Update key: "+ keys[_keyMapping].ToString() + " to " + _key.ToString());
		keys[_keyMapping] = _key;
	}
}