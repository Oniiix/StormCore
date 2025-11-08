// 

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RobotHunter/UI/Template/Prefab/TextPrefab/TextPrefab.h"
#include "Subtitle.generated.h"

/**
 * 
 */
UCLASS()
class ROBOTHUNTER_API USubtitle final : public UUserWidget
{
	GENERATED_BODY()

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlaySubtitle, UDataTable*, _data);

	UPROPERTY(BlueprintAssignable, Category = "Custom", meta =(AllowPrivateAccess))
	FOnPlaySubtitle onPlaySubtitle;

public:
	FORCEINLINE FOnPlaySubtitle& OnPlaySubtitle() { return onPlaySubtitle; }
};
