#pragma once

#include "CoreMinimal.h"
#include "RobotHunter/Game/GPE/Flower/V2/FlowerV2.h"
#include "MistFlowerV2.generated.h"

class USightSystemGPEComponent;
class UAudioComponent;
class AAI_Torpedo;

UCLASS()
class ROBOTHUNTER_API AMistFlowerV2 : public AFlowerV2
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, Category = "CustomProperty")
	TObjectPtr<USightSystemGPEComponent> sightSystem;

	UPROPERTY(EditAnywhere, Category = "CustomProperty")
	TObjectPtr<UAudioComponent> audioComponent;

	UPROPERTY(EditAnywhere, Category = "CustomProperty")
	TSubclassOf<AAI_Torpedo> torpedo;

public:
	AMistFlowerV2();

private:
	UFUNCTION() void PlayerDetected(AActor* _actor);

	UFUNCTION(Server, Unreliable) void ServerRPC_SpawnTorpedo();

protected:
	virtual void StartActivePhase() override;
	virtual void Init() override;
};
