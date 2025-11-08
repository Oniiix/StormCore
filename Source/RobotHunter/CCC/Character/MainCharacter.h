// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Inputs/InputConfig.h"
#include "../Camera/Components/CustomCameraComponent.h"
#include "Components/Interaction/InteractionComponent.h"
#include <RobotHunter/Game/CustomGameInstance.h>
#include <RobotHunter/AI/Component/Life/LifeComponent.h>
#include <RobotHunter/Game/GPE/Members/RightArmActors/RightArmActor.h>
#include <RobotHunter/Game/GPE/Members/CalfActors/CalfActor.h>
#include <RobotHunter/Game/GPE/Members/ThighActors/ThighActors.h>
#include <RobotHunter/Game/GPE/Members/LeftArmActors/LeftArmActor.h>
#include "Components/Inventory/InventoryComponent.h"
#include "../../FSM/Components/PlayerFSMComponent.h"
#include "../../Network/Tree/TreeSourceComponent.h"
#include "../../Utils/ENUM_Speed.h"
#include <RobotHunter/Game/GPE/Members/ArmMemberActor.h>
#include <RobotHunter/Game/GPE/Members/LegMemberActor.h>
#include <RobotHunter/Game/GPE/Components/MovableComponent/MovableComponent.h>
#include "RobotHunter/CCC/Character/Components/Status/StatusComponent.h"
#include "RobotHunter/CCC/Character/Components/Bonus/BonusComponent.h"
#include "Components/Movement/CustomCharacterMovementComponent.h"
#include <RobotHunter/Game/Managers/Day/DayManager.h>
#include "MainCharacter.generated.h"

#define MAIN_CHARA_USE_LOG false

class UVoiceChatComponent;
class AWeatherManager;
class UCharaAnim;
class AArena;
class UGameplaySave;
class UMap3D_Manager;

UCLASS()
class ROBOTHUNTER_API AMainCharacter : public ACharacter
{
	GENERATED_BODY()

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnChooseBonus, TArray<FBonusStruct>, _bonus);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnChangeMappingContext, UInputMappingContext*, _currentContext);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInitAnimation);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEnterInCombat);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBeginPlayer);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnQuitMenu);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMove, FVector2D, _axis);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnJump, bool, _status);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnScope, bool, _status);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnScopeSound, bool, _status);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnYawUpdate, float, _axis);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FIsAttacked);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnRotationEnd);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRightArmInit, ARightArmActor*, _rightArm);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLeftArmInit, ALeftArmActor*, _leftArm);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCalfInit, ACalfActor*, _calf);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnChangeMember);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHit, FHitResult, _result);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStun, bool, isStun);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnQuit, const FUniqueNetIdRepl&, _id);

#pragma region member_anim_events
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnUseRightArm, bool, _hasShoot, int, _numAnim);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnUseCalf);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInteract, bool, _status);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnBump, bool, _status, bool, _isBumpByAI);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStrafeTurn, float, _yaw);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnUpdateCurrentArena);

#pragma endregion

	UPROPERTY(EditAnywhere, Category = "Tool")
	bool useTool;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UMaterialInterface> j2Material;

#pragma region anim
	FTimerHandle shootTimer;
	UPROPERTY(EditAnywhere, Category = "Animation")
	float shootTime = 3;

	UPROPERTY(EditAnywhere, Category = "Death", meta = (UMin = 0, ClampMin = 0, Units = "s"))
	float deathTime = 5.0f;
#pragma endregion


#pragma region Map
	UPROPERTY(EditAnywhere)
	TObjectPtr<UMap3D_Manager> map3DManagerComponent = nullptr;
#pragma endregion


#pragma region camera
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera", meta = (AllowprivateAccess))
	TObjectPtr<UCustomCameraComponent> cameraComponent;
	UPROPERTY(EditAnywhere, Category = "Camera", BluePrintReadWrite,
		meta = (AllowPrivateAccess, UIMin = 0, UIMax = 80, ClampMin = 0, ClampMax = 80))
	int cameraClampMax = 80;
	UPROPERTY(EditAnywhere, Category = "Camera", BluePrintReadWrite,
		meta = (AllowPrivateAccess, UIMin = 0, UIMax = 80, ClampMin = 0, ClampMax = 80))
	int cameraClampMin = 80;


	UPROPERTY(EditAnywhere, Category = "Camera")
	float sensibilityX = 1.0f;
	UPROPERTY(EditAnywhere, Category = "Camera")
	float sensibilityY = 1.0f;
	UPROPERTY(VisibleAnywhere, Category = "Camera")
	bool invertX = false;
	UPROPERTY(VisibleAnywhere, Category = "Camera")
	bool invertY = false;

	UPROPERTY(EditAnywhere, Category = "Camera|Controller")
	float gamepadSensibilityX = 1.0f;
	UPROPERTY(EditAnywhere, Category = "Camera|Controller")
	float gamepadSensibilityY = 1.0f;
	UPROPERTY(VisibleAnywhere, Category = "Camera|Controller")
	bool gamepadInvertX = false;
	UPROPERTY(VisibleAnywhere, Category = "Camera|Controller")
	bool gamepadInvertY = false;
#pragma endregion


