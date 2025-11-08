#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include <Components/TextBlock.h>
#include "AIDebugInfoWidget.generated.h"

UCLASS()
class ROBOTHUNTER_API UAIDebugInfoWidget final : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess))
	TObjectPtr<UTextBlock> stateInfo = nullptr;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess))
	TObjectPtr<UTextBlock> targetInfo = nullptr;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess))
	TObjectPtr<UTextBlock> threatInfo = nullptr;

public:
	void InitWidget();

	void SetStateVisibility(const bool _value);
	void SetTargetVisibility(const bool _value);
	void SetThreatVisibility(const bool _value);

	void SetStateInfo(const FText& _info, const FColor& _color) const;
	void SetTargetInfo(const FString& _info) const;
	void SetThreatInfo(const FString& _info) const;
};
