#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TutoManager.generated.h"

#define TUTO_MANAGER_USE_LOG true

class AArena;
class AAI_Hunter;
class AMainCharacter;
class ARightArmActor;
class ALeftArmActor;
class ACalfActor;
class AThighActor;

UCLASS()
class ROBOTHUNTER_API ATutoManager : public AActor
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Tuto_Property|Arena")
	TObjectPtr<AArena> paralarvaArena = nullptr;
	UPROPERTY(EditAnywhere, Category = "Tuto_Property|Arena", meta = (Umin = 1, ClampMin = 1))
	int paralarvaToKill = 5;
	/*UPROPERTY(EditAnywhere, Category = "Tuto_Property|Arena")
	TObjectPtr<AAI_Hunter> trapper = nullptr;
	UPROPERTY(EditAnywhere, Category = "Tuto_Property|Arena", meta = (Umin = 0.1f, ClampMin = 0.1f, UMax = 1.f, ClampMax = 1.f))
	float trapperLifePercent = 0.5f;*/

	UPROPERTY(VisiBleAnywhere, Category = "Tuto_Property|Chara")
	TObjectPtr<AMainCharacter> mainCharacter = nullptr;
	UPROPERTY(EditAnywhere, Category = "Tuto_Property|Chara")
	bool startWithJump = false;
	UPROPERTY(EditAnywhere, Category = "Tuto_Property|Chara", meta = (UMin = 1.0f, ClampMin = 1.0f))
	float startLife = 50.0f;

	UPROPERTY(EditAnywhere, Category = "Tuto_Property|Chara|Members")
	TSubclassOf<ARightArmActor> rightArmRef = nullptr;
	UPROPERTY(EditAnywhere, Category = "Tuto_Property|Chara|Members")
	TSubclassOf<ALeftArmActor> leftArmRef = nullptr;
	UPROPERTY(EditAnywhere, Category = "Tuto_Property|Chara|Members")
	TSubclassOf<ACalfActor> calfRef = nullptr;
	UPROPERTY(EditAnywhere, Category = "Tuto_Property|Chara|Members")
	TSubclassOf<AThighActor> thighRef = nullptr;

	int countDeadParalarva = 0;
	bool isInitializationGood = false;
	
public:	
	ATutoManager();

private:
	void BeginPlay() override;
	UFUNCTION() void InitTutoManager();
	
	//UFUNCTION()
	//void SetTrapper();
	UFUNCTION()
	void CountDeadParalarva();
	UFUNCTION()
	void CharaDown();
	void ClearParalarvaArena();
};
