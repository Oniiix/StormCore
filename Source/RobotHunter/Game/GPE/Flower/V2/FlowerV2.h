#pragma once

#include "CoreMinimal.h"
#include "RobotHunter/CustomActor.h"
#include "RobotHunter/Game/CustomGameInstance.h"
#include "FlowerV2.generated.h"

UCLASS(Abstract)
class ROBOTHUNTER_API AFlowerV2 : public ACustomActor
{
	GENERATED_BODY()

#pragma region Event 
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnActivate);
	FOnActivate onActivate;
#pragma endregion
	
protected:
#pragma region Cooldown
	UPROPERTY(EditAnywhere, Category = "Custom Property|Cooldown")
	bool useRandomCooldown;

	UPROPERTY(EditAnywhere, Category = "Custom Property|Cooldown", meta = (UIMin = 0.1f, ClampMin = 0.1f, EditCondition = "!useRandomCooldown", EditConditionHides))
	float cooldown;

	UPROPERTY(EditAnywhere, Category = "Custom Property|Cooldown", meta = (UIMin = 0.1f, ClampMin = 0.1f, EditCondition = "useRandomCooldown", EditConditionHides))
	float minCooldown;

	UPROPERTY(EditAnywhere, Category = "Custom Property|Cooldown", meta = (UIMin = 0.1f, ClampMin = 0.1f, EditCondition = "useRandomCooldown", EditConditionHides))
	float maxCooldown;

	FTimerHandle cooldownTimer;
#pragma endregion

	bool isActive;
	FTimerHandle activeTimer;

public:
	AFlowerV2();

protected:
	template<typename WeatherType>
	bool TestWeather();

	virtual void StartActivePhase();
	virtual void StartCooldownPhase();

	virtual void GoodWeatherCallback();
	virtual void WrongWeatherCallback();

	virtual void BeginPlay() override;
	virtual void DrawDebug() override;

	virtual void Init() {}
	virtual void EnableModifiers() {}
	virtual void DisableModifiers() {}
};

template<typename WeatherType>
inline bool AFlowerV2::TestWeather()
{
	UCustomGameInstance* _gi = GetWorld()->GetGameInstance<UCustomGameInstance>();

	if (_gi)
	{
		TSubclassOf<AWeather> _weather = _gi->GetCurrentWeather();

		if (_weather)
		{
			if (_weather->IsChildOf<WeatherType>())
			{
				GoodWeatherCallback();
				return true;
			}
		}
	}

	WrongWeatherCallback();
	return false;
}
