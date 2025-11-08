#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include <Components/TextBlock.h>
#include "HitDamageWidget.generated.h"

/**
 * 
 */
UCLASS()
class ROBOTHUNTER_API UHitDamageWidget final : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess))
	TObjectPtr<UTextBlock> damageValue = nullptr;

public:
	void InitWidget(const int _damage);
};
