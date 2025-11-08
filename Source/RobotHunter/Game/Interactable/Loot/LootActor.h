#pragma once

#include "CoreMinimal.h"
#include "../InteractableActor.h"
#include "LootActor.generated.h"

#define LOOT_USE_LOG false


UCLASS(HideDropdown)
class ROBOTHUNTER_API ALootActor : public AInteractableActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere)
	FString dropName;

	UPROPERTY(EditAnywhere)
	bool isEgg = false;

public:
#pragma region Getter
	FORCEINLINE FString GetDropName() { return dropName; }
#pragma endregion
	
#pragma region Setter
	
	FORCEINLINE void SetDropName(const FString& _key) { dropName = _key; }
#pragma endregion

public:
	ALootActor();

public:
	virtual void Interacted(AMainCharacter* _character)override;
	virtual void LaunchAnimation(AMainCharacter* _character);
};