#pragma region interaction
	UPROPERTY(EditAnywhere, Category = "Interaction")
	TObjectPtr<UInteractionComponent> interactionComponent = nullptr;

	UPROPERTY(EditAnywhere, Category = "Octree")
	TObjectPtr<UTreeSourceComponent> treeSourceComponent = nullptr;
#pragma endregion


#pragma region Inventory
	UPROPERTY(EditAnywhere, Category = "Inventory")
	TObjectPtr<UInventoryComponent> inventoryComponent;
#pragma endregion


#pragma region Network
	UPROPERTY(BlueprintCallable, BlueprintAssignable, meta = (AllowPrivateAccess))
	FOnQuit onQuit;
#pragma endregion


#pragma region inputs
	UPROPERTY(BlueprintCallable, BlueprintAssignable, meta = (AllowPrivateAccess))
	FOnChangeMappingContext onChangeMappingContext;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UInputConfig> inputs = nullptr;
	UPROPERTY(VisibleAnywhere)
	TSoftObjectPtr<UInputMappingContext> currentPlayerContext;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input", meta = (AllowPrivateAccess))
	bool holdScope = true;
	UPROPERTY(VisibleAnywhere)
	bool canMove = true;

	bool disableInput = false;
#pragma endregion


#pragma region FSM_bools
	bool isRunning = true;
	bool inMenu = false;
	bool inInventory = false;
	UPROPERTY(VisibleAnywhere)
	bool inPause = false;
	bool inBlend = false;
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess))
	bool isDown = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess))
	bool inCombat = false;
#pragma endregion
	UPROPERTY(VisibleAnywhere)
	FOnEnterInCombat onEnterInCombat;
	bool inSubMenu = false;


	UPROPERTY(VisibleAnywhere)
	bool isOwner = false;

#pragma region rotation
	UPROPERTY(VisibleAnywhere, Replicated, meta = (AllowPrivateAccess))
	FRotator meshRotation;
	UPROPERTY(EditAnywhere)
	float rotationSpeed = 1000.0f;

	UPROPERTY(EditAnywhere, Category = "Aim")
	float aimRotationSpeed = 400.0f;
	UPROPERTY(EditAnywhere, Category = "Aim")
	float setupAimRotationSpeed = 1000.0f;

	float currentRotationSpeed;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess))
	float targetYaw;
	UPROPERTY(VisibleAnywhere)
	bool canTurn = true;

	FOnRotationEnd onRotationEnd;

	UPROPERTY(EditAnywhere, Category = "Aim", meta = (UMin = 10, ClampMin = 10, UMax = 89, ClampMax = 89))
	float turnClamp = 50.0f;

	FRotator finalRotation;
#pragma endregion
	FVector2D inputDirection;

	TObjectPtr<UCustomGameInstance> gameInstance = nullptr;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UMovableComponent> movableComponent;


	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCustomCharacterMovementComponent> customMovementComponent = nullptr;

#pragma region jump
	UPROPERTY(EditAnywhere, Category = "Jump", BlueprintReadOnly, meta = (AllowPrivateAccess))
	float height = 100;
	UPROPERTY(EditAnywhere, Category = "Jump", BlueprintReadOnly, meta = (AllowPrivateAccess))
	float duration = 2;
	UPROPERTY(EditAnywhere, Category = "Jump", BlueprintReadOnly,
		meta = (UIMin = 0, ClampMin = 0, UIMax = 1, ClampMax = 1, AllowPrivateAccess))
	float alpha = 0.5f;
	FTimerHandle jumpTimer;
	float Ts = 0;

	bool canJump = true;
	float defaultGravityScale = 0;

	FTimerHandle resetTimer;
#pragma endregion

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess))
	TObjectPtr<AWeather> currentWeather = nullptr;

#pragma region Speed
	UPROPERTY(Replicated, VisibleAnywhere, Category = "Speed")
	float alphaSpeed = 0;
	UPROPERTY(EditAnywhere, Category = "Speed")
	TMap<TEnumAsByte<ESpeedMode>, float> speedModes;
	UPROPERTY(VisibleAnywhere, Category = "Speed")
	TMap<TEnumAsByte<ESpeedModifiers>, float> speedModifiers;

	UPROPERTY(EditAnywhere, Category = "Speed")
	float runSpeedBonusModifier = 0.75f;

	UPROPERTY(Replicated)
	TEnumAsByte<ESpeedMode> currentSpeedMode = ESpeedMode::RUN;
	UPROPERTY(Replicated)
	float currentSpeedModifier = ESpeedModifiers::NOMODIFIERS;
	UPROPERTY(EditAnywhere, Category = "Speed")
	float lerpSpeed = 500;
	UPROPERTY(EditAnywhere, Category = "Speed")
	bool useLerpSpeed = true;

#pragma endregion

#pragma region Weight
	UPROPERTY(EditAnywhere, Category = "Speed")
	float maxWeight = 90;
	UPROPERTY(EditAnywhere, Category = "Speed")
	TMap<TEnumAsByte<EWeightState>, float> weightModifiers;
	UPROPERTY(Replicated)
	float currentWeightState = EWeightState::NOWEIGHT;
#pragma endregion

