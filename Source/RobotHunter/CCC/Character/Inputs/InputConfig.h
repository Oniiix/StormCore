#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "InputMappingContext.h"
#include "InputConfig.generated.h"

UCLASS()
class ROBOTHUNTER_API UInputConfig : public UDataAsset
{
	GENERATED_BODY()

#pragma region Context
	UPROPERTY(EditAnywhere, Category = "Context")
	TObjectPtr<UInputMappingContext> gameContext = nullptr;

	UPROPERTY(EditAnywhere, Category = "Context")
	TObjectPtr<UInputMappingContext> uiContext = nullptr;

	UPROPERTY(EditAnywhere, Category = "Context")
	TObjectPtr<UInputMappingContext> deadContext = nullptr;


#pragma region Handcar
	/*UPROPERTY(EditAnywhere, Category = "Context|Handcar")
	TObjectPtr<UInputMappingContext> handcarContext;*/

	UPROPERTY(EditAnywhere, Category = "Context|Handcar")
	TObjectPtr<UInputMappingContext> handleHandcarContext;

	UPROPERTY(EditAnywhere, Category = "Context|Handcar")
	TObjectPtr<UInputMappingContext> brakeHandcarContext;

	UPROPERTY(EditAnywhere, Category = "Context|Handcar")
	TObjectPtr<UInputMappingContext> nitroHandcarContext;
#pragma endregion


	UPROPERTY(EditAnywhere, Category = "Context")
	TObjectPtr<UInputMappingContext> gameplayMenuContext;
#pragma endregion Context



	UPROPERTY(EditAnywhere)
	TObjectPtr<UInputAction> move = nullptr;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UInputAction> rotate = nullptr;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UInputAction> rotatePitch = nullptr;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UInputAction> jump = nullptr;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UInputAction> run = nullptr;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UInputAction> interaction = nullptr;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UInputAction> giveUp = nullptr;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UInputAction> escape = nullptr;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UInputAction> pause = nullptr;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UInputAction> rightArm = nullptr;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UInputAction> leftArm = nullptr;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UInputAction> calfs = nullptr;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UInputAction> thighs = nullptr;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UInputAction> dropBag = nullptr;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UInputAction> backBag = nullptr;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UInputAction> scope = nullptr;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UInputAction> pushToTalk = nullptr;
	UPROPERTY(EditAnywhere, Category = "GameplayMenu")
	TObjectPtr<UInputAction> inventory = nullptr;
	UPROPERTY(EditAnywhere, Category = "GameplayMenu")
	TObjectPtr<UInputAction> previous = nullptr;
	UPROPERTY(EditAnywhere, Category = "GameplayMenu")
	TObjectPtr<UInputAction> next = nullptr;
	UPROPERTY(EditAnywhere, Category = "GameplayMenu")
	TObjectPtr<UInputAction> rotateMapCamera = nullptr;
	UPROPERTY(EditAnywhere, Category = "GameplayMenu")
	TObjectPtr<UInputAction> rotateMapCameraPitch = nullptr;
	UPROPERTY(EditAnywhere, Category = "GameplayMenu")
	TObjectPtr<UInputAction> focusMap = nullptr;
	UPROPERTY(EditAnywhere, Category = "GameplayMenu")
	TObjectPtr<UInputAction> zoomMapCamera = nullptr;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UInputAction> wagonStop;


#pragma region Handcar
	UPROPERTY(EditAnywhere, Category = "Handcar")
	TObjectPtr<UInputAction> handcarAcceleration;


	UPROPERTY(EditAnywhere, Category = "Handcar|Brake")
	TObjectPtr<UInputAction> handcarBrake;

	UPROPERTY(EditAnywhere, Category = "Handcar|Brake")
	TObjectPtr<UInputAction> handcarReleaseBrake;


	UPROPERTY(EditAnywhere, Category = "Handcar|Nitro")
	TObjectPtr<UInputAction> handcarNitroLeft;

	UPROPERTY(EditAnywhere, Category = "Handcar|Nitro")
	TObjectPtr<UInputAction> handcarNitroRight;

