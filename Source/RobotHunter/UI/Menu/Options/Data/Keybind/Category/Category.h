// 

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "UserSettings/EnhancedInputUserSettings.h"
#include "Category.generated.h"


USTRUCT()
struct FCustomKey
{
	GENERATED_USTRUCT_BODY()
	UPROPERTY()
	FString keyName;

	UPROPERTY()
	FPlayerKeyMapping playerKeyMapping;

	UPROPERTY()
	FKey key;

	FCustomKey() = default;

	FCustomKey(const FString& KeyName, const FPlayerKeyMapping& PlayerKeyMapping, const FKey& Key)
	{
		keyName = KeyName;
		playerKeyMapping = PlayerKeyMapping;
		key = Key;
	}
};

/**
 * 
 */
UCLASS()
class ROBOTHUNTER_API UCategory final : public UObject
{
	GENERATED_BODY()

	/*UPROPERTY()
	UInputMappingContext* Context;*/

	/*UPROPERTY()
	FEnhancedActionKeyMapping KeyMapping;*/

	UPROPERTY()
	TMap<FString, UCategory*> SubCategories;

	UPROPERTY()
	TArray<FCustomKey> Keys;

public:
	//FORCEINLINE UInputMappingContext* GetContext() const { return Context; }
	//FORCEINLINE FEnhancedActionKeyMapping GetKeyMapping() const { return KeyMapping; }
	//FORCEINLINE const FPlayerKeyMapping& GetKeyMapping(const FKey& _key) const;
	FORCEINLINE TMap<FString, UCategory*> GetSubCategories() const { return SubCategories; }
	FORCEINLINE TArray<FCustomKey> GetKeys() const { return Keys; }

	//void Init(/*const TObjectPtr<UInputMappingContext>& _context,*/ const FEnhancedActionKeyMapping& _keyMapping);
	void AddSubCategory(const FString& _name, const TObjectPtr<UCategory>& SubCategory);
	void AddKey(const FPlayerKeyMapping& _playerKeyMapping, const FString& KeyName, const FKey& Key);
	void ReplaceKey(const FPlayerKeyMapping& _playerKeyMapping, const FString& KeyName, const FKey& Key);
	void GetKeyByIdentifierMapping(const FString& _keyIdentifier, bool& _exist, FCustomKey& _out) const;
};
