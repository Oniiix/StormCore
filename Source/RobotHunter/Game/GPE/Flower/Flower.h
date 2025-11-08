// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RobotHunter/CustomActor.h"
#include "RobotHunter/Game/GameMode/PrincipalGameMode.h"

#include "Flower.generated.h"


UCLASS()
class ROBOTHUNTER_API AFlower : public ACustomActor
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditAnywhere, Category = "Flower|Timer Settings")float cooldown = 3.f;
	/// <summary>
	/// Duration of effects/Activation of the plant
	/// </summary>
	UPROPERTY(EditAnywhere, Category = "Flower|Timer Settings")float effectDuration = 4.f;
	UPROPERTY(VisibleAnywhere)bool isActive = false;
	UPROPERTY(EditAnywhere, Category = "Flower|Property") bool desactivateModifier = false;

#pragma region Getteur/Setter
public:
	bool IsActive() const { return isActive; }
#pragma endregion
public:	
	AFlower();
protected:
	virtual void BeginPlay() override;
	/// <summary>
	/// Time between each activation of the flower by lightning.
	/// </summary>
	UFUNCTION()virtual void CooldownPhase();
	/// <summary>
	/// Time between each deactivation of the flower by lightning.
	/// </summary>
	UFUNCTION()virtual void ActivePhase();

	bool IsItThisWeather(EMeteoStatus _statue);
	virtual void Init() {};
};





