// 


#include "KeyBindMapping.h"

#include "EnhancedInputSubsystems.h"
#include "GameplayTagContainer.h"
#include "KeybindElement.h"
#include "Category/Category.h"
#include "RobotHunter/UI/Menu/Options/OptionFrame.h"
#include "RobotHunter/UI/Template/Prefab/KeybindPrefab/KeybindPrefab.h"
#include "UserSettings/EnhancedInputUserSettings.h"


void UKeyBindMapping::Init(const TObjectPtr<UOptionFrame>& _parent, const TObjectPtr<UGameUserSettings>& _gus,
                           const TObjectPtr<UWorld>& _world, const TObjectPtr<USettingsSave>& _settings)
{
	Super::Init(_parent, _gus, _world, _settings);

	FTimerHandle timer;
	_world->GetTimerManager().SetTimer(timer, this, &UKeyBindMapping::InitContext, .1f, false, .5f);
}

TArray<FString> UKeyBindMapping::GetChoices() const
{
	return {};
}


void UKeyBindMapping::AddMappingToCategory(const FPlayerKeyMapping& Mapping, const FString& Category,
                                           const FString& SubCategory)
{
	const FString _nameKey = Mapping.GetDisplayName().ToString();

	if (!Categories.Contains(Category))
	{
		UCategory* _cat = NewObject<UCategory>(this);
		Categories.Add(Category, _cat);
	}

	UCategory* _cat = Categories[Category];

	if (!SubCategory.IsEmpty())
	{
		if (!_cat->GetSubCategories().Contains(SubCategory))
		{
			UCategory* _subCat = NewObject<UCategory>(this);
			_cat->AddSubCategory(SubCategory, _subCat);
		}
		_cat->GetSubCategories()[SubCategory]->AddKey(Mapping, _nameKey, Mapping.GetCurrentKey());
	}
	else
	{
		_cat->AddKey(Mapping, _nameKey, Mapping.GetCurrentKey());
	}
}

void UKeyBindMapping::Refresh()
{
	LoadData(parent, world);
}

void UKeyBindMapping::LoadMapping(const FPlayerKeyMapping& _mapping)
{
	if (!prefab)
	{
		return;
	}
	if (!keyMapOrder.Contains(_mapping.GetMappingName().ToString().ToLower()))
	{
		return;
	}

	TArray<FString> displayCategoriesSplit;
	SPLIT(_mapping.GetDisplayCategory().ToString(), displayCategoriesSplit, "|");


	const int sizeList = displayCategoriesSplit.Num();
	if (sizeList == 0)
	{
		return;
	}


	const FString category = displayCategoriesSplit[1];
	FString subCategory;
	if (sizeList >= 3)
	{
		subCategory = displayCategoriesSplit[2];
	}

	if (allowedCategory.Contains(displayCategoriesSplit[0]))
	{
		AddMappingToCategory(_mapping, category, subCategory);
	}
}

/*
void UKeyBindMapping::CreateElement(const TObjectPtr<UOptionFrame>& _parent, const TObjectPtr<UWorld>& _world,
                                    const TTuple<FString, FKey>& KeyByName, UInputMappingContext* _context,
                                    const FEnhancedActionKeyMapping& _enhanced) const
{
	TObjectPtr<UKeybindPrefab> _element = CreateWidget<UKeybindPrefab>(_world, prefab);
	UKeybindElement* _e = NewObject<UKeybindElement>();

	_e->SetData(format, KeyByName, _context, _enhanced);

	_parent->AddChildParameter(_element);
	_element->SetOptionData(_e);
}*/

void UKeyBindMapping::CreateElement(const TObjectPtr<UOptionFrame>& _parent, const TObjectPtr<UWorld>& _world,
                                    UCategory* _category,
                                    const FCustomKey& KeyByName, const FPlayerKeyMapping& _enhanced) const
{
	TObjectPtr<UKeybindPrefab> _element = CreateWidget<UKeybindPrefab>(_world, prefab);
	UKeybindElement* _e = NewObject<UKeybindElement>(UKeybindElement::StaticClass());

	_e->SetData(KeyByName, _category, _enhanced);

	_parent->AddChildParameter(_element);
	_element->SetOptionData(_e);
	_element->SetKeyName(_e->GetKeyStr());
}

void UKeyBindMapping::LoadData(const TObjectPtr<UOptionFrame>& _parent, const TObjectPtr<UWorld>& _world)
{
	const TObjectPtr<UScrollBox> ScrollBox = _parent->GetOptionParameter()->GetScrollbox();
	ScrollBox->ClearChildren();


	for (const TTuple<FString, UCategory*>& Category : Categories)
	{
		_parent->AddCategory(CategoryPrefab, Category.Key);

		UCategory* _cat = Category.Value;

		int _size = keyMapOrder.Num();
		for (int i = 0; i < _size; i++)
		{
			FCustomKey Key;
			bool bExist;
			_cat->GetKeyByIdentifierMapping(keyMapOrder[i], bExist, Key);
			if (bExist)
			{
				CreateElement(_parent, _world, _cat, Key, Key.playerKeyMapping);
			}
		}

		const TMap<FString, UCategory*> _subCategories = Category.Value->GetSubCategories();
		if (!_subCategories.IsEmpty())
		{
			for (const TTuple<FString, UCategory*>& SubCategory : _subCategories)
			{
				_parent->AddCategory(SubCategoryPrefab, SubCategory.Key);

				_size = keyMapOrder.Num();
				for (int i = 0; i < _size; i++)

				{
					for (int i2 = 0; i2 < _size; i2++)
					{
						FCustomKey Key;
						bool bExist;
						_cat->GetKeyByIdentifierMapping(keyMapOrder[i2], bExist, Key);
						if (bExist)
						{
							CreateElement(_parent, _world, SubCategory.Value, Key, Key.playerKeyMapping);
						}
					}
				}
			}
		}
	}
}

void UKeyBindMapping::InitContext()
{
	if (!world)
	{
		return;
	}
	if (!world->GetFirstLocalPlayerFromController())
	{
		return;
	}

	const UEnhancedInputLocalPlayerSubsystem* EISubsystem = world->GetFirstLocalPlayerFromController()->GetSubsystem<
		UEnhancedInputLocalPlayerSubsystem>();

	UEnhancedInputUserSettings* UserSettings = EISubsystem->GetUserSettings();

	if (UserSettings)
	{
		UserSettings->LoadOrCreateSettings(world->GetFirstLocalPlayerFromController());

		for (UInputMappingContext* Context : Contexts)
		{
			if (!UserSettings->IsMappingContextRegistered(Context))
			{
				UserSettings->RegisterInputMappingContext(Context);
			}
		}
	}

	//get all profiles (we only have one)
	for (const TPair<FGameplayTag, TObjectPtr<UEnhancedPlayerMappableKeyProfile>>& ProfilePair : UserSettings->
	     GetAllSavedKeyProfiles())
	{
		const TObjectPtr<UEnhancedPlayerMappableKeyProfile>& Profile = ProfilePair.Value;
		if (!Profile)
		{
			return;
		}

		//get every mapping in profile
		for (const TPair<FName, FKeyMappingRow>& RowPair : Profile->GetPlayerMappingRows())
		{
			const int _size = RowPair.Value.Mappings.Num();
			//get every ActionMapping in mapping
			for (int i = 0; i < _size; i++)
			{
				FPlayerKeyMapping _mapping = RowPair.Value.Mappings.Get(FSetElementId::FromInteger(0));

				LoadMapping(_mapping);
			}
		}
	}
}