#pragma region members
	UPROPERTY(Replicated, EditAnywhere, Category = "Members | Right")
	TSubclassOf<ARightArmActor> currentRightArm = nullptr;
	UPROPERTY(ReplicatedUsing = OnRep_UsingRightArm, VisibleAnywhere, Category = "Members | Right")
	TObjectPtr<ARightArmActor> usingRightArm = nullptr;

	UPROPERTY(Replicated, EditAnywhere, Category = "Members | Left")
	TSubclassOf<ALeftArmActor> currentLeftArm = nullptr;
	UPROPERTY(ReplicatedUsing = OnRep_UsingLeftArm, VisibleAnywhere, Category = "Members | Left")
	TObjectPtr<ALeftArmActor> usingLeftArm = nullptr;

	UPROPERTY(Replicated, EditAnywhere, Category = "Members | Calfs")
	TSubclassOf<ACalfActor> currentCalf = nullptr;
	UPROPERTY(ReplicatedUsing = OnRep_UsingCalf, VisibleAnywhere, Category = "Members | Calfs")
	TObjectPtr<ACalfActor> usingCalf = nullptr;

	UPROPERTY(Replicated, EditAnywhere, Category = "Members | Thigh")
	TSubclassOf<AThighActor> currentThigh = nullptr;
	UPROPERTY(ReplicatedUsing = OnRep_UsingThigh, VisibleAnywhere, Category = "Members | Thigh")
	TObjectPtr<AThighActor> usingThigh = nullptr;
	UPROPERTY(Replicated, VisibleAnywhere, Category = "Members")
	TObjectPtr<AArmMemberActor> usingArm = nullptr;
	UPROPERTY(Replicated, VisibleAnywhere, Category = "Members")
	TObjectPtr<ALegMemberActor> usingLeg = nullptr;

	UPROPERTY(BlueprintCallable, BlueprintAssignable, meta = (AllowPrivateAccess))
	FOnRightArmInit onRightArmInit;
	UPROPERTY(BlueprintCallable, BlueprintAssignable, meta = (AllowPrivateAccess))
	FOnLeftArmInit onLeftArmInit;
	UPROPERTY(BlueprintCallable, BlueprintAssignable, meta = (AllowPrivateAccess))
	FOnCalfInit onCalfInit;
	UPROPERTY(BlueprintCallable, BlueprintAssignable, meta = (AllowPrivateAccess))
	FOnChangeMember onChangeMember;


	UPROPERTY(VisibleAnywhere)
	TArray<TSubclassOf<AMembersActor>> unlockedMembers;
	UPROPERTY(VisibleAnywhere)
	TArray<TObjectPtr<AMembersActor>> usingMembers;


	UPROPERTY(VisibleAnywhere)
	bool isAiming = false;

	UPROPERTY(EditAnywhere, Category = "Members|Left")
	float leftArmAimDelay = 0.5f;

	UPROPERTY(Replicated, VisibleAnywhere)
	bool isShooting = false;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, meta = (AllowprivateAccess))
	bool isDashing = false;

	UPROPERTY(VisibleAnywhere)
	bool isAimingLocal = false;

	bool canScope = true;
	bool scopeDisable = false;

	FTimerHandle leftArmAimTimer;
#pragma endregion

#pragma region life
	UPROPERTY(EditAnywhere, Category = "Life")
	TObjectPtr<ULifeComponent> lifeComponent = nullptr;

	UPROPERTY(EditAnywhere, Category = "Life")
	bool enableGodMode = false;

#pragma endregion

#pragma region FSM
	UPROPERTY(EditAnywhere, Category = "FSM")
	TObjectPtr<UPlayerFSMComponent> fsmComponent;
#pragma endregion

#pragma region Status
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status", meta = (AllowPrivateAccess))
	TObjectPtr<UStatusComponent> statusComponent;

	bool isStun = false;
#pragma endregion

#pragma region Bonus
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bonus", meta = (AllowPrivateAccess))
	TObjectPtr<UBonusComponent> bonusComponent;

	float speedBonus = 0.0f;
	float speedAimBonus = 0.0f;

	UPROPERTY(BlueprintCallable, BlueprintAssignable, meta = (AllowPrivateAccess))
	FOnChooseBonus onChooseBonus;
#pragma endregion

#pragma region Events
	UPROPERTY(BlueprintCallable, BlueprintAssignable, meta = (AllowPrivateAccess))
	FOnMove onMove;
	UPROPERTY(BlueprintCallable, BlueprintAssignable, meta = (AllowPrivateAccess))
	FOnQuitMenu onQuitMenu;
	UPROPERTY(BlueprintCallable, BlueprintAssignable, meta = (AllowPrivateAccess))
	FOnJump onJump;
	UPROPERTY(BlueprintCallable, BlueprintAssignable, meta = (AllowPrivateAccess))
	FOnScope onScope;
	UPROPERTY(BlueprintCallable, BlueprintAssignable, meta = (AllowPrivateAccess))
	FOnScopeSound onScopeSound;

	UPROPERTY(BlueprintCallable, BlueprintAssignable, meta = (AllowPrivateAccess))
	FOnYawUpdate onYawUpdate;

	UPROPERTY(BlueprintCallable, BlueprintAssignable, meta = (AllowPrivateAccess))
	FIsAttacked isAttacked;

	UPROPERTY(BlueprintCallable, BlueprintAssignable, meta = (AllowPrivateAccess))
	FOnHit onHit;
	UPROPERTY(BlueprintCallable, BlueprintAssignable, meta = (AllowPrivateAccess))
	FOnInteract onInteract;
	UPROPERTY(BlueprintCallable, BlueprintAssignable, meta = (AllowPrivateAccess))
	FOnInitAnimation onInitAnimation;

	UPROPERTY(BlueprintCallable, BlueprintAssignable, meta = (AllowPrivateAccess))
	FOnStun onStun;
	UPROPERTY(BlueprintCallable, BlueprintAssignable, meta = (AllowPrivateAccess))
	FOnBump onBump;
	UPROPERTY(BlueprintCallable, BlueprintAssignable, meta = (AllowPrivateAccess))
	FOnUpdateCurrentArena onUpdateCurrentArena;

