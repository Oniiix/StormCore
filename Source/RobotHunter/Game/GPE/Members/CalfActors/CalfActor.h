// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../LegMemberActor.h"
#include "CalfActor.generated.h"

UCLASS(HideDropdown)
class ROBOTHUNTER_API ACalfActor : public ALegMemberActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere)
		float damages = 10;

	bool isUsing = false;

	UPROPERTY(EditAnywhere, Category = "Custom|Property|Mesh")
	TObjectPtr < USkeletalMesh> memberSkeletalMesh;
	UPROPERTY(EditAnywhere, Category = "Custom|Property|Animation")
	TObjectPtr<UClass> animClass;
	 
	
public:	
	FORCEINLINE bool GetIsUsing() const { return isUsing; }
	FORCEINLINE void SetIsUsing(const bool _status) { isUsing = _status; }

	virtual void PressBehavior();
	virtual void ReleaseBehavior();
	virtual void SetupMember() override;

	virtual void ResetMember() override;

};
