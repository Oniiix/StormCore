#pragma once

#include "CoreMinimal.h"
#include "MainHUD.h"
#include "RobotHunter/UI/ATH/Reticle/Reticle.h"
#include "RobotHunter/Utils/ENUM_Reticle.h"
#include "GameHUD.generated.h"
/**
* Struct permettant de renvoyer la position et la direction ou le joueur regarde avec la camera (position in world)
*/


/**
 * 
 */
UCLASS(HideDropdown)
class ROBOTHUNTER_API AGameHUD final : public AMainHUD
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, Category = Custom)
	TMap<EReticleType, TObjectPtr<UReticle>> reticles =
	{
		{EReticleType::LEFT_ARM, nullptr},
		{EReticleType::RIGHT_ARM, nullptr},
	};

public:
	/**
	 * Set the current reticle of weapon
	 * @param _reticleType reticle type of reticle
	 * @param reticle UReticle*
	 */
	FORCEINLINE void SetCrosshair(EReticleType _reticleType, const TObjectPtr<UReticle>& reticle)
	{
		reticles[_reticleType] = reticle;
	}

	FORCEINLINE TObjectPtr<UReticle> GetReticle(EReticleType _reticleType) { return reticles[_reticleType]; }
	FORCEINLINE bool ContainReticle(EReticleType _reticleType) { return reticles.Contains(_reticleType); }
	/**
 * Recovers all weapon aiming points, in world position
 * @return TArray<FPositionViewing>
 */
	TArray<FPositionViewing> GetLocationsViewingPoints(EReticleType _reticleType) const;

	/**
	 * Recover the middle point of screen in world position
	 * @return TArray<FPositionViewing>
	 */
	FPositionViewing GetCenterPoint() const;

	virtual void HideUI() override;
	virtual void ShowUI() override;
	virtual void BeginPlay() override;
};