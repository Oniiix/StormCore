// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CamouflageStats.generated.h"

/**
 * 
 */
UCLASS()
class ROBOTHUNTER_API UCamouflageStats : public UDataAsset
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere)
	TArray<UStaticMesh*> allMeshs;

public:
	FORCEINLINE TArray<UStaticMesh*> GetAllMesh()const
	{
		return allMeshs;
	}

	FORCEINLINE UStaticMesh* GetMesh(int _index)const
	{
		return allMeshs[_index];
	}
};
