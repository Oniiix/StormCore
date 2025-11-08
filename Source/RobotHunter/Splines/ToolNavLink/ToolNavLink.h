// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RobotHunter/Splines/CustomSplineActor.h"
#include "RobotHunter/AI/JumpAI/NavLinkJumpAI.h"
#include "RobotHunter/Splines/CustomSplineComponent/CustomSplineComponent.h"
#include "RobotHunter/Utils/ENUM_AI.h"
#include "ToolNavLink.generated.h"

UCLASS()
class ROBOTHUNTER_API AToolNavLink : public ACustomSplineActor
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Spline", BlueprintReadWrite, meta = (AllowPrivateAccess))
	TObjectPtr<UCustomSplineComponent> startSpline = nullptr;
	UPROPERTY(EditAnywhere, Category = "Property");
	TSubclassOf<ANavLinkJumpAI> navLink = nullptr;
	UPROPERTY(EditAnywhere, Category = "Property")
	float timeAddPoint = 1;

	UPROPERTY()
	TArray<TObjectPtr<ANavLinkJumpAI>> allNavLink;

	bool canUseTimer = true;

public:
	AToolNavLink();

	FORCEINLINE float GetTimeAddPoint() const { return timeAddPoint; }
	FORCEINLINE bool GetCanUseTimer() const { return canUseTimer; }

	UFUNCTION(CallInEditor, Category = "ToolsNavLink")
	void SpawnNavLink();
	UFUNCTION(CallInEditor, Category = "ToolsNavLink")
	void ClearAllNavLink();
	UFUNCTION(CallInEditor, Category = "ToolsNavLink")
	void RefreshStartSpline();

	void AddPointSpline(const int _index);
	void RemovePointSpline(const int _index);

private:
	void MovePointNavLink(ANavLinkJumpAI* _navLink, const int _indexSpline);
	void RefreshPoints()override;
	void SnapSplinePoint()override;
	void SnapSpline(USplineComponent* _spline) override;
};
