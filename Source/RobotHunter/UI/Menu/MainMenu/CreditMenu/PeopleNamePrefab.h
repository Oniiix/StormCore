// 

#pragma once

#include "CoreMinimal.h"
#include "RobotHunter/UI/Template/Prefab/Prefab.h"
#include "RobotHunter/UI/Template/Prefab/TextPrefab/TextPrefab.h"
#include "PeopleNamePrefab.generated.h"

/**
 * 
 */
UCLASS()
class ROBOTHUNTER_API UPeopleNamePrefab : public UPrefab
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess))
	TObjectPtr<UTextPrefab> NamePeople;

public:
	void SetNamePeople(const FString& _name) const { SET_TEXT_STR(NamePeople->GetText(), _name); }
};
