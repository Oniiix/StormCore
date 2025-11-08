

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "BumpBoxComponent.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = "Collision", hidecategories = (Object, LOD, Lighting, TextureStreaming), editinlinenew, meta = (DisplayName = "BumpBoxCollision", BlueprintSpawnableComponent))
class ROBOTHUNTER_API UBumpBoxComponent : public UBoxComponent
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, Category = "Bump", meta = (UMin = 0, ClampMin = 0, Units = "cm"))
	float bumpUpForce = 5000;
	UPROPERTY(EditAnywhere, Category = "Bump", meta = (UMin = 0, ClampMin = 0, Units = "cm"))
	float bumpForwardForce = 5000;
	UPROPERTY(EditAnywhere, Category = "Bump", meta = (UMin = 0, ClampMin = 0))
	float bumpGravity = 5.0f;
	UPROPERTY(EditAnywhere, Category = "Bump", meta = (UMin = 0, ClampMin = 0, Units = "cm"))
	float bumpDeceleration = 100.0f;


	UPROPERTY()
	TObjectPtr<AActor> owner = nullptr;

public:
	void Bump(class AMainCharacter* _chara, const float _ratio = 1);

protected:
	virtual void BeginPlay() override;

private:
	
	UFUNCTION() void OnBumpLanded(ACharacter* _character);

	FORCEINLINE FVector GetOwnerLocation() const { return owner->GetActorLocation(); }
	FORCEINLINE FVector GetOwnerUpVector() const { return owner->GetActorUpVector(); }
};
