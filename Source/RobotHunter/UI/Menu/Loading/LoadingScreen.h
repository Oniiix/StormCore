// 

#pragma once

#include "CoreMinimal.h"
#include "Components/ProgressBar.h"
#include "RobotHunter/UI/UI.h"
#include "LoadingScreen.generated.h"

/**
 * 
 */
UCLASS()
class ROBOTHUNTER_API ULoadingScreen final : public UUI
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess))
	TObjectPtr<UProgressBar> loadingProgressBar;

public:
	/**
	 * Set the percentage of progress bar to load
	 * @param _percent percentage between 0 and 100
	 */
	UFUNCTION(Blueprintable, BlueprintCallable, Category = "Custom")
	void SetLoadPercent(const float _percent) const;
};
