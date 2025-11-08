#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "RobotHunter/Utils/DebugUtils.h"
#include "RobotHunter/Utils/ENUM_AI.h"
#include "AIsAttackDataAsset.generated.h"

class AProjectile;
class UAttackCQCDataAsset;

USTRUCT()
struct FAICQCAttack
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	TObjectPtr<UAttackCQCDataAsset> CQCAttack = nullptr;
	UPROPERTY(EditAnywhere, meta = (UMin = 0, ClampMin = 0))
	int token = 1;
	UPROPERTY()
	int maxToken = 0;
	UPROPERTY(EditAnywhere, meta = (UMin = 0.f, ClampMin = 0.f))
	float generationDelay = 3.f;
	UPROPERTY(EditAnywhere, meta = (UMin = 0.f, ClampMin = 0.f))
	float randomDeviation = 0.f;
	UPROPERTY(EditAnywhere)
	TArray<TEnumAsByte<EAIType>> listAIsAllowed;

	FAICQCAttack()
	{
		maxToken = token;
	}
};

USTRUCT()
struct FAIDistanceAttack
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	TSubclassOf<AProjectile> projectileRef = nullptr;
	UPROPERTY(EditAnywhere, meta = (UMin = 0, ClampMin = 0))
	int token = 1;
	UPROPERTY()
	int maxToken = 0;
	UPROPERTY(EditAnywhere, meta = (ClampMin = 0.f))
	float generationDelay = 3.f;
	UPROPERTY(EditAnywhere, meta = (ClampMin = 0.f))
	float randomDeviation = 0.f;
	UPROPERTY(EditAnywhere)
	TArray<TEnumAsByte<EAIType>> listAIsAllowed;

	FAIDistanceAttack()
	{
		maxToken = token;
	}
};

/**
 * The data asset that store AI's attack and handle token use
 */
UCLASS()
class ROBOTHUNTER_API UAIsAttackDataAsset : public UDataAsset
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TArray<FAICQCAttack> listShortRangeAttack;
	UPROPERTY(EditAnywhere)
	TArray<FAIDistanceAttack> listMidRangeAttack;
	UPROPERTY(EditAnywhere)
	TArray<FAIDistanceAttack> listLongRangeAttack;

public:
	FORCEINLINE TArray<FAICQCAttack> GetListShortRangeAttack() { return listShortRangeAttack; }
	FORCEINLINE TArray<FAIDistanceAttack> GetListMidRangeAttack() { return listMidRangeAttack; }
	FORCEINLINE TArray<FAIDistanceAttack> GetListLongRangeAttack() { return listLongRangeAttack; }

	bool CheckAvailability();
	void GetToken(const ERangeAttack& _range, const int _index);
	UFUNCTION()
	void GenerateToken(const ERangeAttack& _range, const int _index);
};
