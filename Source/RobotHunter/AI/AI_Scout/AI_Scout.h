#pragma once

#include "CoreMinimal.h"
#include "RobotHunter/AI/AI_Base.h"
//#include "RobotHunter/AI/Component/Attack/Projectile/AttackProjectileComponent.h"
#include "AI_Scout.generated.h"

class AAI_Torpedo;

UCLASS(HideDropdown)
class ROBOTHUNTER_API AAI_Scout : public AAI_Base
{
	GENERATED_BODY()

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStartSpawnTorpedo);
	FOnStartSpawnTorpedo onStartSpawnTorpedo;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStopSpawnTorpedo);
	FOnStopSpawnTorpedo onStopSpawnTorpedo;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess))
	bool isEndSearch = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess))
	bool canEscape = true;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess))
	FVector searchPosition;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess))
	TSubclassOf< AAI_Torpedo>torpedoRef = nullptr;

	UPROPERTY(EditAnywhere, Category = "Custom|Property|Jump", BlueprintReadOnly, meta = (AllowPrivateAccess))
	float timeFly = 3;
	UPROPERTY(VisibleAnywhere, Category = "Custom|Property|Jump", BlueprintReadOnly, meta = (AllowPrivateAccess))
	bool canMarkJump = true;

	UPROPERTY(EditAnywhere, Category = "Custom|Property|Mark")
	TArray<TEnumAsByte< EObjectTypeQuery>> layerObstacle;

public:
	AAI_Scout();

#pragma region MethodEvent
	FORCEINLINE FOnStartSpawnTorpedo& OnStartSpawnTorpedo() 
	{ 
		return onStartSpawnTorpedo;
	}
	FORCEINLINE FOnStopSpawnTorpedo& OnStopSpawnTorpedo()
	{
		return onStopSpawnTorpedo; 
	}
#pragma endregion

#pragma region Getteur
	FORCEINLINE bool GetIsEndSearch()const 
	{
		return isEndSearch;
	}
	FORCEINLINE bool GetCanEscape()const 
	{
		return canEscape;
	}
	FORCEINLINE FVector GetSearchPosition()const 
	{
		return searchPosition;
	}
	FORCEINLINE float GetTimeFly()const
	{
		return timeFly;
	}
	FORCEINLINE TArray<TEnumAsByte< EObjectTypeQuery>> GetLayerObstacle()const
	{
		return layerObstacle;
	}
#pragma endregion

#pragma region Setteur
	FORCEINLINE void SetIsEndSearch(const bool _isEndSearch) 
	{
		isEndSearch = _isEndSearch;
	}
	FORCEINLINE void SetCanEscape(const bool _canEscape)
	{
		canEscape = _canEscape;
	}
	FORCEINLINE void SetSearchPosition(const FVector& _searchPosition)
	{
		searchPosition = _searchPosition;
	}
	FORCEINLINE void SetMarkCanJump(const bool _canMarkJump)
	{
		canMarkJump = _canMarkJump;
	}
#pragma endregion
protected:
	virtual void BeginPlay()override;

public:
	UFUNCTION() void DisableJump();
};