#pragma endregion

#pragma region members_anim_events

	UPROPERTY(BlueprintCallable, BlueprintAssignable, meta = (AllowPrivateAccess))
	FOnUseRightArm onUseRightArm;
	UPROPERTY(BlueprintCallable, BlueprintAssignable, meta = (AllowPrivateAccess))
	FOnUseCalf onUseCalf;
	UPROPERTY(BlueprintCallable, BlueprintAssignable, meta = (AllowPrivateAccess))
	FOnStrafeTurn onStrafeTurn;

	UPROPERTY()
	TObjectPtr<UCharaAnim> charaAnim = nullptr;

#pragma endregion

#pragma region Handcar
	bool isUsingHandcar = false;
	bool isUsingNitroHandcar;
#pragma endregion

	UPROPERTY(EditAnywhere)
	TArray<TEnumAsByte<EObjectTypeQuery>> groundLayer;

	UPROPERTY()
	FOnBeginPlayer onBeginPlayer;

#pragma region meteo
	UPROPERTY(EditAnywhere)
	TSubclassOf<AWeatherManager> weatherManagerRef;
	UPROPERTY(Replicated)
	TObjectPtr<AWeatherManager> weatherManager;
	UPROPERTY(EditAnywhere)
	TSubclassOf<ADayManager> dayManagerRef;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess))
	TObjectPtr<ADayManager> dayManager;
#pragma endregion

	UPROPERTY(EditAnywhere, BluePrintReadWrite, meta = (AllowPrivateAccess))
	bool hasStartWCK = false;
#pragma region IA
	TObjectPtr<AArena> currentArena = nullptr;
#pragma endregion
#pragma region forceinline

