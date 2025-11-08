#pragma once

#include "CoreMinimal.h"
#include "InputMappingContext.h"
#include "Category/Category.h"
#include "RobotHunter/UI/Menu/Options/Data/OptionData.h"
#include "RobotHunter/UI/Template/Prefab/SeparatorPrefab/SeparatorPrefab.h"

#include "KeyBindMapping.generated.h"

UCLASS(Blueprintable, BlueprintType)
class ROBOTHUNTER_API UKeyBindMapping final : public UOptionData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Custom")
	TArray<UInputMappingContext*> Contexts;

	//The allowed category to show keybind. (Keyboard, Gamepad, Debug or whatever)
	UPROPERTY(EditAnywhere, Category = "Custom")
	TArray<FString> allowedCategory = {"Keyboard", "Gamepad"};

	/** The order of keys to show \nSet the name mapping of key*/
	UPROPERTY(EditAnywhere, Category = "Custom")
	TArray<FString> keyMapOrder = {"keyboard_move_forward"};

	UPROPERTY()
	TMap<FString, UCategory*> Categories;

	UPROPERTY(EditAnywhere, Category = "Custom|Prefab")
	TSubclassOf<USeparatorPrefab> CategoryPrefab;

	UPROPERTY(EditAnywhere, Category = "Custom|Prefab")
	TSubclassOf<USeparatorPrefab> SubCategoryPrefab;

public:
	void Refresh();

private:
	virtual void Init(const TObjectPtr<UOptionFrame>& Parent, const TObjectPtr<UGameUserSettings>& GUS,
	                  const TObjectPtr<UWorld>& World, const TObjectPtr<USettingsSave>& Settings) override;
	virtual TArray<FString> GetChoices() const override;

	void InitContext();
	void LoadMapping(const FPlayerKeyMapping& _mapping);
	void AddMappingToCategory(const FPlayerKeyMapping& Mapping,
	                          const FString& Category, const FString& SubCategory);
	/*void CreateElement(const TObjectPtr<UOptionFrame>& _parent, const TObjectPtr<UWorld>& _world,
	                   const TTuple<FString, FKey>& KeyByName, UInputMappingContext* _context,
	                   const FEnhancedActionKeyMapping& _enhanced) const;*/
	void CreateElement(const TObjectPtr<UOptionFrame>& _parent, const TObjectPtr<UWorld>& _world, UCategory* _category,
	                   const FCustomKey& KeyByName, const FPlayerKeyMapping& _enhanced) const;
	void LoadData(const TObjectPtr<UOptionFrame>& Parent, const TObjectPtr<UWorld>& World);
};
