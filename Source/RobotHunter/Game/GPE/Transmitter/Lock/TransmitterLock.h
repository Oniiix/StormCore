
#pragma once

#include "CoreMinimal.h"
#include "RobotHunter/Game/GPE/Transmitter/TransmitterActor.h"
#include "TransmitterLock.generated.h"


class AKeyLootActor;

UCLASS()
class ROBOTHUNTER_API ATransmitterLock : public ATransmitterActor
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Custom|Property") TObjectPtr<AKeyLootActor> keyToOpen = nullptr;

public:
	ATransmitterLock();
	virtual void Interacted(AMainCharacter* _character) override;

private:
	bool HasKey(AMainCharacter* _character);
};
