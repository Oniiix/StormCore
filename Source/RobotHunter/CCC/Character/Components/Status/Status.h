#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "RobotHunter/Utils/ENUM_Speed.h"
#include "Status.generated.h"

class UStatusComponent;

UENUM(BlueprintType)
enum EStatusType
{	
	SLOW,
	MARK,
	RES_MODIF,
	STAGGER,
	DOT,
	PROTECT,
	STUN,
	CD_MODIF,
	DEACTIVATE,
	FREEZE,
	BLEED,

	DEFAULT_STATUS
};

USTRUCT()
struct FStatusParams
{
	GENERATED_BODY()

	UPROPERTY()
	bool useStack = false;

	UPROPERTY(EditAnywhere, meta = (UIMin = 0.1f, ClampMin = 0.1f))
	float value = 0.1f;

	UPROPERTY(EditAnywhere, meta = (UIMin = -1.0f, ClampMin = -1.0f, EditCondition = "!useStack", EditConditionHides))
	float duration = -1.0f;


	UPROPERTY(EditAnywhere)
	TEnumAsByte<EStatusType> type = EStatusType::DEFAULT_STATUS;

	UPROPERTY(EditAnywhere, meta = (EditCondition = "type == EStatusType::RES_MODIF", EditConditionHides))
	TEnumAsByte<EStatusType> resistanceType = EStatusType::DEFAULT_STATUS;


	bool useSlowType = true;

	UPROPERTY(EditAnywhere, meta = (EditCondition = "type == EStatusType::SLOW", EditConditionHides))
	TEnumAsByte<ESpeedModifiers> slowType = ESpeedModifiers::NOMODIFIERS;


	UPROPERTY(EditAnywhere, meta = (UIMin = -180.0f, ClampMin = -180.0f, UIMax = 180.0f, ClampMax = 180.0f, EditCondition = "type == EStatusType::PROTECT", EditConditionHides))
	float shieldDirectionalAngle = 0.0f;

	UPROPERTY(EditAnywhere, meta = (UIMin = 0.0f, ClampMin = 0.0f, UIMax = 360.0f, ClampMax = 360.0f, EditCondition = "type == EStatusType::PROTECT", EditConditionHides))
	float shieldAngle = 0.0f;


	UPROPERTY(EditAnywhere, meta = (UIMin = 0.1f, ClampMin = 0.1f, EditCondition = "useStack", EditConditionHides))
	float stackDuration = 1.0f;

	UPROPERTY(EditAnywhere, meta = (UIMin = 1, ClampMin = 1, EditCondition = "useStack", EditConditionHides))
	int maxStack = 1;

	int currentStack = 1;


	FStatusParams() = default;

	FStatusParams(const EStatusType& _type, const float _value, const float _duration, const EStatusType& _resistanceType = EStatusType::DEFAULT_STATUS, 
		const ESpeedModifiers& _slowType = ESpeedModifiers::NOMODIFIERS, const bool _useSlowType = true)
	{
		useStack = false;

		value = _value;
		duration = _duration;

		type = _type;
		resistanceType = _resistanceType;

		useSlowType = _useSlowType;
		slowType = _slowType;
	}

	FStatusParams(const EStatusType& _type, const float _shieldValue, const float _shieldDuration, const float _shieldDirectionalAngle, const float _shieldAngle, 
		const EStatusType& _resistanceType = EStatusType::DEFAULT_STATUS, const ESpeedModifiers& _slowType = ESpeedModifiers::NOMODIFIERS, const bool _useSlowType = true)
	{
		useStack = false;

		value = _shieldValue;
		duration = _shieldDuration;

		type = _type;
		resistanceType = _resistanceType;

		useSlowType = _useSlowType;
		slowType = _slowType;

		shieldDirectionalAngle = _shieldDirectionalAngle;
		shieldAngle = _shieldAngle;
	}

	FStatusParams(const EStatusType& _type, const float _stackDuration, const int _maxStack, const float _value, const EStatusType& _resistanceType = EStatusType::DEFAULT_STATUS,
		const ESpeedModifiers& _slowType = ESpeedModifiers::NOMODIFIERS, const bool _useSlowType = true)
	{
		useStack = true;

		value = _value;

		type = _type;
		resistanceType = _resistanceType;

		useSlowType = _useSlowType;
		slowType = _slowType;

		stackDuration = _stackDuration;
		maxStack = _maxStack;
	}
};

UCLASS()
class ROBOTHUNTER_API UStatus : public UObject
{
	GENERATED_BODY()

protected:
	UPROPERTY()
	TObjectPtr<UStatusComponent> statusComponent;

	UPROPERTY()
	TObjectPtr<AActor> source;

	bool isApplied;
	FStatusParams params;
	FTimerHandle statusTimer;

public:
	FORCEINLINE void SetParams(const FStatusParams& _params) { params = _params; }
	FORCEINLINE void SetStatusComponent(UStatusComponent* _comp) { statusComponent = _comp; }
	FORCEINLINE void SetSource(AActor* _source) { source = _source; }
	FORCEINLINE void SetIsApplied(const bool _isApplied) { isApplied = _isApplied; }

	FORCEINLINE void SetValue(const float _value) { params.value = _value; }

	FORCEINLINE bool GetIsApplied() const { return isApplied; }
	FORCEINLINE EStatusType GetType() const { return params.type; }
	FORCEINLINE EStatusType GetResistanceType() const { return params.resistanceType; }

public:
	UStatus();
	UStatus(const FStatusParams& _params);

protected:
	virtual void StatusEnded();

public:
	void ClearStatusTimer();
	bool CheckIfCanBeApplied(const float _maxValue = -2.0f);

	virtual void ApplyStatus(AActor* _actor);
	virtual void UnapplyStatus(AActor* _actor);

	virtual void RefreshStatus(const bool _sourceCooldownPhase, AActor* _actor = nullptr);
	virtual void StartStatusTimer(const float _duration = -1.0f);

	virtual float GetValue() const;
};
