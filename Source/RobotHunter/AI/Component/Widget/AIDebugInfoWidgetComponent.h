#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "RobotHunter/UI/AI/AIDebugInfoWidget.h"
#include "AIDebugInfoWidgetComponent.generated.h"

#define AI_DEBUG_WIDGET_USE_LOG false

class AMainCharacter;
class AAI_Base;
class AHitDamage;

UCLASS()
class ROBOTHUNTER_API UAIDebugInfoWidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Replicated)
	TSubclassOf<AHitDamage> hitDamage = nullptr;
	UPROPERTY(VisibleInstanceOnly)
	TObjectPtr<AMainCharacter> mainCharacter = nullptr;
	UPROPERTY(VisibleInstanceOnly, Replicated)
	TObjectPtr<UAIDebugInfoWidget> AIDebugInfoWidget = nullptr;
	UPROPERTY(VisibleInstanceOnly, ReplicatedUsing = OnRep_StateInfo) 
	FText stateInfo = FText();
	UPROPERTY(VisibleInstanceOnly, Replicated)
	FColor stateColor = FColor();
	UPROPERTY(VisibleInstanceOnly, ReplicatedUsing = OnRep_TargetInfo)
	FString targetInfo = FString();
	UPROPERTY(VisibleInstanceOnly, ReplicatedUsing = OnRep_ThreatInfo)
	FString threatsInfo = FString();

	AAI_Base* owner = nullptr;
	FVector location;
	FVector cameraLocation;
	bool hitDamageVisibility = false;
	bool isInitializationGood = false;
	
public:
	void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void InitAIDebugInfo();

	UFUNCTION()
	void SetStateVisibility(const bool _value);
	UFUNCTION()
	void SetTargetVisibility(const bool _value);
	UFUNCTION()
	void SetThreatVisibility(const bool _value);
	UFUNCTION()
	FORCEINLINE void SetHitDamageVisibility(const bool _value) { hitDamageVisibility = _value; }

	void SetWidgetVisibility(const bool _value);

	UFUNCTION()
	void AIDamage(AActor* _owner, const int _currentLife, const int _damage, AActor* _damageSource);

	void SetStateInfo(const FText& _info, const FColor& _color);
	void SetTargetInfo(const FString& _info);
	void SetThreatInfo(const TArray<FString>& _infos);

	UFUNCTION()
	void OnRep_StateInfo();
	UFUNCTION()
	void OnRep_TargetInfo();
	UFUNCTION()
	void OnRep_ThreatInfo();

private:
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;
};
