// 

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "RobotHunter/CCC/Character/Components/Bonus/BonusComponent.h"
#include "BonusInformation.generated.h"

USTRUCT(BlueprintType)
struct FBonusInformation
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere)
	TEnumAsByte<EBonusType> type = BonusNone;

	UPROPERTY(EditAnywhere)
	FText nameBonus = TEXTSTR("Bonus Name x{0}");

	UPROPERTY(EditAnywhere)
	FText descriptionBonus = TEXTSTR("Description Bonus with {0} multiplier");

	UPROPERTY(VisibleAnywhere)
	int stack = 0;

	UPROPERTY(VisibleAnywhere)
	float multiplier = 0;


	UPROPERTY(VisibleAnywhere)
	float currentBonus = 0;

	UPROPERTY(VisibleAnywhere)
	FSlateBrush image;

	void SetBonus(const FBonusStruct& _struct)
	{
		stack = _struct.stack;
		multiplier = _struct.currentBonus;
		type = _struct.type;
		image = _struct.cardInformation.brush;
		nameBonus = _struct.cardInformation.nameBonus;
		descriptionBonus = _struct.informationPause.descriptionBonusCard;
		currentBonus = _struct.GetFormattedCalculate();
	}

	void RefreshCurrentBonus(const FBonusStruct& _struct)
	{
		stack = _struct.stack;
		currentBonus = _struct.GetFormattedCalculate();
	}

	bool IsSame(const FBonusStruct& _struct) const
	{
		return type == _struct.type;
	}
};
