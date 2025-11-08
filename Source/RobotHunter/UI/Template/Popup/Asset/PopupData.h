// Fichier MethodDataAsset.h
#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "RobotHunter/Utils/DebugUtils.h"
#include "PopupData.generated.h"

DECLARE_DYNAMIC_DELEGATE(FVoidDelegate);

UCLASS(BlueprintType)
class ROBOTHUNTER_API UPopupData final : public UDataAsset
{
	GENERATED_BODY()

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnQuit);

	UPROPERTY(BlueprintAssignable, BlueprintCallable, meta = (AllowPrivateAccess))
	FOnQuit onQuit;

	UPROPERTY(EditAnywhere, Category = "Custom")
	FText Text;

	UPROPERTY(EditAnywhere, Category = "Custom")
	FString textFormat = "<normal>{0}</>";

	UPROPERTY(EditAnywhere, Category = "Custom")
	FString optionFormat = "<normal>{0}</>";

	UPROPERTY(VisibleAnywhere, Category = "Custom|Debug")
	TMap<FString, FVoidDelegate> MethodMap = {};

	UPROPERTY(EditAnywhere, Category = "Custom", meta=(UIMin = 0, ClampMin = 0))
	int spaceBetweenElement;

public:
	FORCEINLINE FString GetText() const { return FORMAT(textFormat, Text.ToString()); }
	FORCEINLINE int GetSpaceBetweenElement() const { return spaceBetweenElement; }

	UFUNCTION(BlueprintCallable, Category = "Custom|Popup")
	void BindMethodOption(FText OptionName, const FVoidDelegate& MethodToBind);

	FORCEINLINE TMap<FString, FVoidDelegate> GetEvents() const
	{
		return MethodMap;
	}

	UFUNCTION(BlueprintCallable, Category = "Custom|Popup")
	FORCEINLINE void ClearOptions() { MethodMap.Empty(); }
};
