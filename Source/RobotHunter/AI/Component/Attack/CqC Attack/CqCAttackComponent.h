#pragma once

#include "CoreMinimal.h"
#include "RobotHunter/Utils/ENUM_TriggerZone.h"
#include "Components/ActorComponent.h"
#include "CqCAttackComponent.generated.h"

#define CQC_SOCKET "CqC_Socket"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class ROBOTHUNTER_API UCqCAttackComponent : public UActorComponent
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CqCAttackComponent_Property", meta = (AllowPrivateAccess))
	TObjectPtr<UShapeComponent> trigger = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CqCAttackComponent_Property", meta = (AllowPrivateAccess))
	EShapeType shapeType = EShapeType::BOX;

public:
	UCqCAttackComponent();

	TObjectPtr<UShapeComponent> GetBoxCQC();

private:
	void Change();

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
};
