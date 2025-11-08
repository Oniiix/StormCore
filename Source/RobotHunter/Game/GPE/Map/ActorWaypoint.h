#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RobotHunter/Utils/DebugUtils.h"
#include "ActorWaypoint.generated.h"

USTRUCT(BlueprintType, Blueprintable)
struct FCustomVector4D
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float X = 0.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Y = 0.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Z = 0.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float W = 0.0f;

	FCustomVector4D(){}

};

USTRUCT()
struct FWaypointInformation
{
	GENERATED_BODY()
	//Name of waypoint
	UPROPERTY(EditAnywhere)
	FText name = TEXTSTR("title_waypoint");

	//Description of waypoint
	UPROPERTY(EditAnywhere)
	FText description = TEXTSTR("description_waypoint");

	//Vector4 to get coordinate and color
	UPROPERTY(EditAnywhere)
	TArray<FCustomVector4D> coordinateColor =
		{ FCustomVector4D(), FCustomVector4D() };
};


UCLASS()
class ROBOTHUNTER_API AActorWaypoint : public AActor
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Custom")
	FWaypointInformation waypointInfo;

public:
	FWaypointInformation GetWaypointInfo() const { return waypointInfo; }

private:
	/**
	 * Get the array of vector 4 custom, with X, Y, Z and W value
	 * @return Array of FCustomVector4D
	 */
	UFUNCTION(BlueprintCallable, Category = "Custom") FORCEINLINE TArray<FCustomVector4D> GetWaypointsCoordinatesAndColor() const
	{
		return waypointInfo.coordinateColor;
	}
};
 