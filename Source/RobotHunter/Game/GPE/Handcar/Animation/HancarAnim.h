

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include <RobotHunter/Game/GPE/Handcar/CustomHandcarActor.h>
#include "HancarAnim.generated.h"

/**
 * 
 */
UCLASS()
class ROBOTHUNTER_API UHancarAnim : public UAnimInstance
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, Replicated, BlueprintReadWrite, meta = (AllowPrivateAccess))
	TObjectPtr<ANitroHandcarActor> handcar = nullptr;


public:


	void NativeBeginPlay();

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
