// Objectif 3D Copyright

#pragma once

#include "CoreMinimal.h"
#include "RobotHunter/Game/GPE/Members/LeftArmActors/LeftArmActor.h"
#include "RobotHunter/AI/AI_Base.h"
#include "RobotHunter/CCC/Character/Components/Status/StatusComponent.h"
#include "Components/SpotLightComponent.h"
#include "SnapFlashActor.generated.h"

/**
 * 
 */
UCLASS(HideDropdown)
class ROBOTHUNTER_API ASnapFlashActor : public ALeftArmActor
{
	GENERATED_BODY()

#pragma region Event
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnExplosion, const bool, _hit, const FVector&, _position, const FVector&, _direction, FHitResult, _impactResult);
	UPROPERTY(BlueprintReadWrite, BlueprintAssignable, meta = (AllowPrivateAccess))
	FOnExplosion onExplosion;
#pragma endregion

#pragma region PropertyDistance
	UPROPERTY(EditAnywhere, Category = "Custom|Property|SnapFlash|DistanceCharge")
	float distanceMin = 150;
	UPROPERTY(EditAnywhere, Category = "Custom|Property|SnapFlash|DistanceCharge")
	float distanceMax = 500;
	float currentDistance = 0;
#pragma endregion

#pragma region PropertyCone
	UPROPERTY(EditAnywhere, Category = "Custom|Property|SnapFlash|Cone", BlueprintReadWrite, meta = (AllowPrivateAccess))
	float lengthConeStun = 150;
	UPROPERTY(EditAnywhere, Category = "Custom|Property|SnapFlash|Cone", BlueprintReadWrite, meta = (AllowPrivateAccess))
	float lengthConeStagger = 300;
#pragma endregion

#pragma region Angle
	UPROPERTY(EditAnywhere, Category = "Custom|Property|SnapFlash|Angle", BlueprintReadWrite, meta = (AllowPrivateAccess))
	float angleStragger = 45;
	UPROPERTY(EditAnywhere, Category = "Custom|Property|SnapFlash|Angle", BlueprintReadWrite, meta = (AllowPrivateAccess))
	float angleStun = 45;
#pragma endregion

#pragma region PropertyLayer
	UPROPERTY(EditAnywhere, Category = "Custom|Property|SnapFlash|Layer")
	TArray<TEnumAsByte< EObjectTypeQuery>> layerSnap;
	UPROPERTY(EditAnywhere, Category = "Custom|Property|SnapFlash|Layer")
	TArray<TEnumAsByte< EObjectTypeQuery>> layerCone;
	UPROPERTY(EditAnywhere, Category = "Custom|Property|SnapFlash|Layer")
	TArray<TEnumAsByte< EObjectTypeQuery>> layerObstacle;
#pragma endregion

	UPROPERTY(EditAnywhere, Category = "Custom|Property|SnapFlash|Status")
	FStatusParams staggerParams = FStatusParams(EStatusType::STAGGER, 100.0f, 0.5f);

	UPROPERTY(EditAnywhere, Category = "Custom|Property|SnapFlash|Status")
	FStatusParams stunParams = FStatusParams(EStatusType::STUN, 0.0f, 3.0f);

	UPROPERTY(EditAnywhere, Category = "Custom|Property|SnapFlash|Status")
	float delayStun = 3;


	UPROPERTY()
	TObjectPtr<UStatusComponent> statusComp = nullptr;

	TObjectPtr<USpotLightComponent> spotLight = nullptr;

	TObjectPtr< UCameraComponent> cam = nullptr;

protected:
	virtual void BeginPlay() override;
	virtual void InitMember()override;
	virtual void ReleaseBehavior() override;

private:
	void SpawnCone(const FVector& _impactPoint, const FVector& _direction);
	void Shoot(const FVector& _direction, const FVector& _targetPosition, const FHitResult& _result);
	void StunAI(AAI_Base* _ai,const FVector& _pointImpact);
	void StaggerAI(AAI_Base* _ai,const FVector& _pointImpact);
	// RPC
	UFUNCTION(Unreliable,Server) void ServerRpc_Shoot(const FVector& _direction, const FVector& _targetPosition, const FHitResult& _result);
	UFUNCTION(Unreliable,NetMulticast, WithValidation) void ClientRpc_CallEvent(const bool _hit, const FVector& _position, const FVector& _direction, const FHitResult& _result);
	float AngleTest(const FVector& _u, const FVector& _v);
};
