#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"
#include "AlienDoor.generated.h"

#define DOOR_USE_LOG false

class AArena;

UCLASS()
class ROBOTHUNTER_API AAlienDoor : public AActor
{
	GENERATED_BODY()

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnOpen);
	UPROPERTY(BlueprintAssignable)
	FOnOpen onOpen;
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnClose);
	UPROPERTY(BlueprintAssignable)
	FOnClose onClose;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AlienDoor_Property", meta = (AllowPrivateAccess))
	TObjectPtr<USceneComponent> root = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AlienDoor_Property", meta = (AllowPrivateAccess))
	TObjectPtr<UBoxComponent> collisionBox = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AlienDoor_Property", meta = (AllowPrivateAccess))
	TObjectPtr<UBoxComponent> triggerBox = nullptr;

	UPROPERTY(EditAnywhere, Category = "AlienDoor_Property")
	TObjectPtr<UWidgetComponent> endDoorWidget = nullptr;
	UPROPERTY(EditAnywhere, Category = "AlienDoor_Property")
	float delayDisableCollision = 4.5f; // match animation

	FName defaultCollisionProfile;
	TObjectPtr<AArena> arena = nullptr;
	bool isClosed = false;
	bool isInitializationGood = false;
	
public:	
	AAlienDoor();

	void BeginPlay() override;

	FORCEINLINE FOnOpen& OnOpen() { return onOpen; }
	FORCEINLINE FOnClose& OnClose() { return onClose; }
	FORCEINLINE void SetArena(AArena* _arena) { arena = _arena; }

	UFUNCTION()
	void TriggerBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void OpenDoor();
	void ClearDoor();
	UFUNCTION()
	void CloseDoor();
	UFUNCTION()
	void DisableCollision();

	UFUNCTION()
	void ShowWidget();
	UFUNCTION()
	void HideWidget();
};
