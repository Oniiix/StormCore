

#pragma once

#include "CoreMinimal.h"
#include "../CalfActor.h"
#include "SeismicKickDataAsset.h"
#include "EnhancedInputComponent.h"
#include "InputMappingContext.h"
#include "Camera/CameraComponent.h"
#include "SeismicKickActor.generated.h"

UCLASS(HideDropdown)
class ROBOTHUNTER_API ASeismicKickActor : public ACalfActor
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category="Custom|SeismicKick")
	TObjectPtr<USeismicKickDataAsset> settings = nullptr;

	float maxChargeDuration = 3;
	float minChargeDuration = 1;


	float offset = 100;
	float minLength = 100;
	float maxLength = 500;
	FVector boxExtent = FVector(100);
	TSubclassOf<ASBoxActor> boxRef = nullptr;

	FVector direction;

	FTimerHandle chargeTimer;
	FTimerManager* timerManager = nullptr;


	TObjectPtr<UCameraComponent> cam = nullptr;

public:
	//TODO 
	FORCEINLINE bool GetHasPressed() const { return isPressed; }

protected:
	virtual void InitMember() override;
	virtual void PressBehavior() override;
	virtual void ReleaseBehavior() override;
	virtual bool CanUse() override;
	virtual void ResetMember() override;

	virtual void Tick(float _deltaTime) override;
private:
	void SpawnSeismBox(const FVector& _direction, const float _ratio);
	void InitPropertyWithDataAsset();

	UFUNCTION() void ChargeFinish(const float _maxTimer);

	UFUNCTION(Server, Unreliable) void ServerRpc_SpawnSeismBox(const FVector& _direction, const float _ratio);

	void RotateMesh();

	UFUNCTION() void FinishSK();
};
