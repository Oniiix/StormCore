// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "EvolutionOnArenaDataTable.generated.h"

USTRUCT(BlueprintType)
struct FEvolutionOnArenaTableRow : public FTableRowBase
{
	GENERATED_USTRUCT_BODY() 

public:
	FEvolutionOnArenaTableRow() = default;

	/** The 'Name' column is the same as the evolution state */

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (UMin = 0, ClampMin = 0, UMax = 30, ClampMax = 30))
	int paraLarvaCount = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (UMin = 0, ClampMin = 0, UMax = 30, ClampMax = 30))
	int trapperCount = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (UMin = 0, ClampMin = 0, UMax = 30, ClampMax = 30))
	int titanCount = 0;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (UMin = 0, ClampMin = 0, UMax = 30, ClampMax = 30))
	//int pennantCount = 0;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (UMin = 0, ClampMin = 0, UMax = 30, ClampMax = 30))
	//int torpedoCount = 0;
};

/**
 * The data table of arena's evolution 
 */
UCLASS()
class ROBOTHUNTER_API UEvolutionOnArenaDataTable : public UDataTable
{
	GENERATED_BODY()
	
public:
	UEvolutionOnArenaDataTable();
};
