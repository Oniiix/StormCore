// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Image.h"
#include "RobotHunter/UI/UI.h"
#include "RobotHunter/Utils/ENUM_Reticle.h"
#include "Reticle.generated.h"

#define DEBUG_CROSSHAIR false

class AMembersActor;
class ALeftArmActor;
class ARightArmActor;
class UBaseReticle;

USTRUCT()
struct FPositionViewing
{
	GENERATED_BODY()

	FVector position;
	FVector direction;

	FPositionViewing(const FVector& _position = FVector(0), const FVector& _direction = FVector(0))
	{
		position = _position;
		direction = _direction;
	}

	FString ToString() const
	{
		return "Position: " + position.ToString() + "\nDirection: " + direction.ToString();
	}
};

/**
 * 
 */
UCLASS(HideDropdown)
class ROBOTHUNTER_API UReticle : public UUI
{
	GENERATED_BODY()

protected:
	//Set the reticle type
	UPROPERTY(EditAnywhere, Category = Custom)
	EReticleType ReticleType = EReticleType::None;

	//UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess))
	//TObjectPtr<UBaseReticle> baseReticle;

	/*
	*A list of crosshair placements normalized between 0 and 1, based on user screen size.
	*We take the size of the user's screen as a reference.
	*For example, on a 1920 x 1080 screen, 0.5 = 50%, so 0.5 of 1920px = 960px
	*(Default: 0.5, 0.5 = center of screen)
	**/
	UPROPERTY(VisibleAnywhere, Category = Custom, meta = (TitleProperty = "Reticle"))
	TArray<FVector2D> PercentCrosshairInScreen = {FVector2D(.5f, .5f)};

public:
	FORCEINLINE EReticleType GetReticleType() const { return ReticleType; }
	//FORCEINLINE TObjectPtr<UBaseReticle> GetBaseReticle() const { return baseReticle; }

	virtual TArray<FPositionViewing> GetLocationsViewingPoints() const;

protected:
	virtual void NativePreConstruct() override;
	virtual void RegisterEvents() override;

	void ResizeFullScreen(bool bArg) const;
	void Resize(FViewport* Viewport = nullptr, unsigned I = 0) const;
	void UpdatePositionAnchors(FVector2D& _position, const TObjectPtr<UImage>& _crosshair);


	TObjectPtr<ARightArmActor> GetRightMember() const;
	TObjectPtr<ALeftArmActor> GetLeftMember() const;
};
