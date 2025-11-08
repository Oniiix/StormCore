// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include <RobotHunter/Game/CustomGameInstance.h>
#include "LightningGenerator.generated.h"



class ACustomGameMode;
class ALightning;
UCLASS(Blueprintable)
class ROBOTHUNTER_API ULightningGenerator : public UObject
{
	GENERATED_BODY()

#pragma region Property

	float lightningStrikeFrequency = 1;

	//UPROPERTY(EditAnywhere, meta = (ClampMin = 1)) float modifierlightningStrikeFrequency = 2;
	UPROPERTY(EditAnywhere, meta = (ClampMin = 100)) float rangeAroundPlayer = 300;
	UPROPERTY(EditAnywhere) TSubclassOf< ALightning> lightningRef = nullptr;
	//Secure to avoid generating lightning 2 times in a row
	/*UPROPERTY(VisibleAnywhere) bool isProcessGeneratingLightning = false;*/


	FTimerHandle timerLightningStrikeFrequency;

#pragma endregion Property

#pragma region Method
public:
	FORCEINLINE void SetLightningStrikeFrequency(const float _lightningFrequency) { lightningStrikeFrequency = _lightningFrequency; }
	//Start timer and end of each loop GenerateLightning  
	void StartGenerateLightning();
	//Stop timer after generateLightning
	void StopGenerateLightning();
private:
	//At the end of each loop, spawn lightning 
	UFUNCTION() void GenerateLightning();
#pragma endregion Method
};
