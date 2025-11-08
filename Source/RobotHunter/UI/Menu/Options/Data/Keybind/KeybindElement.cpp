// 


#include "KeybindElement.h"

#include "KeyBindMapping.h"

void UKeybindElement::Init(const TObjectPtr<UOptionFrame>& _parent, const TObjectPtr<UGameUserSettings>& _gus,
                           const TObjectPtr<UWorld>& _world, const TObjectPtr<USettingsSave>& _settings)
{
	Super::Init(_parent, _gus, _world, _settings);
}

TArray<FString> UKeybindElement::GetChoices() const
{
	return {key.ToString()};
}

/*
void UKeybindElement::SetData(const FString& String, const TTuple<FString, FKey>& keyName,
                              UInputMappingContext* _inputMappingContext,
                              const FEnhancedActionKeyMapping& _enhancedActionKeyMapping)
{
	InputMappingContext = _inputMappingContext;
	EnhancedActionKeyMapping = _enhancedActionKeyMapping;
	key = keyName.Value;
	keyStr = keyName.Key;

	if (keyStr.IsEmpty())
	{
		nameOption = TEXTSTR(FORMAT(String, "Unknown"));
	}
	else
	{
		nameOption = TEXTSTR(FORMAT(String, keyStr));
	}
}
*/

void UKeybindElement::SetData(const FCustomKey& keyName, UCategory* _category,
                              const FPlayerKeyMapping& _enhancedActionKeyMapping)
{
	//InputMappingContext = _inputMappingContext;
	Category = _category;
	PlayerKeyMapping = _enhancedActionKeyMapping;
	key = keyName.key;
	keyStr = keyName.keyName;
	keyBinding = keyStr;

	if (keyStr.IsEmpty())
	{
		nameOption = TEXTSTR("Unknown");
	}
	else
	{
		nameOption = TEXTSTR(keyStr);
	}
}


void UKeybindElement::SetNewKey(const FKey& newKey)
{
	key = newKey;
	Category->ReplaceKey(PlayerKeyMapping, keyBinding, key);
	keyStr = newKey.ToString();
	PlayerKeyMapping.SetCurrentKey(key);
}
