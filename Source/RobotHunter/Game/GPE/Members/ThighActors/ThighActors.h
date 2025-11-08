// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../LegMemberActor.h"
#include "ThighActors.generated.h"

/**
 * 
 */
UCLASS(HideDropdown)
class ROBOTHUNTER_API AThighActor : public ALegMemberActor
{
	GENERATED_BODY()

public:
	virtual TArray<USkeletalMeshComponent*> GetMemberMeshs() override { return TArray<USkeletalMeshComponent*>({ rightMemberMesh, leftMemberMesh }); };
};
