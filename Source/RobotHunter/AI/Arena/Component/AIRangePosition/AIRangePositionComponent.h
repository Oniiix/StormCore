// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RobotHunter/CCC/Character/MainCharacter.h"
#include "RobotHunter/Utils/ENUM_AI.h"
#include "AIRangePositionComponent.generated.h"

class AArena;

USTRUCT(Blueprintable)
struct FPointRangeArena
{
	GENERATED_BODY()

public:
	FVector position = FVector::ZeroVector;
	bool isAvailable = false;
	int index = -1;

	FPointRangeArena()
	{
		position = FVector::ZeroVector;
		isAvailable = true;
		index = -1;
	}

	FPointRangeArena(const FVector& _position, const bool _isAvailable, const int _index)
		: position(_position), isAvailable(_isAvailable), index(_index)
	{
	}
};

USTRUCT()
struct FListRange
{
	GENERATED_BODY()

public:
	TArray<float> allRanges;

	FListRange() = default;

	FListRange(TArray<float> _allRanges)
	{
		allRanges = _allRanges;
	}
};

USTRUCT()
struct FListPointRangeArena
{
	GENERATED_USTRUCT_BODY()

public:
	TArray<FPointRangeArena> allPointInArena;

	FListPointRangeArena() = default;

	FListPointRangeArena(TArray<FPointRangeArena> _allPointInArena)
	{
		allPointInArena = _allPointInArena;
	}
};

USTRUCT()
struct FMapRangePosition
{
	GENERATED_BODY()

public:
	TMap<TEnumAsByte<ERangeAttack>, FListPointRangeArena> mapRangePosition;

	FMapRangePosition() = default;

	FMapRangePosition(TMap<TEnumAsByte<ERangeAttack>, FListPointRangeArena> _mapRangePosition)
	{
		mapRangePosition = _mapRangePosition;
	}
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class ROBOTHUNTER_API UAIRangePositionComponent : public UActorComponent
{
	GENERATED_BODY()

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnRefreshPoint);

	FOnRefreshPoint onRefreshPoint;


	UPROPERTY(EditAnywhere, Category = "Property|Custom|Range|Short", meta = (UMin = 0, ClampMin = 0))
	float shortRangeMin = 0;
	UPROPERTY(EditAnywhere, Category = "Property|Custom|Range|Short", meta = (UMin = 0, ClampMin = 0))
	float shortRangeMax = 200;

	UPROPERTY(EditAnywhere, Category = "Property|Custom|Range|Middle", meta = (UMin = 0, ClampMin = 0))
	float middleRangeMin = 200;
	UPROPERTY(EditAnywhere, Category = "Property|Custom|Range|Middle", meta = (UMin = 0, ClampMin = 0))
	float middleRangeMax = 600;

	UPROPERTY(EditAnywhere, Category = "Property|Custom|Range|Long", meta = (UMin = 0, ClampMin = 0))
	float longRangeMin = 600;
	UPROPERTY(EditAnywhere, Category = "Property|Custom|Range|Long", meta = (UMin = 0, ClampMin = 0))
	float longRangeMax = 1400;

	UPROPERTY(EditAnywhere, Category = "Property|Custom|Range", meta = (UMin = 0, ClampMin = 0))
	float distanceRefreshPoint = 150;

	UPROPERTY(EditAnywhere, Category = "Property|Custom|Point", meta = (UMin = 0, ClampMin = 0))
	int gap = 100;
	UPROPERTY(EditAnywhere, Category = "Property|Custom|Point")
	TArray<TEnumAsByte<EObjectTypeQuery>> layerObject;
	UPROPERTY(EditAnywhere, Category = "Property|Custom|Point")
	TArray<TEnumAsByte<EObjectTypeQuery>> layerSnap;
	UPROPERTY(EditAnywhere, Category = "Property|Custom|Point", meta = (UMin = 0, ClampMin = 0))
	float heightMax = 300;
	UPROPERTY(EditAnywhere, Category = "Property|Custom|Point")
	float heightMin = -500;
	UPROPERTY(EditAnywhere, Category = "Property|Custom|Point", meta = (UMin = 0, ClampMin = 0))
	bool drawDebug = false;
	UPROPERTY(EditAnywhere, Category = "Property|Custom|Point", meta = (UMin = 0, ClampMin = 0))
	float acceptanceRadiusAI = 50;

	UPROPERTY(EditAnywhere, Category = "Property|Custom|Point|Cover")
	TArray<TEnumAsByte<EObjectTypeQuery>> layerCover;
	UPROPERTY(EditAnywhere, Category = "Property|Custom|Point|Cover")
	float distanceMaxCover = 100;

	UPROPERTY(EditAnywhere, Category = "Property|Custom")
	float distanceAIPlayerMin = 500;

	UPROPERTY()
	TArray<FPointRangeArena> allPoints;
	UPROPERTY()
	TMap<TEnumAsByte<ERangeAttack>, FListRange> allRanges;

	UPROPERTY()
	TEnumAsByte<ERangeAttack> typeRange;
	UPROPERTY()
	TMap<TEnumAsByte<ERangeAttack>, FListPointRangeArena> allPointInArena;
	UPROPERTY()
	TMap<AMainCharacter*, FMapRangePosition> rangePlayers;

	bool playerDetected = false;

	UPROPERTY()
	TMap<AMainCharacter*, FVector> initPlayersPosition;

	UPROPERTY()
	TArray<AMainCharacter*> players;

	TObjectPtr<AArena> owner = nullptr;