public:
	FORCEINLINE UInputConfig* GetInputConfig() const { return inputs; }
	FORCEINLINE TSoftObjectPtr<UInputMappingContext> GetCurrentPlayerContext() const { return currentPlayerContext; }

	FORCEINLINE UCustomCharacterMovementComponent* GetCustomMovementComponent() const
	{
		return customMovementComponent;
	}

	FORCEINLINE AWeatherManager* GetWeatherManager() const { return weatherManager; }
	FORCEINLINE UCustomCameraComponent* GetCameraComponent() const { return cameraComponent; }
	UFUNCTION(BlueprintCallable)
	FORCEINLINE ULifeComponent* GetLifeComponent() const { return lifeComponent; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE UInteractionComponent* GetInteractionComponent() const { return interactionComponent; }

	FORCEINLINE UMap3D_Manager* GetMap3D_Manager_Component() const { return map3DManagerComponent; }

	FORCEINLINE UStatusComponent* GetStatusComponent() const { return statusComponent; }
	FORCEINLINE UBonusComponent* GetBonusComponent() const { return bonusComponent; }
	FORCEINLINE UCharaAnim* GetCharaAnim() const { return charaAnim; }

	FORCEINLINE UInventoryComponent* GetInventoryComponent() const { return inventoryComponent; }
	UFUNCTION(BlueprintCallable)
	FORCEINLINE ARightArmActor* GetUsingRightArm() const { return usingRightArm; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE ALeftArmActor* GetUsingLeftArm() const { return usingLeftArm; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE AThighActor* GetUsingThigh() const { return usingThigh; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE ACalfActor* GetUsingCalf() const { return usingCalf; }

	FORCEINLINE TArray<TSubclassOf<AMembersActor>> GetUnlockedMembers() const { return unlockedMembers; }
	FORCEINLINE TArray<TObjectPtr<AMembersActor>> GetUsingMembers() const { return usingMembers; }

	FORCEINLINE ESpeedMode GetSpeedMode() const { return currentSpeedMode; }
	FORCEINLINE float GetSpeedModifier() const { return currentSpeedModifier / 100; }
	FORCEINLINE float GetWeightState() const { return currentWeightState / 100; }

	FORCEINLINE ADayManager* GetDayManager() const { return dayManager; }

	FORCEINLINE float GetSensibilityX() const { return sensibilityX; }
	FORCEINLINE float GetSensibilityY() const { return sensibilityY; }
	FORCEINLINE bool GetInvertX() const { return invertX; }
	FORCEINLINE bool GetInvertY() const { return invertY; }
	FORCEINLINE bool GetGamepadInvertX() const { return gamepadInvertX; }
	FORCEINLINE bool GetGamepadInvertY() const { return gamepadInvertY; }
	FORCEINLINE float GetGamepadSensibilityX() const { return gamepadSensibilityX; }
	FORCEINLINE float GetGamepadSensibilityY() const { return gamepadSensibilityY; }

	FORCEINLINE void SetIsAiming(const bool _isAiming) { isAiming = _isAiming; }
	FORCEINLINE void SetIsDashing(const bool _isDashing) { isDashing = _isDashing; }
	FORCEINLINE void SetIsShooting(const bool _isShooting) { isShooting = _isShooting; }
	FORCEINLINE void SetIsDown(const bool _isDown) { isDown = _isDown; }
	FORCEINLINE void SetInCombat(const bool _inCombat) { inCombat = _inCombat; }
	FORCEINLINE void SetInMenu(const bool _status) { inMenu = _status; }
	FORCEINLINE void SetInSubMenu(const bool _inSubMenu) { inSubMenu = _inSubMenu; }
	FORCEINLINE void SetInPause(const bool _status) { inPause = _status; }
	FORCEINLINE void SetInBlend(const bool _inBlend) { inBlend = _inBlend; }
	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SetCanJump(const bool _canJump) { canJump = _canJump; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SetScopeDisable(const bool _value) { scopeDisable = _value; }

	FORCEINLINE void SetHoldScope(const bool _status) { holdScope = _status; }
	FORCEINLINE void SetHasStartWCK(const bool _status) { hasStartWCK = _status; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SetCanMove(const bool _status) { canMove = _status; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SetCanTurn(const bool _status) { canTurn = _status; }

	FORCEINLINE void SetIsUsingHandcar(const bool _isUsing) { isUsingHandcar = _isUsing; }
	FORCEINLINE void SetIsUsingNitroHandcar(const bool _isUsing) { isUsingNitroHandcar = _isUsing; }

	UFUNCTION(BlueprintCallable)
	void SetMeshRotation(FRotator _rotation, const bool _canTurn, const bool _world = true);
	FORCEINLINE float GetTargetYaw() const { return targetYaw; }
	FORCEINLINE FRotator GetMeshRotation() const { return meshRotation; }

	FORCEINLINE FVector2D GetInputDirection() const { return inputDirection; }

	FORCEINLINE bool GetIsAiming() const { return isAiming; }
	FORCEINLINE bool GetIsAimingLocal() const { return isAimingLocal; }
	FORCEINLINE bool GetIsShooting() { return isShooting; }
	FORCEINLINE bool GetIsDashing() const { return isDashing; }
	FORCEINLINE bool GetIsDown() const { return isDown; }
	FORCEINLINE bool GetInCombat() const { return inCombat; }
	FORCEINLINE bool GetInMenu() const { return inMenu; }
	FORCEINLINE bool GetInSubMenu() const { return inSubMenu; }
	FORCEINLINE bool GetInPause() const { return inPause; }
	FORCEINLINE bool GetInInventory() const { return inInventory; }
	FORCEINLINE bool GetDisableInput() const { return disableInput; }
	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SetDisableInput(const bool _statut) { disableInput = _statut; }

	FORCEINLINE bool GetInBlend() const { return inBlend; }
	UFUNCTION(BlueprintCallable)
	FORCEINLINE bool GetIsOwner() const { return isOwner; }

	FORCEINLINE bool GetHoldScope() const { return holdScope; }
	FORCEINLINE bool GetCanMove() const { return canMove; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE bool GetIsUsingHandcar() const { return isUsingHandcar; }


	UFUNCTION(BlueprintCallable)
	FORCEINLINE bool GetIsUsingNitroHandcar() const { return isUsingNitroHandcar; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SetSpeedMode(const ESpeedMode& _mode) { currentSpeedMode = _mode; }

	UFUNCTION(BlueprintCallable, Server, UnReliable)
	void ServerRpc_SetClientSpeedMode(const ESpeedMode& _mode);

	FORCEINLINE void SetSpeedBonus(const float _bonus) { speedBonus = _bonus; }
	FORCEINLINE void SetSpeedAimBonus(const float _bonus) { speedAimBonus = _bonus; }

	FORCEINLINE FOnMove& OnMove() { return onMove; }
	FORCEINLINE FOnRotationEnd& OnRotationEnd() { return onRotationEnd; }
	FORCEINLINE FOnJump& OnJump() { return onJump; }
	FORCEINLINE FOnScope& OnScope() { return onScope; }
	FORCEINLINE FOnYawUpdate& OnYawUpdate() { return onYawUpdate; }
	FORCEINLINE FOnRightArmInit& OnRightArmInit() { return onRightArmInit; }
	FORCEINLINE FOnLeftArmInit& OnLeftArmInit() { return onLeftArmInit; }
	FORCEINLINE FOnCalfInit& OnCalfInit() { return onCalfInit; }
	FORCEINLINE FOnChangeMember& OnChangeMember() { return onChangeMember; }
	FORCEINLINE FIsAttacked& IsAttacked() { return isAttacked; }
	FORCEINLINE FOnHit& OnHit() { return onHit; }
	FORCEINLINE FOnUseRightArm& OnUseRightArm() { return onUseRightArm; }
	FORCEINLINE FOnUseCalf& OnUseCalf() { return onUseCalf; }
	FORCEINLINE FOnInitAnimation& OnInitAnimation() { return onInitAnimation; }
	FORCEINLINE FOnInteract& OnInteract() { return onInteract; }
	FORCEINLINE FOnBeginPlayer& OnBeginPlayer() { return onBeginPlayer; }
	FORCEINLINE FOnStun& OnStun() { return onStun; }
	FORCEINLINE FOnQuit& OnQuit() { return onQuit; }
	FORCEINLINE FOnBump& OnBump() { return onBump; }
	FORCEINLINE FOnStrafeTurn& OnStrafeTurn() { return onStrafeTurn; }
	FORCEINLINE FOnChooseBonus& OnChooseBonus() { return onChooseBonus; }
	FORCEINLINE FOnEnterInCombat& GetOnEnterInCombat() { return onEnterInCombat; }
	FORCEINLINE FOnChangeMappingContext& OnChangeMappingContext() { return onChangeMappingContext; }


	FORCEINLINE UCustomGameInstance* GetCustomGameInstance() const
	{
		return Cast<UCustomGameInstance>(GetGameInstance());
	}

	FORCEINLINE bool IsFighting() { return gameInstance->GetGameStatus() == EGameStatus::FIGHTING; }
	FORCEINLINE bool IsInStealth() { return gameInstance->GetGameStatus() == EGameStatus::STEALTH; }

#pragma endregion

public:
	AMainCharacter(const FObjectInitializer& ObjectInitializer);

	void EnablePlayerContext(TSoftObjectPtr<UInputMappingContext> _context);
	void DisablePlayerContext(TSoftObjectPtr<UInputMappingContext> _context);

	void PlayerIsAttacked();

private:

public:
	virtual void BeginPlay() override;

	UFUNCTION()
	void StartPlay();

	void InitDayManager();

	void LoadSaves();

	UFUNCTION()
	void InitAnimInstance(UCharaAnim* _anim);

	void InitWeatherManager();

	void BeginPlayer();

	UFUNCTION(BlueprintCallable)
	void SetupMembers(class ACustomPlayerController* _playerController);
	void LoadCalf(UGameplaySave* _save, ACustomPlayerController* _playerController, size_t _i);
	void LoadThigh(UGameplaySave* _save, ACustomPlayerController* _playerController, size_t _i);
	void LoadLeftArm(UGameplaySave* _save, ACustomPlayerController* _playerController, size_t _i);
	void LoadRightArm(UGameplaySave* _save, ACustomPlayerController* _playerController, size_t _i);
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual bool ShouldTickIfViewportsOnly() const override;

	void StartMove(const FInputActionValue& _value);
	void Move(const FInputActionValue& _value);
	void CancelMove(const FInputActionValue& _value);
	void UpdateMovement(const FVector2D& _newValue, const FVector& _moveDir);
	UFUNCTION(Server, UnReliable)
	void ServerRpc_UpdateMovement(const FVector2D& _newValue, const FVector& _moveDir);
	UFUNCTION(NetMulticast, UnReliable)
	void MultiRpc_UpdateMovement(const FVector2D& _newValue, const FVector& _moveDir);
	void StopMove(const FInputActionValue& _value);
	float FindAngle(const FVector2D& _target);
	void Turn(const float _targetYaw);

	bool CheckGoodRotation(FRotator& _finalRotation);
	UFUNCTION()
	void ResetGravityScale(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	                       FVector NormalImpulse, const FHitResult& Hit);

public:
	void Interact(const FInputActionValue& _value);
	UFUNCTION(Server, UnReliable)
	void ServerRpc_Interact(const FInputActionValue& _value);
	UFUNCTION(NetMulticast, UnReliable)
	void MultiRpc_Interact(const FInputActionValue& _value);
	UFUNCTION()
	FORCEINLINE FOnQuitMenu& OnQuitMenu() { return onQuitMenu; }

	void QuitMenu();
	void EscapeMenu(const FInputActionValue& _value);

#pragma region Life

	UFUNCTION(BlueprintCallable)
	void EnableGodMode();
	UFUNCTION(BlueprintCallable)
	void DisableGodMode();
#pragma endregion

#pragma region Speed
	void UpdateSpeed();

	float LerpSpeed(float _currentSpeed, float _result);

	float GetAlphaSpeed();

#pragma endregion

#pragma region Run
	void Run(const FInputActionValue& _value);
	void Run_Internal();
	UFUNCTION(Server, UnReliable)
	void ServerRpc_Run();
	void Run_Hold(const FInputActionValue& _value);
#pragma endregion

#pragma region Jump
	void CustomJump(const FInputActionValue& _value);
	UFUNCTION(BlueprintCallable)
	void GoodJump(float _alpha, float _duration, float _height);
	void Jump_Internal(float _alpha, float _duration, float _height);
	UFUNCTION(Server, UnReliable)
	void ServerRpc_CustomJump(float _alpha, float _duration, float _height);
	UFUNCTION(NetMulticast, UnReliable)
	void MultiRpc_CustomJump(float _alpha, float _duration, float _height);
	UFUNCTION(NetMulticast, UnReliable)
	void MultiRpc_EndJump();
	UFUNCTION(NetMulticast, UnReliable)
	void MultiRpc_ResetGravity();

	void CheckForJumpBuffer();

	//Add By Thibaud => Cancel the current Jump
	virtual void StopJumping() override;

#pragma endregion

#pragma region Bump
	UFUNCTION(BlueprintCallable)
	void BumpPlayer(const FVector& _bumpSource, const bool _isBumpByIA = true);
	UFUNCTION(BlueprintCallable)
	void EndBump();
#pragma endregion

#pragma region Rotation
	void Rotate(const FInputActionValue& _value);
	void RotatePitchCamera(const FInputActionValue& _value);
	UFUNCTION(Server, UnReliable)
	void ServerRpc_UpdateRotation(const FRotator& _rotation);


	void SetupAimStarted();
	UFUNCTION()
	void SetupAimFinished();
#pragma endregion

#pragma region Weight
	UFUNCTION(Server, UnReliable)
	void ServerRpc_SetWeightState(const EWeightState& _state);
#pragma endregion

#pragma region Modifiers
	UFUNCTION(Server, UnReliable)
	void ServerRpc_AddModifier(const ESpeedModifiers& _modifier);
	UFUNCTION(Server, UnReliable)
	void ServerRpc_RemoveModifier(const ESpeedModifiers& _modifier);

	UFUNCTION(Server, UnReliable)
	void ServerRPC_SetModifier(const ESpeedModifiers& _modifier);

	UFUNCTION(Server, UnReliable)
	void ServerRpc_SetCurrentSpeedModifier(const ESpeedModifiers& _modifier, const float _percent);
	UFUNCTION(Server, UnReliable)
	void ServerRpc_ResetCurrentSpeedModifier();
#pragma endregion

#pragma region members

	void ConstructArmMembers(TSubclassOf<AArmMemberActor> _currentMember);
	void ConstructLegMembers(TSubclassOf<ALegMemberActor> _currentMember);
	void SaveMembers();
	UFUNCTION()
	void SetRightArm(TSubclassOf<ARightArmActor> _rightArm);
	UFUNCTION(Server, UnReliable)
	void ServerRpc_SetClientRightArm(TSubclassOf<ARightArmActor> _rightArm);
	UFUNCTION()
	void SetLeftArm(TSubclassOf<ALeftArmActor> _leftArm);
	UFUNCTION(Server, UnReliable)
	void ServerRpc_SetClientLeftArm(TSubclassOf<ALeftArmActor> _leftArm);
	UFUNCTION()
	void SetCalf(TSubclassOf<ACalfActor> _calf);
	UFUNCTION(Server, UnReliable)
	void ServerRpc_SetClientCalf(TSubclassOf<ACalfActor> _calf);
	UFUNCTION()
	void SetThigh(TSubclassOf<AThighActor> _thigh);
	UFUNCTION(Server, UnReliable)
	void ServerRpc_SetClientThigh(TSubclassOf<AThighActor> _thigh);

	void Scope(const FInputActionValue& _value);
	bool CanScope();
	UFUNCTION(NetMulticast, Unreliable)
	void MultiRpc_Scope(const bool _value);
	UFUNCTION(Server, Unreliable)
	void ServerRpc_Scope(const bool _value);
	void ScopeRelease(const FInputActionValue& _value);
	UFUNCTION(NetMulticast, Unreliable)
	void MultiRpc_ScopeRelease();
	UFUNCTION(Server, Unreliable)
	void ServerRpc_ScopeRelease();

	void ActivateScope();
	void DeactivateScope(const bool _canHold);


	void RightArmStart(const FInputActionValue& _value);
	void RightArm(const FInputActionValue& _value);
	UFUNCTION(NetMulticast, UnReliable)
	void MultiRpc_RightArm(const FInputActionValue& _value);
	UFUNCTION(Server, UnReliable)
	void ServerRpc_RightArm(const FInputActionValue& _value);
	void RightArmRelease(const FInputActionValue& _value);

	void StopShoot();
	UFUNCTION(NetMulticast, UnReliable)
	void MultiRpc_StopShoot();
	UFUNCTION(Server, UnReliable)
	void ServerRpc_StopShoot();


	void LeftArmStart(const FInputActionValue& _value);
	void LeftArm(const FInputActionValue& _value);
	UFUNCTION(NetMulticast, UnReliable)
	void MultiRpc_LeftArm(const FInputActionValue& _value);
	UFUNCTION(Server, UnReliable)
	void ServerRpc_LeftArm(const FInputActionValue& _value);
	void LeftArmRelease(const FInputActionValue& _value);

	void Calf(const FInputActionValue& _value);
	void CalfRelease(const FInputActionValue& _value);

	void Thigh(const FInputActionValue& _value);
	void ThighRelease(const FInputActionValue& _value);

	UFUNCTION()
	void OnRep_UsingRightArm(ARightArmActor* _oldArm);
	UFUNCTION()
	void OnRep_UsingLeftArm(ALeftArmActor* _oldArm);
	UFUNCTION()
	void OnRep_UsingCalf(ACalfActor* _oldArm);
	UFUNCTION()
	void OnRep_UsingThigh(AThighActor* _oldArm);

	UFUNCTION(BlueprintCallable)
	void DeactivateMembers(const TArray<AMembersActor*>& _exceptionMembers, const bool _resetMembers = true);
	UFUNCTION(BlueprintCallable)
	void ReactivateMembers(const TArray<AMembersActor*>& _exceptionMembers);
#pragma endregion

#pragma region membres_anim

	UFUNCTION(BlueprintCallable)
	bool HasNailGun();
	UFUNCTION(BlueprintCallable)
	bool HasFireflies();
	UFUNCTION(BlueprintCallable)
	bool HasSnapFlash();
	UFUNCTION(BlueprintCallable)
	bool HasZBB();
	UFUNCTION(BlueprintCallable)
	bool HasHauler();
	UFUNCTION(BlueprintCallable)
	bool HasRoller();
	UFUNCTION(BlueprintCallable)
	bool HasEjectKick();
	UFUNCTION(BlueprintCallable)
	bool HasWCK();
	UFUNCTION(BlueprintCallable)
	bool HasSK();
#pragma endregion

#pragma region Cameras

	void SetCameraExploMode();
	void SetCameraHUBMode();
	void SetCameraStealthMode();
	void SetCameraFightMode();
	void SetCameraShootMode(FString _cameraKey);

	FORCEINLINE void SetCameraSensibilityX(const float _newValue, const bool _isGamepad = false)
	{
		if (_isGamepad) gamepadSensibilityX = _newValue;
		else sensibilityX = _newValue;
	}

	FORCEINLINE void SetCameraSensibilityY(const float _newValue, const bool _isGamepad = false)
	{
		if (_isGamepad) gamepadSensibilityY = _newValue;
		else sensibilityY = _newValue;
	}

	FORCEINLINE void SetCameraInvertX(const bool _newValue, const bool _isGamepad = false)
	{
		if (_isGamepad) gamepadInvertX = _newValue;
		else invertX = _newValue;
	}

	FORCEINLINE void SetCameraInvertY(const bool _newValue, const bool _isGamepad = false)
	{
		if (_isGamepad) gamepadInvertY = _newValue;
		else invertY = _newValue;
	}

#pragma endregion

#pragma region CHEAT

public:
	void SetDeadMode(const FInputActionValue& _value);
	UFUNCTION()
	void LaunchDeathTransition();
	UFUNCTION()
	void ToHub();
	UFUNCTION()
	void SetDeath();
	void SetExploMode(const FInputActionValue& _value);
	void SetStealthMode(const FInputActionValue& _value);
	void SetFightMode(const FInputActionValue& _value);
	void SetShootMode(const FInputActionValue& _value);

	void SetBIG(const FInputActionValue& _value);
	void SetFAT(const FInputActionValue& _value);
	void SetHEAVY(const FInputActionValue& _value);
	void SetMASSIVE(const FInputActionValue& _value);
	void SetNOWEIGHT(const FInputActionValue& _value);


	UPROPERTY(EditAnywhere, Category = "Editor")
	bool inEditor = false;
	UPROPERTY(EditAnywhere, Category = "Editor|Members")
	TSubclassOf<ARightArmActor> rightArmEditor;
	UPROPERTY(EditAnywhere, Category = "Editor|Members")
	TSubclassOf<ALeftArmActor> leftArmEditor;
	UPROPERTY(EditAnywhere, Category = "Editor|Members")
	TSubclassOf<ACalfActor> calfEditor;
	UPROPERTY(EditAnywhere, Category = "Editor|Members")
	TSubclassOf<AThighActor> thighEditor;

	UFUNCTION(CallInEditor, Category = "Editor|Members")
	void AddMemberToChara();
	UFUNCTION(CallInEditor, Category = "Editor|Members")
	void ResetCharaMember();
#pragma endregion

#pragma region VoiceChat
	//UFUNCTION(Server,Reliable) void ServerRpc_InitVoiceChat();
	//UFUNCTION(NetMulticast,Reliable) void MultiRpc_InitVoiceChat();
	void PlayerDestroyVoiceChat();
	UFUNCTION(Server, Reliable)
	void ServerRpc_DestroyVoiceChat();
	void PlayerInitVoiceChat();
	void CreateUI();
	virtual void OnPlayerStateChanged(APlayerState* NewPlayerState, APlayerState* OldPlayerState) override;
	void PushToTalk(const FInputActionValue& _value);
#pragma endregion

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;
	void GiveUp(const FInputActionValue& _value);
	void Pause(const FInputActionValue& _value);
	void OpenInventory(const FInputActionValue& _value);
	void Drop(const FInputActionValue& _value);
	void SetCurrentArena(AArena* _arena);
	UFUNCTION()
	void InitArenaCamera();
	UFUNCTION()
	void SetupEndCamera(AAlienDoor* exitGate, const bool _giveBonus);

	UFUNCTION(BlueprintCallable)
	FHitResult GetGroundInformation();

public:
	void Destroyed() override;
	void Stun(const float _stunDuration);

	void ResetChara();

	UFUNCTION(BlueprintImplementableEvent)
	void OnSwitchPauseState(const bool _state);
#pragma region Achievement Steam
	void UnlockAchievement(const FName& _achievementName);
#pragma endregion
};
