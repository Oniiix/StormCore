// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RobotHunter/AI/AI_Base.h"
#include "AI_Torpedo.generated.h"

class AArena;
UCLASS(HideDropdown)
class ROBOTHUNTER_API AAI_Torpedo : public AAI_Base
{
	GENERATED_BODY()

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEmerging);
	FOnEmerging onEmerging;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, ReplicatedUsing = On_Emerging, meta = (AllowPrivateAccess))
	bool isEmerging = false;

public:
	AAI_Torpedo();

	FORCEINLINE FOnEmerging& OnEmerging() { return onEmerging; };

private:
	UFUNCTION(BlueprintCallable) void Emerging();
	UFUNCTION() void On_Emerging();
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;
};
