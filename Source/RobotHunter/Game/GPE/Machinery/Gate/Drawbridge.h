

#pragma once

#include "CoreMinimal.h"
#include "RobotHunter/Game/GPE/Machinery/Receiver.h"
#include "Drawbridge.generated.h"

UCLASS()
class ROBOTHUNTER_API ADrawbridge : public AReceiver
{
	GENERATED_BODY()

	/*UPROPERTY(EditAnywhere, Category = "Drawbridge| Property", meta = (ClampMin = 1.0f)) float timeToOpen =1;
	UPROPERTY(EditAnywhere, Category = "Drawbridge| Property", meta = (ClampMin = 1.0f)) float timeToClose = 1;
	UPROPERTY(VisibleAnywhere, Category = "Drawbridge| Property") bool openGate = false;
	UPROPERTY(VisibleAnywhere, Category = "Drawbridge| Property") bool closeGate = false;
	UPROPERTY(EditAnywhere, Category = "Drawbridge| Property") bool activateRotation = false;
	UPROPERTY(EditAnywhere)TObjectPtr<UStaticMeshComponent> drawBridge = nullptr;
	float roll =0, yaw =0;*/

public:
	ADrawbridge();

//protected:
//	virtual void BeginPlay() override;
//	virtual void Tick(float DeltaTime) override;
//private:
//	void Move();
//
//	void Open();
//	void Close();
//
//	FRotator GetRotationForGate(const bool& _closeGate);
//
//	float GetRotationDuringOpening(const float& _a, const float& _calculeAngle);
//	float GetRotationDuringClosing(const float& _a, const float& _calculeAngle);
//
//	float CalculateAngle(const float& _a, const float& _calculeAngle, const bool& _inverse);
//
//	UFUNCTION() void HowSwitch();
//public:
};