	int nbrPlayer = 0;

public:
	UAIRangePositionComponent();

	FORCEINLINE FOnRefreshPoint& OnRefreshPoint()
	{
		return onRefreshPoint;
	}

#pragma region Getter
	FORCEINLINE TArray<FPointRangeArena> GetListPointInRange(const ERangeAttack& _range, const AMainCharacter* _player) const
	{
		return rangePlayers[_player].mapRangePosition[_range].allPointInArena;
	}

	FORCEINLINE float GetAcceptanceRadiusAI() const
	{
		return acceptanceRadiusAI;
	}

	FORCEINLINE float GetMinDistanceRange(const ERangeAttack& _typeRange) const
	{
		return allRanges[_typeRange].allRanges[0];
	}

	FORCEINLINE float GetMaxDistanceRange(const ERangeAttack& _typeRange) const
	{
		return allRanges[_typeRange].allRanges[1];
	}
	/// <summary>
	/// gives the range of the current position
	/// </summary>
	/// <param name="_AILocation"> current position </param>
	/// <param name="_player"> player target </param>
	/// <returns></returns>
	ERangeAttack GetAICurrentRangeOnTarget(const FVector& _AILocation, const AMainCharacter* _player) const;
	/// <summary>
	/// Get a random point available, if not point available get _pointPosition
	/// </summary>
	/// <param name="_pointPosition"> center position </param>
	/// <param name="_length"> length of circle </param>
	/// <returns> position of a point available </returns>
	FVector GetPositionRandomAround(const FVector& _pointPosition, const float _length)const;
	/// <summary>
	/// Get if the position is in Range
	/// </summary>
	/// <param name="_AILocation"> current position </param>
	/// <param name="_player"> player target </param>
	/// <param name="_range"> range max </param>
	/// <returns></returns>
	bool IsAIInOfRange(const FVector& _AILocation, const AMainCharacter* _player, const ERangeAttack& _range = ERangeAttack::Long) const;
	FORCEINLINE bool PointIsAvailable(const FPointRangeArena& _point)const
	{
		return allPoints[_point.index].isAvailable;
	}
#pragma endregion

#pragma region Setter
	/// <summary>
	/// Set the available point
	/// </summary>
	/// <param name="_targetPoint"> target Point </param>
	/// <param name="_isAvailable"> variable set </param>
	void SetAvailablePoint(const FPointRangeArena& _targetPoint, const bool _isAvailable);
#pragma endregion

protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void DrawDebug();

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

private:
	/// <summary>
	/// Spawn all point in arena
	/// </summary>
	void SpawnPoint();

public:
	UFUNCTION()
	void PlayerDetected(FUniqueNetIdRepl _playerNetId);
	UFUNCTION()
	void PlayerLost(FUniqueNetIdRepl _playerNetId);
	/// <summary>
	/// Calculated all the range 
	/// </summary>
	void RangePointPosition();
	/// <summary>
	/// Gives a point in the desired range based on the position
	/// </summary>
	/// <param name="_rangeType"> target range </param>
	/// <param name="_aiLocation"> current position </param>
	/// <param name="_target"> player target </param>
	/// <param name="_canCheckLine"> if is true check if the line </param>
	/// <param name="_offSetShoot"></param>
	/// <returns> point destination </returns>
	FPointRangeArena GetTargetPoint(const ERangeAttack& _rangeType, const FVector& _aiLocation, const AMainCharacter* _target, const bool _canCheckLine = false, const float _offSetShoot = 200);
	/// <summary>
	/// Get point around of the target position
	/// </summary>
	/// <param name="_targetPosition"> target position of the point </param>
	/// <param name="_range"> range target </param>
	/// <param name="_player"> player target </param>
	/// <param name="_canLine"> if is true check if the line </param>
	/// <param name="_offSetShoot"></param>
	/// <returns> point destination </returns>
	FPointRangeArena GetPointToLocation(const FVector& _targetPosition, const ERangeAttack& _range, const AMainCharacter* _player, const bool _canLine = false, const float _offSetShoot = 200);
	/// <summary>
	/// check if the point is still valid
	/// </summary>
	/// <param name="_startPoint"> point check </param>
	/// <param name="_range"> target range </param>
	/// <param name="_target"> player target</param>
	/// <returns></returns>
	bool PointIsValid(const FPointRangeArena& _startPoint, const ERangeAttack& _range, const AMainCharacter* _target) const;
	/// <summary>
	/// Check Collision all Points
	/// </summary>
	/// <param name="_checkCover"> active system cover </param>
	/// <returns></returns>
	TArray<FPointRangeArena> CheckCollisionAllPoint(const bool _checkCover = false);
	/// <summary>
	/// Check collision the point
	/// </summary>
	/// <param name="_pos"> position of point </param>
	/// <returns></returns>
	bool CheckCollision(const FVector& _pos) const;
	/// <summary>
	/// Check system Cover of point
	/// </summary>
	/// <param name="_point"> target point </param>
	/// <param name="_listPoint"> update the list </param>
	void CheckCover(const FPointRangeArena& _point, TArray<FPointRangeArena>& _listPoint) const;
	/// <summary>
	/// Get if the point is Navigable
	/// </summary>
	/// <param name="_position"> target point </param>
	/// <returns> if point if navigable </returns>
	bool PointIsNavigable(const FVector& _position)const;
	//TArray<FPointRangeArena> GetPointAround(const FVector& _actorPosition, const FVector& _boundsActor, AActor* _actor, FBox _box);

};
