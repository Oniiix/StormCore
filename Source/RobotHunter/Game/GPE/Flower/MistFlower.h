// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Flower.h"

#include "MistFlower.generated.h"

class AAI_Torpedo;
//class UNoiseSystemComponent;
class USightSystemGPEComponent;
class UAudioComponent;
UCLASS()
class ROBOTHUNTER_API AMistFlower : public AFlower
{
	GENERATED_BODY()
#pragma region Property
private:
	UPROPERTY(EditAnywhere, Category = "MistFlower|Property", meta = (ClampMin = 10))float loudness = 100.0f;
	UPROPERTY(EditAnywhere, Category = "MistFlower|Property")TSubclassOf<AAI_Torpedo> torpedo = nullptr;
	/*UPROPERTY(EditAnywhere) TObjectPtr<UNoiseSystemComponent > noiseSystem = nullptr;*/
	UPROPERTY(EditAnywhere) TObjectPtr<UAudioComponent> audioSystem = nullptr;
	UPROPERTY(EditAnywhere) TObjectPtr<USightSystemGPEComponent> sightSystem = nullptr;

	bool activateSpawnTorpedo = false;
		
#pragma endregion
public:
	AMistFlower();
#pragma region Methods
private:
	virtual void Init() override;
	UFUNCTION()void ActiveSpawnTorpedo();
	UFUNCTION()void DesactivateSpawnTorpedo();
	UFUNCTION()void PlayerDetected(AActor* _actor);
	void SpawnTorpedo();
	UFUNCTION(Server, Unreliable, WithValidation) void ServerRPC_SpawnTorpedo();
	//UFUNCTION(Server, Unreliable, WithValidation) void ServerRPC_MakeNoise();
	
#pragma endregion
};
