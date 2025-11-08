// 


#include "Category.h"

#include "UserSettings/EnhancedInputUserSettings.h"

/*
void UCategory::Init(/*const TObjectPtr<UInputMappingContext>& _context, const FEnhancedActionKeyMapping& _keyMapping)
{
	//Context = _context;
	KeyMapping = _keyMapping;
}*/


void UCategory::AddSubCategory(const FString& _name, const TObjectPtr<UCategory>& SubCategory)
{
	SubCategories.Add(_name, SubCategory);
}

void UCategory::AddKey(const FPlayerKeyMapping& _playerKeyMapping, const FString& KeyName, const FKey& Key)
{
	Keys.Add(FCustomKey(KeyName, _playerKeyMapping, Key));
}

void UCategory::ReplaceKey(const FPlayerKeyMapping& _playerKeyMapping, const FString& KeyName, const FKey& Key)
{
	for (FCustomKey& CustomKey : Keys)
	{
		if (CustomKey.playerKeyMapping == _playerKeyMapping)
		{
			CustomKey.keyName = KeyName;
			CustomKey.key = Key;
		}
	}
}

void UCategory::GetKeyByIdentifierMapping(const FString& _keyIdentifier, bool& _exist, FCustomKey& _out) const
{
	_exist = false;
	_out = FCustomKey();

	for (const FCustomKey& CustomKey : Keys)
	{
		if (CustomKey.playerKeyMapping.GetMappingName().ToString().ToLower().Equals(_keyIdentifier.ToLower()))
		{
			_exist = true;
			_out = CustomKey;
			break;
		}
	}
}
