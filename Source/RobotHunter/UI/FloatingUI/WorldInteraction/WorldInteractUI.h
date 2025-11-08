// Objectif 3D Copyright

#pragma once

#include "CoreMinimal.h"
#include "Components/InputKeySelector.h"
#include "RobotHunter/UI/FloatingUI/FloatingUI.h"
#include "RobotHunter/UI/Template/Prefab/TextPrefab/TextPrefab.h"
#include "Components/Image.h"
#include "WorldInteractUI.generated.h"

class AMainCharacter;
class AInteractableActor;

UCLASS()
class ROBOTHUNTER_API UWorldInteractUI final : public UFloatingUI
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, Category = "Custom")
	FString key;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess))
	TObjectPtr<UInputKeySelector> InputKey = nullptr;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess))
	TObjectPtr<UTextPrefab> interactionText = nullptr;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess))
	TObjectPtr<UImage> rangeImage = nullptr;

	UPROPERTY(VisibleAnywhere)
	FString text;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess))
	bool inSight;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess))
	bool inInteractionZone;

	UPROPERTY()
	TObjectPtr<AMainCharacter> chara = nullptr;

	UPROPERTY()
	TObjectPtr<AInteractableActor> owner = nullptr;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess))
	TObjectPtr<AInteractableActor> touchComponent = nullptr;

	float interactionRange = 0.0f;

	bool isInit = false;

public:
	FORCEINLINE bool IsInit() const { return isInit; }
	FORCEINLINE void SetInteractOwner(AInteractableActor* _owner) { owner = _owner; }

	virtual void Init() override;
	void SetKey(const FText& Text);

	UFUNCTION(BlueprintImplementableEvent) void ShowInteract();
	UFUNCTION(BlueprintImplementableEvent) void ShowInRange();
	UFUNCTION(BlueprintImplementableEvent) void HideInRange();

	virtual void Setup() override;  
protected:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;


private:

	virtual void InitWithDelay() override;

};
