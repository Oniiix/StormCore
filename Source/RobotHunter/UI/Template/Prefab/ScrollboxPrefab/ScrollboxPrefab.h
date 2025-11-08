// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ScrollboxPrefabOverrideStyle.h"
#include "Components/ScrollBox.h"
#include "RobotHunter/UI/Template/Prefab/Prefab.h"
#include "ScrollboxPrefab.generated.h"

/**
 * 
 */
UCLASS(HideDropdown)
class ROBOTHUNTER_API UScrollboxPrefab final : public UPrefab
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess))
	TObjectPtr<UScrollBox> scrollbox;

	UPROPERTY(EditAnywhere, Category = "Custom|Style", meta=(EditCondition="bOverrideStyle"))
	TObjectPtr<UScrollboxPrefabOverrideStyle> overrideStyle;

public:
	FORCEINLINE TObjectPtr<UScrollBox>& GetScrollbox() { return scrollbox; }

private:
	virtual void NativePreConstruct() override;
	void LoadStyle() const;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
};
