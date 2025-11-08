// 

#pragma once

#include "CoreMinimal.h"
#include "RobotHunter/UI/FloatingUI/FloatingUI.h"
#include "RobotHunter/UI/Template/Prefab/TextPrefab/TextPrefab.h"
#include "HitValue.generated.h"

/**
 * 
 */
UCLASS()
class ROBOTHUNTER_API UHitValue final : public UFloatingUI
{
	GENERATED_BODY()

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSpawn, const float, _duration);

	UPROPERTY(BlueprintAssignable, BlueprintCallable, meta = (AllowPrivateAccess))
	FOnSpawn onSpawn;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess))
	TObjectPtr<UTextPrefab> text;

	/**
	 * Set the color from damage
	 */
	UPROPERTY(EditAnywhere, Category = "Custom")
	TMap<int, FLinearColor> colorByDamage;

	UPROPERTY(EditAnywhere, Category = "Custom|Critical")
	FText textCritical = TEXTSTR("<title>{0}</>");

	UPROPERTY(EditAnywhere, Category = "Custom|Critical")
	FLinearColor colorCritical = FLinearColor::Red;

	bool critical = false;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Custom", meta = (AllowPrivateAccess))
	int damage = 0;

public:
	virtual void NativeConstruct() override;

	void Spawn(const float _lifeTime) const;
	void SetTextDamage(int _damage, bool _critical = false);

private:
	FLinearColor GetColorFromDamage() const;
};
