#pragma once

#include "CoreMinimal.h"
#include "RobotHunter/CustomActorComponent.h"
#include "RobotHunter/Utils/DebugUtils.h"
#include "BonusComponent.generated.h"

class AMainCharacter;
class ULifeComponent;
class UStatusComponent;

UENUM(BlueprintType)
enum EBonusType
{
	DefaultBonus,

	SpeedBonus,
	SpeedAimBonus,

	ProtectionBonus,
	ResistanceBonus,
	CooldownBonus,

	LifeBonus,
	HealBonus,
	RegenBonus,
	ReviveBonus,

	WeaponDamageBonus,
	KickDamageBonus,

	CounterBonus,

	BleedBonus,

	LifeStealBonus,

	BonusTypeCount,

	BonusNone
};

USTRUCT()
struct FBonusCardInformation
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FText nameBonus = TEXTSTR("");

	UPROPERTY(EditAnywhere)
	FText descriptionBonusCard = TEXTSTR("");

	UPROPERTY(EditAnywhere)
	FLinearColor color = FLinearColor::White;

	UPROPERTY(EditAnywhere)
	FSlateBrush brush = FSlateBrush();
};

USTRUCT()
struct FBonusInformationPause
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FText descriptionBonusCard = TEXTSTR("");
};

USTRUCT(BlueprintType)
struct FBonusStruct
{
	GENERATED_BODY()

	UPROPERTY()
	TEnumAsByte<EBonusType> type = BonusNone;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UCurveFloat> modifierFromStackCurve = nullptr;

	UPROPERTY(EditAnywhere, meta = (UIMin = 0.01f, ClampMin = 0.01f))
	float baseModifier;

	UPROPERTY(EditAnywhere, meta = (UIMin = 0.01f, ClampMin = 0.01f))
	float modifierEfficiency;

	UPROPERTY(EditAnywhere, meta = (UIMin = 0.01f, ClampMin = 0.01f))
	float spawnPercent;

	UPROPERTY(EditAnywhere)
	FBonusCardInformation cardInformation;

	UPROPERTY(EditAnywhere)
	FBonusInformationPause informationPause;

	UPROPERTY(VisibleAnywhere)
	float currentBonus;

	UPROPERTY(VisibleAnywhere)
	float currentModifier;

	UPROPERTY(EditAnywhere)
	float a = 0;
	UPROPERTY(EditAnywhere)
	float b = 0;
	UPROPERTY(EditAnywhere)
	float c = 0;

	UPROPERTY(VisibleAnywhere)
	int stack;

	FBonusStruct()
	{
		type = DefaultBonus;

		modifierFromStackCurve = nullptr;

		baseModifier = 1.5f;
		modifierEfficiency = 0.5f;

		spawnPercent = 12.5f;

		currentBonus = 0.0f;
		currentModifier = 0.0f;

		stack = 0;
	}

	float GetFormattedCalculate() const
	{
		UE_LOG(LogTemp, Warning, TEXT("Formula: (%f + %f - %f) * %f"), a, currentBonus, b, c);
		return (a + currentBonus - b) * c;
	}
};

UCLASS()
class ROBOTHUNTER_API UBonusComponent : public UCustomActorComponent
{
	GENERATED_BODY()

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAddBonus, const FBonusStruct&, bonus);

	UPROPERTY(BlueprintAssignable, meta = (AllowPrivateAccess))
	FOnAddBonus onAddBonus;

	UPROPERTY(EditAnywhere, Category = "Custom Property")
	TMap<TEnumAsByte<EBonusType>, FBonusStruct> bonus;

	UPROPERTY(EditAnywhere, Category = "Custom Property", meta = (UIMin = 0.01f, ClampMin = 0.01f))
	float lostSpawnPercent;

	UPROPERTY(EditAnywhere, Category = "Custom Property|Bleed")
	float bleedDuration;

	UPROPERTY(EditAnywhere, Category = "Custom Property|Bleed")
	float bleedRate;

	UPROPERTY()
	TObjectPtr<AMainCharacter> player;

	UPROPERTY()
	TObjectPtr<ULifeComponent> lifeComponent;

	UPROPERTY()
	TObjectPtr<UStatusComponent> statusComponent;

	float baseMaxLife;

public:
	FORCEINLINE void SetBaseMaxLife(const float _baseMaxLife)
	{
		if (_baseMaxLife > 0.0f)
			baseMaxLife = _baseMaxLife;
	}

	FORCEINLINE float GetBaseMaxLife() const { return baseMaxLife; }
	FORCEINLINE TMap<TEnumAsByte<EBonusType>, FBonusStruct> GetBonus() const { return bonus; }

	FORCEINLINE FOnAddBonus& GetOnAddBonus() { return onAddBonus; }

public:
	UBonusComponent();

private:
	void GetPlayerStatusComponent();

	void UpdateBonus(FBonusStruct& _bonus);

	void AddSpeedBonus(const FBonusStruct& _bonus) const;
	void AddSpeedAimBonus(const FBonusStruct& _bonus) const;

	void AddProtectionBonus(const FBonusStruct& _bonus) const;
	void AddResistanceBonus(const FBonusStruct& _bonus) const;
	void AddCooldownBonus(const FBonusStruct& _bonus) const;

	void AddLifeBonus(const FBonusStruct& _bonus) const;
	void AddHealBonus(const FBonusStruct& _bonus) const;
	void AddRegenBonus(const FBonusStruct& _bonus) const;
	void AddReviveBonus(const FBonusStruct& _bonus) const;

	void AddWeaponDamageBonus(const FBonusStruct& _bonus) const;
	void AddKickDamageBonus(const FBonusStruct& _bonus) const;

	void AddCounterBonus(const FBonusStruct& _bonus) const;

	void AddBleedBonus(const FBonusStruct& _bonus) const;

	void AddLifeStealBonus(const FBonusStruct& _bonus) const;

	float GetMaxSpawnPercent() const;

	FBonusStruct GetRandomBonusFromProbabilities(const float _rand, float& _maxPercent,
	                                             TMap<float, FBonusStruct>& _probabilities) const;

protected:
	virtual void BeginPlay() override;

public:
	FBonusStruct GetRandomBonus() const;
	TArray<FBonusStruct> GetRandomBonus(const int _numberOfTypes) const;
	void AddBonus(const EBonusType& _bonusType, const bool _updateBonus = true);
	void SetPlayer(AMainCharacter* _player);

	UFUNCTION(CallInEditor)
	void AddRandomBonus();
	void AddRandomBonus(const int _numberOfBonus);

	void LoadBonus(const TArray<FBonusStruct>& _bonus);
	void RefreshMembersDamageBonus();
};
