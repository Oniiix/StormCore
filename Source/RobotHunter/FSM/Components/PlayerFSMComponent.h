#pragma once

#include "CoreMinimal.h"
#include "FSMComponent.h"
#include "PlayerFSMComponent.generated.h"

class AMainCharacter;

UCLASS()
class ROBOTHUNTER_API UPlayerFSMComponent : public UFSMComponent
{
	GENERATED_BODY()
	
	UPROPERTY()
	TObjectPtr<AMainCharacter> player;

public:
	FORCEINLINE void SetPlayer(AMainCharacter* _player) { player = _player; }

	FORCEINLINE AMainCharacter* GetPlayer() const { return player; }

public:
	UPlayerFSMComponent();
};