	/*UPROPERTY(EditAnywhere, Category = "Handcar")
	TObjectPtr<UInputAction> handcarChangeWay;

	UPROPERTY(EditAnywhere, Category = "Handcar")
	TObjectPtr<UInputAction> handcarHorn;*/
#pragma endregion Handcar


#pragma region CHEAT
	UPROPERTY(EditAnywhere)
	TObjectPtr<UInputAction> dead = nullptr;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UInputAction> explo = nullptr;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UInputAction> stealth = nullptr;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UInputAction> fight = nullptr;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UInputAction> shoot = nullptr;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UInputAction> BIG = nullptr;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UInputAction> FAT = nullptr;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UInputAction> HEAVY = nullptr;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UInputAction> MASSIVE = nullptr;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UInputAction> NOWEIGHT = nullptr;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UInputAction> GODMODE_enable = nullptr;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UInputAction> GODMODE_disable = nullptr;

#pragma endregion

public:
	UInputConfig();

#pragma region Context
	FORCEINLINE TSoftObjectPtr<UInputMappingContext> GetGameContextInput() const { return gameContext; }
	FORCEINLINE TSoftObjectPtr<UInputMappingContext> GetUIContextInput() const { return uiContext; }
	FORCEINLINE TSoftObjectPtr<UInputMappingContext> GetGameplayMenuContextInput() const { return gameplayMenuContext; }
	FORCEINLINE TSoftObjectPtr<UInputMappingContext> GetDeadContextInput() const { return deadContext; }

#pragma region Handcar
	//FORCEINLINE TSoftObjectPtr<UInputMappingContext> GetHandcarContextInput() const { return handcarContext; }
	FORCEINLINE TSoftObjectPtr<UInputMappingContext> GetBrakeHandcarContextInput() const { return brakeHandcarContext; }
	FORCEINLINE TSoftObjectPtr<UInputMappingContext> GetHandleHandcarContextInput() const { return handleHandcarContext; }
	FORCEINLINE TSoftObjectPtr<UInputMappingContext> GetNitroHandcarContextInput() const { return nitroHandcarContext; }
#pragma endregion

#pragma endregion Context

	FORCEINLINE UInputAction* GetMoveInput() const { return move; }
	FORCEINLINE UInputAction* GetRotateInput() const { return rotate; }
	FORCEINLINE UInputAction* GetRotatePitchInput() const { return rotatePitch; }
	FORCEINLINE UInputAction* GetJumpInput() const { return jump; }
	FORCEINLINE UInputAction* GetRunInput() const { return run; }
	FORCEINLINE UInputAction* GetInteractionInput() const { return interaction; }
	FORCEINLINE UInputAction* GetGiveUpInput() const { return giveUp; }
	FORCEINLINE UInputAction* GetEscapeInput() const { return escape; }
	FORCEINLINE UInputAction* GetPauseInput() const { return pause; }
	FORCEINLINE UInputAction* GetRightArmInput() const { return rightArm; }
	FORCEINLINE UInputAction* GetLeftArmInput() const { return leftArm; }
	FORCEINLINE UInputAction* GetThighsInput() const { return thighs; }
	FORCEINLINE UInputAction* GetCalfsInput() const { return calfs; }
	FORCEINLINE UInputAction* GetScopeInput() const { return scope; }
	FORCEINLINE UInputAction* GetInventoryInput() const { return inventory; }
	FORCEINLINE UInputAction* GetPreviousInput() const { return previous; }
	FORCEINLINE UInputAction* GetNextInput() const { return next; }
	FORCEINLINE UInputAction* GetDropInput() const { return dropBag; }
	FORCEINLINE UInputAction* GetBackBagInput() const { return backBag; }
	FORCEINLINE UInputAction* GetPushToTalkInput() const { return pushToTalk; }

	FORCEINLINE UInputAction* GetWagonStop() const { return wagonStop; }

#pragma region Handcar
	FORCEINLINE UInputAction* GetHandcarAccelerationInput() const { return handcarAcceleration; }

	FORCEINLINE UInputAction* GetHandcarBrakeInput() const { return handcarBrake; }
	FORCEINLINE UInputAction* GetHandcarReleaseBrakeInput() const { return handcarReleaseBrake; }

	FORCEINLINE UInputAction* GetHandcarNitroLeftInput() const { return handcarNitroLeft; }
	FORCEINLINE UInputAction* GetHandcarNitroRightInput() const { return handcarNitroRight; }

	/*FORCEINLINE UInputAction* GetHandcarChangeWayInput() const { return handcarChangeWay; }
	FORCEINLINE UInputAction* GetHandcarHornInput() const { return handcarHorn; }*/
#pragma endregion Handcar

	FORCEINLINE UInputAction* GetDeadInput() const { return dead; }
	FORCEINLINE UInputAction* GetExploInput() const { return explo; }
	FORCEINLINE UInputAction* GetStealthInput() const { return stealth; }
	FORCEINLINE UInputAction* GetFightInput() const { return fight; }
	FORCEINLINE UInputAction* GetShootInput() const { return shoot; }
	FORCEINLINE UInputAction* GetBIGInput() const { return BIG; }
	FORCEINLINE UInputAction* GetFATInput() const { return FAT; }
	FORCEINLINE UInputAction* GetHEAVYInput() const { return HEAVY; }
	FORCEINLINE UInputAction* GetMASSIVEInput() const { return MASSIVE; }
	FORCEINLINE UInputAction* GetNOWEIGHTInput() const { return NOWEIGHT; }
	FORCEINLINE UInputAction* GetEnableGodModeInput() const { return GODMODE_enable; }
	FORCEINLINE UInputAction* GetDisableGodModeInput() const { return GODMODE_disable; }
	FORCEINLINE UInputAction* GetRotateMapCamera() const { return rotateMapCamera; }
	FORCEINLINE UInputAction* GetRotateMapCameraPitch() const { return rotateMapCameraPitch;}
	FORCEINLINE UInputAction* GetZoomMapCamera() const { return zoomMapCamera;}
	FORCEINLINE UInputAction* GetFocusMap() const { return focusMap;}
};
