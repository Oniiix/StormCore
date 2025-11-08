#include "MainCharacter.h"
#include <EnhancedInputComponent.h>
#include "GameFramework/CharacterMovementComponent.h"
#include <EnhancedInputSubsystems.h>
#include "GameFramework/PawnMovementComponent.h"
#include <Net/UnrealNetwork.h>
#include "RobotHunter/UI/HUD/MainHUD.h"
#include "RobotHunter/Game/CustomGameState.h"
#include "RobotHunter/Network/Tree/TreeManagerSubSystem.h"
#include "RobotHunter/Utils/DebugUtils.h"
#include "RobotHunter/Network/VoiceChat/Component/VoiceChatComponent.h"
#include "../../Game/GPE/Nest/Egg.h"
#include "RobotHunter/Game/Interactable/Loot/Key/KeyLootActor.h"
#include "RobotHunter/SaveSystem/SaveManagerSubsystem.h"
#include "RobotHunter/SaveSystem/SettingsSave.h"
#include <RobotHunter/Game/Managers/Weather/WeatherManager.h>
#include "RobotHunter/CCC/Character/Animations/CharaAnim.h"
#include "RobotHunter/CCC/CustomPlayerController.h"
#include <RobotHunter/Game/Interactable/Handcar/LifeStockHandcarActor.h>
#include <RobotHunter/Game/Interactable/Handcar/EggStockHandcarActor.h>
#include "RobotHunter/UI/Menu/Hub/StockMenu/NewStock.h"
#include <RobotHunter/Game/GPE/Members/CalfActors/SeismicKick/SeismicKickActor.h>
#include <RobotHunter/Game/GPE/Members/CalfActors/WorldChampionsKick/WorldChampionsKickActor.h>
#include "RobotHunter/Utils/CustomBlueprintLibrary.h"
#include "RobotHunter/Utils/CustomMathBlueprintLibrary.h"
#include "CommonInputSubsystem.h"
#include <RobotHunter/SaveSystem/GameplaySave.h>
#include <RobotHunter/AI/Arena/Arena.h>
#include "EngineUtils.h"
#include "RobotHunter/Network/OnlineManagerSubsystem.h"
#include "RobotHunter/Game/GPE/Nest/AlienDoor.h"
#include "RobotHunter/Game/GPE/Map/NewMap3D/Map3D_Manager.h"
#include "RobotHunter/InputSystem/InputSubsystem.h"
#include "RobotHunter/UI/ATH/EndDay/EndDay.h"


AMainCharacter::AMainCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UCustomCharacterMovementComponent>(
		ACharacter::CharacterMovementComponentName))
{
	PrimaryActorTick.bCanEverTick = true;

#if WITH_EDITOR
	PrimaryActorTick.bStartWithTickEnabled = true;
#endif

	useTool = true;

	cameraComponent = CreateDefaultSubobject<UCustomCameraComponent>("CameraComponent");
	cameraComponent->SetupAttachment(RootComponent);
	AddOwnedComponent(cameraComponent);

	interactionComponent = CreateDefaultSubobject<UInteractionComponent>("InteractionComponent");
	interactionComponent->SetupAttachment(RootComponent);
	AddOwnedComponent(interactionComponent);

	lifeComponent = CreateDefaultSubobject<ULifeComponent>("LifeComponent");
	AddOwnedComponent(lifeComponent);

	statusComponent = CreateDefaultSubobject<UStatusComponent>("Status");
	AddOwnedComponent(statusComponent);

	bonusComponent = CreateDefaultSubobject<UBonusComponent>("BonusComponent");
	AddOwnedComponent(bonusComponent);

	fsmComponent = CreateDefaultSubobject<UPlayerFSMComponent>("FSM");
	AddOwnedComponent(fsmComponent);

	treeSourceComponent = CreateDefaultSubobject<UTreeSourceComponent>("StreaminSource");
	AddOwnedComponent(treeSourceComponent);

	movableComponent = CreateDefaultSubobject<UMovableComponent>("Movable");
	AddOwnedComponent(movableComponent);

	inventoryComponent = CreateDefaultSubobject<UInventoryComponent>("InventoryComponent");
	AddOwnedComponent(inventoryComponent);


	map3DManagerComponent = CreateDefaultSubobject<UMap3D_Manager>("Map3DManagerComponent");
	AddOwnedComponent(map3DManagerComponent);

	isAiming = false;

	isUsingNitroHandcar = false;
}

bool AMainCharacter::ShouldTickIfViewportsOnly() const
{
	return useTool;
}

void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();
	if (inEditor) return;

	//Turn the mesh in the chara direction and set the chara rotation to 0
	const float _charaYaw = GetActorRotation().Yaw;
	GetCameraComponent()->GetSpringArm()->SetRelativeRotation(FRotator(0.0f, GetActorRotation().Yaw, 0.0f));
	SetActorRotation(FRotator(0.0f));
	SetMeshRotation(FRotator(0.0f, _charaYaw, 0.0f), true);

	//get the custom Movement component + Setup
	customMovementComponent = Cast<UCustomCharacterMovementComponent>(GetMovementComponent());
	customMovementComponent->InitMovement();
	
	//get speed modifier from GameState
	ACustomGameState* _gameState = Cast<ACustomGameState>(GetWorld()->GetGameState());
	if (_gameState)
	{
		if (_gameState->GetCurrentWeather())
		{
			_gameState->GetCurrentWeather()->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, false));
		}
		speedModifiers = _gameState->GetSpeedModifiers();
	}

	//Disable Chara for FadeOut
	if (gameInstance)
	{
		DisableInput(Cast<APlayerController>(Controller));
		gameInstance->OnFinishFadeOut().AddUniqueDynamic(this, &AMainCharacter::StartPlay);
		if (_gameState)
			gameInstance->SetGameStatus(_gameState->GetStartStatus());
	}

	//Bind event OnUpdaterCurrentArena
	onUpdateCurrentArena.AddDynamic(this, &AMainCharacter::InitArenaCamera);

	//Set isOwner ==> Usefull for Network (so not anymore)
	if (APlayerController* _mainController = GetWorld()->GetFirstPlayerController())
		isOwner = _mainController == Controller;

	//Init Day and Meteo Manager
	InitDayManager();
	if (gameInstance->GetGameStatus() == EGameStatus::HUB)
		InitWeatherManager();

	//Setup FSM
	if (isOwner)
	{
		EnablePlayerContext(inputs->GetGameContextInput());
		fsmComponent->SetPlayer(this);
		fsmComponent->Start();
	}



	//Setup TreeSource Component
	if (!isOwner)
		treeSourceComponent->ChangeSourceSettings(treeSourceComponent->GetRadius() / 3);

	//Setup Bonus Component
	if (bonusComponent)
		bonusComponent->SetPlayer(this);

	//Spawn Meteo + Load Save for Bonus
	if (gameInstance->GetGameStatus() != EGameStatus::HUB)
	{
		//MOVE TO PRINCIPALE GAMEMODE

		//if (isOwner)
		//{
		//	currentWeather = GetWorld()->SpawnActor<AWeather>(gameInstance->GetCurrentWeather(), GetActorLocation(),
		//	                                                  GetActorRotation());
		//	if (currentWeather)
		//		currentWeather->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
		//}


		LoadSaves();
	}

	//Setup LifeComponent + Bind OnDeath
	if (lifeComponent)
	{
		lifeComponent->InitLife();
		if (isOwner)
			lifeComponent->OnDie().AddDynamic(this, &AMainCharacter::SetDeath);
	}
	else
		LOG_ERROR(MAIN_CHARA_USE_LOG, "lifeComponent is null!!");

	//Setup interaction
	interactionComponent->InitInteraction(this);


	//Set currentRotationSpeed
	currentRotationSpeed = rotationSpeed;

	//Get UI Accessibility save and set property
	if (const TObjectPtr<USettingsSave>& _settings = gameInstance->GetSubsystem<USaveManagerSubsystem>()->
	                                                               GetSaveSettings())
	{
		sensibilityX = _settings->GetSensibilityX();
		sensibilityY = _settings->GetSensibilityY();
		invertX = _settings->GetInvertX();
		invertY = _settings->GetInvertY();

		gamepadSensibilityX = _settings->GetControllerSensibilityX();
		gamepadSensibilityY = _settings->GetControllerSensibilityY();
		gamepadInvertX = _settings->GetControllerInvertX();
		gamepadInvertY = _settings->GetControllerInvertY();
	}

	//Set player material ==> Network
	if ((!HasAuthority() && isOwner) || (HasAuthority() && !isOwner))
		GetMesh()->SetMaterialByName("Body1", j2Material);

	//set Gravity for Jump
	defaultGravityScale = GetCharacterMovement()->GravityScale;

	//Call an event
	BeginPlayer();
}

void AMainCharacter::StartPlay()
{
	EnableInput(Cast<APlayerController>(Controller));
	gameInstance->OnFinishFadeOut().RemoveDynamic(this, &AMainCharacter::StartPlay);
}

void AMainCharacter::LoadSaves()
{
	USaveManagerSubsystem* _saveManager = GetWorld()->GetGameInstance()->GetSubsystem<USaveManagerSubsystem>();
	if (UGameplaySave* _save = _saveManager->GetCurrentSave())
	{
		FBonusSave _bonus = _save->GetBonusSave();
		GetBonusComponent()->SetBaseMaxLife(_bonus.baseMaxLife);
		GetBonusComponent()->LoadBonus(_bonus.bonusStructs);

		LOG(MAIN_CHARA_USE_LOG, "SAVE LOAD");
	}
}

void AMainCharacter::InitAnimInstance(UCharaAnim* _anim)
{
	//charaAnim->UnBind();
	charaAnim = _anim;
	charaAnim->Bind();
}

void AMainCharacter::InitWeatherManager()
{
	if (HasAuthority() && !GetWorld()->GetGameState<ACustomGameState>()->GetWeatherManager())
	{
		AWeatherManager* _weather = GetWorld()->SpawnActor<AWeatherManager>(weatherManagerRef);
		weatherManager = _weather;
		GetWorld()->GetGameState<ACustomGameState>()->SetWeatherManager(weatherManager);
		weatherManager->SetDayManager(dayManager);
		weatherManager->SelectWeather();
	}
}

void AMainCharacter::InitDayManager()
{
	ADayManager* _dayManager = GetWorld()->SpawnActor<ADayManager>(dayManagerRef);
	dayManager = _dayManager;

	USaveManagerSubsystem* _saveManager = GetWorld()->GetGameInstance()->GetSubsystem<USaveManagerSubsystem>();
	if (UGameplaySave* _save = _saveManager->GetCurrentSave())
	{
		FGameSave _game = _save->GetGameSave();
		dayManager->LoadActualDay(_game.day);
		LOG(MAIN_CHARA_USE_LOG, "DAY LOAD => " + FString::FromInt(dayManager->GetCurrentDay()));
	}
}

void AMainCharacter::BeginPlayer()
{
	onBeginPlayer.Broadcast();
}

void AMainCharacter::SetupMembers(ACustomPlayerController* _playerController)
{
	gameInstance = GetCustomGameInstance();
	if (!HasAuthority())return;
	//LOG(true, "SETUP MEMBER");

	USaveManagerSubsystem* _saveManager = GetWorld()->GetGameInstance()->GetSubsystem<USaveManagerSubsystem>();
	if (_saveManager)
	{
		UGameplaySave* _save = _saveManager->GetCurrentSave();
		if (_save)
		{
			for (size_t i = 0; i < _playerController->GetAllMembers().Num(); i++)
			{
				LoadRightArm(_save, _playerController, i);
				LoadLeftArm(_save, _playerController, i);
				LoadThigh(_save, _playerController, i);
				LoadCalf(_save, _playerController, i);
			}
		}
	}

	unlockedMembers.Add(_playerController->GetRightArm());
	unlockedMembers.Add(_playerController->GetLeftArm());
	unlockedMembers.Add(_playerController->GetCalf());
	unlockedMembers.Add(_playerController->GetThigh());
	SetRightArm(_playerController->GetRightArm());
	SetLeftArm(_playerController->GetLeftArm());
	SetCalf(_playerController->GetCalf());
	SetThigh(_playerController->GetThigh());


	//_gi->OnRightArmChanged().AddDynamic(this, &AMainCharacter::SetRightArm);
	//_gi->OnLeftArmChanged().AddDynamic(this, &AMainCharacter::SetLeftArm);
	//_gi->OnCalfChanged().AddDynamic(this, &AMainCharacter::SetCalf);
	//_gi->OnThighChanged().AddDynamic(this, &AMainCharacter::SetThigh);
}

void AMainCharacter::LoadCalf(UGameplaySave* _save, ACustomPlayerController* _playerController, size_t _i)
{
	if (_save->GetMembersSave().calfType == EMemberType::NONE)return;
	if (_save->GetMembersSave().calfType == _playerController->GetAllMembers()[_i]->GetDefaultObject<AMembersActor>()->
		GetMemberType())
		_playerController->SetCalf(_playerController->GetAllMembers()[_i]);
}

void AMainCharacter::LoadThigh(UGameplaySave* _save, ACustomPlayerController* _playerController, size_t _i)
{
	if (_save->GetMembersSave().thighType == EMemberType::NONE)return;
	if (_save->GetMembersSave().thighType == _playerController->GetAllMembers()[_i]->GetDefaultObject<AMembersActor>()->
		GetMemberType())
		_playerController->SetThigh(_playerController->GetAllMembers()[_i]);
}

void AMainCharacter::LoadLeftArm(UGameplaySave* _save, ACustomPlayerController* _playerController, size_t _i)
{
	if (_save->GetMembersSave().leftArmType == EMemberType::NONE)return;
	if (_save->GetMembersSave().leftArmType == _playerController->GetAllMembers()[_i]->GetDefaultObject<AMembersActor>()
		->GetMemberType())
		_playerController->SetLeftArm(_playerController->GetAllMembers()[_i]);
}

void AMainCharacter::LoadRightArm(UGameplaySave* _save, ACustomPlayerController* _playerController, size_t _i)
{
	if (_save->GetMembersSave().rightArmType == EMemberType::NONE)return;
	if (_save->GetMembersSave().rightArmType == _playerController->GetAllMembers()[_i]->GetDefaultObject<
		AMembersActor>()->GetMemberType())
		_playerController->SetRightArm(_playerController->GetAllMembers()[_i]);
}

void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!gameInstance || inEditor)
		return;

	//Mesh Rotation
	if (isOwner && canTurn)
		Turn(targetYaw);
	if (canTurn)
		GetMesh()->SetRelativeRotation(meshRotation);

	//Speed
	UpdateSpeed();

	//Log
	LOG_COLOR_TICK(true, "State => " + fsmComponent->GetRunningFSM()->GetCurrentState()->GetName(), PINK);
}

void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (!inputs)
		return;
	UEnhancedInputComponent* _input = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	_input->BindAction(inputs->GetMoveInput(), ETriggerEvent::Started, this, &AMainCharacter::StartMove);
	_input->BindAction(inputs->GetMoveInput(), ETriggerEvent::Triggered, this, &AMainCharacter::Move);
	_input->BindAction(inputs->GetMoveInput(), ETriggerEvent::Completed, this, &AMainCharacter::StopMove);
	_input->BindAction(inputs->GetMoveInput(), ETriggerEvent::Canceled, this, &AMainCharacter::CancelMove);

	_input->BindAction(inputs->GetRotateInput(), ETriggerEvent::Triggered, this, &AMainCharacter::Rotate);
	_input->BindAction(inputs->GetRotatePitchInput(), ETriggerEvent::Triggered, this,
	                   &AMainCharacter::RotatePitchCamera);

	_input->BindAction(inputs->GetJumpInput(), ETriggerEvent::Started, this, &AMainCharacter::CustomJump);
	_input->BindAction(inputs->GetRunInput(), ETriggerEvent::Triggered, this, &AMainCharacter::Run);
	_input->BindAction(inputs->GetEscapeInput(), ETriggerEvent::Started, this, &AMainCharacter::EscapeMenu);
	_input->BindAction(inputs->GetPauseInput(), ETriggerEvent::Started, this, &AMainCharacter::Pause);

	_input->BindAction(inputs->GetRightArmInput(), ETriggerEvent::Started, this, &AMainCharacter::RightArm);
	_input->BindAction(inputs->GetRightArmInput(), ETriggerEvent::Completed, this, &AMainCharacter::RightArmRelease);

	_input->BindAction(inputs->GetLeftArmInput(), ETriggerEvent::Started, this, &AMainCharacter::LeftArm);
	_input->BindAction(inputs->GetLeftArmInput(), ETriggerEvent::Completed, this, &AMainCharacter::LeftArmRelease);

	_input->BindAction(inputs->GetCalfsInput(), ETriggerEvent::Started, this, &AMainCharacter::Calf);
	_input->BindAction(inputs->GetCalfsInput(), ETriggerEvent::Completed, this, &AMainCharacter::CalfRelease);

	_input->BindAction(inputs->GetThighsInput(), ETriggerEvent::Started, this, &AMainCharacter::Thigh);
	_input->BindAction(inputs->GetThighsInput(), ETriggerEvent::Completed, this, &AMainCharacter::ThighRelease);

	_input->BindAction(inputs->GetScopeInput(), ETriggerEvent::Started, this, &AMainCharacter::Scope);
	_input->BindAction(inputs->GetScopeInput(), ETriggerEvent::Completed, this, &AMainCharacter::ScopeRelease);

	if (interactionComponent)
		_input->BindAction(inputs->GetInteractionInput(), ETriggerEvent::Started, this, &AMainCharacter::Interact);

	_input->BindAction(inputs->GetGiveUpInput(), ETriggerEvent::Triggered, this, &AMainCharacter::GiveUp);
	_input->BindAction(inputs->GetDeadInput(), ETriggerEvent::Triggered, this, &AMainCharacter::SetDeadMode);
	_input->BindAction(inputs->GetExploInput(), ETriggerEvent::Triggered, this, &AMainCharacter::SetExploMode);
	_input->BindAction(inputs->GetStealthInput(), ETriggerEvent::Triggered, this, &AMainCharacter::SetStealthMode);
	_input->BindAction(inputs->GetFightInput(), ETriggerEvent::Triggered, this, &AMainCharacter::SetFightMode);
	_input->BindAction(inputs->GetShootInput(), ETriggerEvent::Triggered, this, &AMainCharacter::SetShootMode);
	_input->BindAction(inputs->GetDropInput(), ETriggerEvent::Started, this, &AMainCharacter::Drop);


	_input->BindAction(inputs->GetFATInput(), ETriggerEvent::Triggered, this, &AMainCharacter::SetFAT);
	_input->BindAction(inputs->GetBIGInput(), ETriggerEvent::Triggered, this, &AMainCharacter::SetBIG);
	_input->BindAction(inputs->GetHEAVYInput(), ETriggerEvent::Triggered, this, &AMainCharacter::SetHEAVY);
	_input->BindAction(inputs->GetMASSIVEInput(), ETriggerEvent::Triggered, this, &AMainCharacter::SetMASSIVE);
	_input->BindAction(inputs->GetNOWEIGHTInput(), ETriggerEvent::Triggered, this, &AMainCharacter::SetNOWEIGHT);

	_input->BindAction(inputs->GetPushToTalkInput(), ETriggerEvent::Triggered, this, &AMainCharacter::PushToTalk);
	_input->BindAction(inputs->GetEnableGodModeInput(), ETriggerEvent::Started, this, &AMainCharacter::EnableGodMode);
	_input->BindAction(inputs->GetDisableGodModeInput(), ETriggerEvent::Started, this, &AMainCharacter::DisableGodMode);

	//TODO CCC-MainCharacter : A FAIRE
	//_input->BindAction(inputs->GetRunInput(), ETriggerEvent::Triggered, this, &AMainCharacter::Run_Hold);
}


void AMainCharacter::EnablePlayerContext(TSoftObjectPtr<UInputMappingContext> _context)
{
	if (!inputs)
		return;
	APlayerController* _player = GetWorld()->GetFirstPlayerController();
	UEnhancedInputLocalPlayerSubsystem* _inputSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(
		_player->GetLocalPlayer());
	_inputSystem->ClearAllMappings();
	_inputSystem->AddMappingContext(_context.LoadSynchronous(), 0);
	currentPlayerContext = _context;
	onChangeMappingContext.Broadcast(currentPlayerContext.Get());
}

void AMainCharacter::DisablePlayerContext(TSoftObjectPtr<UInputMappingContext> _context)
{
	if (!inputs)
		return;
	APlayerController* _player = GetWorld()->GetFirstPlayerController();
	UEnhancedInputLocalPlayerSubsystem* _inputSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(
		_player->GetLocalPlayer());
	_inputSystem->ClearAllMappings();
	_inputSystem->RemoveMappingContext(_context.LoadSynchronous());
	currentPlayerContext = nullptr;
	onChangeMappingContext.Broadcast(nullptr);
}

void AMainCharacter::PlayerIsAttacked()
{
	inCombat = true;
	onEnterInCombat.Broadcast();
}

void AMainCharacter::SetMeshRotation(FRotator _rotation, const bool _canTurn, const bool _world)
{
	canTurn = _canTurn;
	LOG_COLOR(MAIN_CHARA_USE_LOG, "Mesh rotation => " + _rotation.ToString(), BLUE);

	_rotation -= _world ? GetActorRotation() : FRotator(0.0f);

	if (!canTurn)
	{
		GetMesh()->SetRelativeRotation(_rotation);
		meshRotation = _rotation;
	}
	targetYaw = _rotation.Yaw;
}

void AMainCharacter::StartMove(const FInputActionValue& _value)
{
	if (isStun || !canMove)
		return;

	FVector2D _newValue = _value.Get<FVector2D>();
	if (_newValue == FVector2D::ZeroVector)
		return;

	USpringArmComponent* _arm = cameraComponent->GetSpringArm();
	if (!GetIsAiming() && !GetIsShooting())
		targetYaw = FindAngle(_newValue);
	else
		finalRotation = FRotator(0.0f, targetYaw = _arm->GetRelativeRotation().Yaw, 0.0f);
}

void AMainCharacter::Move(const FInputActionValue& _value)
{
	if (isStun || !canMove)
		return;
	FVector2D _newValue = _value.Get<FVector2D>();
	if (_newValue == FVector2D::ZeroVector)
		return;

	USpringArmComponent* _arm = cameraComponent->GetSpringArm();

	if (!GetIsAiming() && !GetIsShooting())
		targetYaw = FindAngle(_newValue);
	else
		finalRotation = FRotator(0.0f, targetYaw = _arm->GetRelativeRotation().Yaw, 0.0f);

	const FRotator& _camRotation = cameraComponent->GetCamera()->GetComponentRotation();
	const FVector& _forward = UCustomMathBlueprintLibrary::GetForwardFromRotation(
		FRotator(0.0f, _camRotation.Yaw, 0.0f));
	const FVector& _right = UCustomMathBlueprintLibrary::GetRightFromRotation(FRotator(0.0f, _camRotation.Yaw, 0.0f));
	const FVector& _dir = _forward * _newValue.X + _right * _newValue.Y;

	UpdateMovement(_newValue, _dir);
	onMove.Broadcast(_newValue);
	inputDirection = _newValue;
	AddMovementInput(_dir);
}

void AMainCharacter::CancelMove(const FInputActionValue& _value)
{
	if (isStun)
		return;

	FVector2D _newValue = _value.Get<FVector2D>();

	inputDirection = _newValue;
	UpdateMovement(_newValue, FVector(0));
}

void AMainCharacter::UpdateMovement(const FVector2D& _newValue, const FVector& _moveDir)
{
	if (HasAuthority())
		MultiRpc_UpdateMovement(_newValue, _moveDir);
	else
		ServerRpc_UpdateMovement(_newValue, _moveDir);
}

void AMainCharacter::ServerRpc_UpdateMovement_Implementation(const FVector2D& _newValue, const FVector& _moveDir)
{
	MultiRpc_UpdateMovement(_newValue, _moveDir);
}

void AMainCharacter::MultiRpc_UpdateMovement_Implementation(const FVector2D& _newValue, const FVector& _moveDir)
{
	if (charaAnim)
		charaAnim->UpdateMovement(_newValue, _moveDir);
}

void AMainCharacter::StopMove(const FInputActionValue& _value)
{
	if (isStun)
		return;

	FVector2D _newValue = _value.Get<FVector2D>();

	inputDirection = _newValue;
	UpdateMovement(_newValue, FVector(0));
}

float AMainCharacter::FindAngle(const FVector2D& _target)
{
	float _result = FMath::RadiansToDegrees(
		FMath::Acos(FVector2D::DotProduct(FVector2D(1, 0), _target) / (FVector2D(1, 0).Length() * _target.Length())));
	_result = FMath::IsNegativeOrNegativeZero(_target.Y) && _target.Y < 0 ? -_result : _result;

	return _result + cameraComponent->GetSpringArm()->GetRelativeRotation().Yaw;
}

void AMainCharacter::Turn(const float _targetYaw)
{
	if ((GetIsAiming() || GetIsShooting()) &&
		(FMath::Abs(UCustomBlueprintLibrary::GetAimYaw(GetMesh()->GetRelativeRotation(),
		                                               GetCameraComponent()->GetSpringArm()->GetRelativeRotation())) >
			turnClamp))
	{
		finalRotation = FRotator(GetMesh()->GetRelativeRotation().Pitch,
		                         cameraComponent->GetSpringArm()->GetRelativeRotation().Yaw,
		                         GetMesh()->GetRelativeRotation().Roll);

		if (targetYaw != finalRotation.Yaw)
			targetYaw = finalRotation.Yaw;

		if (!charaAnim->GetCanStrafeTurn() && GetCustomMovementComponent()->Velocity.Length() == 0)
			charaAnim->SetCanStrafeTurn(true);
		onStrafeTurn.Broadcast(targetYaw);
	}
	else if (!(GetIsAiming() || GetIsShooting()))
	{
		const FRotator& _meshR = GetMesh()->GetRelativeRotation();
		const float _realYaw = FMath::Abs(_meshR.Yaw) - FMath::Abs(_targetYaw) < 0
			                       ? FMath::IsNegativeOrNegativeZero(_meshR.Yaw)
				                         ? _targetYaw - 360
				                         : _targetYaw + 360
			                       : _targetYaw;
		finalRotation = FRotator(_meshR.Pitch, _realYaw, _meshR.Roll);
	}

	FRotator _newRot = FMath::RInterpConstantTo(meshRotation, finalRotation, GetWorld()->DeltaTimeSeconds,
	                                            currentRotationSpeed);
	if (meshRotation != _newRot)
	{
		meshRotation = _newRot;
		if (!HasAuthority() && isOwner)
			ServerRpc_UpdateRotation(meshRotation);
	}

	if (CheckGoodRotation(_newRot))
		onRotationEnd.Broadcast();
}

bool AMainCharacter::CheckGoodRotation(FRotator& _finalRotation)
{
	return FMath::IsNearlyEqual(GetMesh()->GetRelativeRotation().Yaw, _finalRotation.Yaw, 5.0f);
}

void AMainCharacter::ResetGravityScale(UPrimitiveComponent* HitComponent, AActor* OtherActor,
                                       UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	GetCharacterMovement()->GravityScale = defaultGravityScale;
}

void AMainCharacter::ServerRpc_UpdateRotation_Implementation(const FRotator& _rotation)
{
	meshRotation = _rotation;
}

void AMainCharacter::Interact(const FInputActionValue& _value)
{
	//TODO replace ServerRpc -> MultiRpc quand tu es client par juste un ServerRpc et tu fais les anim de ton coté | si on part du principe qu'on a que 2 joueurs possible
	if (inBlend || isAiming || isShooting || isStun)
		return;
	interactionComponent->Interact();
}

void AMainCharacter::ServerRpc_Interact_Implementation(const FInputActionValue& _value)
{
	MultiRpc_Interact(_value);
}

void AMainCharacter::MultiRpc_Interact_Implementation(const FInputActionValue& _value)
{
	//if (charaAnim && interactionComponent->GetClosestActor())
	//{
	//	if (isUsingNitroHandcar || isUsingHandcar ) return;
	//	if (Cast<ABrakeHandcarActor>(interactionComponent->GetClosestActor()))
	//		charaAnim->SetIsUsingBrake(!charaAnim->GetIsUsingBrake());
	//}
}

void AMainCharacter::QuitMenu()
{
	if (!inMenu && !inPause) return;

	if (inPause)
	{
		//TODO trouver une solution pour mettre en pause sans freeze l'ui
		//GetWorld()->GetFirstPlayerController()->SetPause(false);
		ReactivateMembers({});
		OnSwitchPauseState(false);
	}
	inMenu = false;
	inInventory = false;
	inPause = false;
	GetGameInstance<UCustomGameInstance>()->SetInPause(false);

	AMainHUD* _hud = GETFPC->GetHUD<AMainHUD>();
	CHECK_NULL(_hud, "hud is null");
	_hud->SetState(GetGameInstance<UCustomGameInstance>()->GetGameStatus() == HUB
		               ? EStateUI::PlayerLobbyUI
		               : EStateUI::PlayerGameUI);

	onQuitMenu.Broadcast();

	//GetWorld()->GetFirstPlayerController()->SetViewTargetWithBlend(this, 1);
}

void AMainCharacter::EscapeMenu(const FInputActionValue& _value)
{
	if (inSubMenu)
	{
		AMainHUD* _hud = GETFPC->GetHUD<AMainHUD>();
		CHECK_NULL(_hud, "hud is null");
		UNewStock* _stock = _hud->GetCurrentUI<UNewStock>();

		_stock->CloseOverlay();
		inSubMenu = false;
	}
	else
	{
		QuitMenu();
	}
}

void AMainCharacter::EnableGodMode()
{
	LOG_COLOR_TIME(true, "GOD MODE ENABLE", CYAN, 3);
	lifeComponent->EnableGodMode();
}

void AMainCharacter::DisableGodMode()
{
	//statusComponent->RemoveStatus(this, EStatusType::PROTECT);
	lifeComponent->DisableGodMode();
}

void AMainCharacter::UpdateSpeed()
{
	const bool _inHub = GetCustomGameInstance()->GetGameStatus() == EGameStatus::HUB;

	const float _speedModifiers = _inHub
		                              ? 1.0f
		                              : (1.0f + speedBonus) - (1.0f - (currentSpeedModifier / 100.0f));

	float _currentSpeed = GetCharacterMovement()->MaxWalkSpeed;

	float _alphaResult = GetAlphaSpeed();
	float _result = 0;
	_result = FMath::Lerp(speedModes[ESpeedMode::RUN], speedModes[ESpeedMode::MINIMUM], _alphaResult);

	if (currentSpeedMode == ESpeedMode::SHOOT || currentSpeedMode == ESpeedMode::WALK)
	{
		if (_result > speedModes[currentSpeedMode])
			GetCharacterMovement()->MaxWalkSpeed = useLerpSpeed
				                                       ? LerpSpeed(_currentSpeed,
				                                                   speedModes[currentSpeedMode] * (1.0f +
					                                                   speedAimBonus))
				                                       : speedModes[currentSpeedMode] * (1.0f + speedAimBonus);
		else
			GetCharacterMovement()->MaxWalkSpeed = useLerpSpeed
				                                       ? LerpSpeed(_currentSpeed, _result * (1.0f + speedAimBonus))
				                                       : _result * (1.0f + speedAimBonus);
	}
	else if (currentSpeedMode == ESpeedMode::SPRINT || currentSpeedMode == ESpeedMode::NOSPEED)
	{
		GetCharacterMovement()->MaxWalkSpeed = useLerpSpeed
			                                       ? LerpSpeed(_currentSpeed,
			                                                   speedModes[currentSpeedMode] * (1.0f + (speedBonus *
				                                                   runSpeedBonusModifier)))
			                                       : speedModes[currentSpeedMode] * (1.0f + (speedBonus *
				                                       runSpeedBonusModifier));
	}
	else
		GetCharacterMovement()->MaxWalkSpeed = useLerpSpeed
			                                       ? LerpSpeed(_currentSpeed, _result * _speedModifiers)
			                                       : _result * _speedModifiers;

	//LOG_COLOR(MAIN_CHARA_USE_LOG, FString::SanitizeFloat(GetCharacterMovement()->MaxWalkSpeed), RED);
}

float AMainCharacter::LerpSpeed(float _currentSpeed, float _result)
{
	return FMath::FInterpConstantTo(_currentSpeed, _result, GetWorld()->GetDeltaSeconds(), lerpSpeed);
}

float AMainCharacter::GetAlphaSpeed()
{
	alphaSpeed = 0;
	if (inventoryComponent->GetCurrentEgg())
		alphaSpeed = inventoryComponent->GetTotalInventoryWeight() / (inventoryComponent->GetMaxWeightBiomass() +
			inventoryComponent->GetCurrentEgg()->GetWeight());
	else
		alphaSpeed = inventoryComponent->GetTotalInventoryWeight() / inventoryComponent->GetMaxWeightBiomass();
	return alphaSpeed;
}

void AMainCharacter::BumpPlayer(const FVector& _bumpSource, const bool _isBumpByIA)
{
	if (charaAnim->GetIsBumped()) return;
	const FRotator& _newRot = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(),
	                                                                 FVector(_bumpSource.X, _bumpSource.Y,
	                                                                         GetActorLocation().Z));
	SetMeshRotation(_newRot, false);
	DeactivateMembers({});
	canMove = false;
	canTurn = false;
	onBump.Broadcast(true, _isBumpByIA);
}

void AMainCharacter::EndBump()
{
	ReactivateMembers({});
	canMove = true;
	canTurn = true;
}


void AMainCharacter::Rotate(const FInputActionValue& _value)
{
	bool isGamepad = false;
	if (UCommonInputSubsystem* CommonInputSystem = UCommonInputSubsystem::Get(GETFPC->GetLocalPlayer()))
	{
		isGamepad = CommonInputSystem->GetCurrentInputType() == ECommonInputType::Gamepad;
	}
	const float _axis = _value.Get<float>() * (!isGamepad
		                                           ? (invertX ? -sensibilityX : sensibilityX)
		                                           : gamepadInvertX
		                                           ? -gamepadSensibilityX
		                                           : gamepadSensibilityX);
	USpringArmComponent* _arm = cameraComponent->GetSpringArm();
	_arm->AddRelativeRotation(FRotator(0, _axis, 0));
	onYawUpdate.Broadcast(_axis);
}

void AMainCharacter::RotatePitchCamera(const FInputActionValue& _value)
{
	bool isGamepad = false;
	if (UCommonInputSubsystem* CommonInputSystem = UCommonInputSubsystem::Get(GETFPC->GetLocalPlayer()))
	{
		isGamepad = CommonInputSystem->GetCurrentInputType() == ECommonInputType::Gamepad;
	}
	const float _axis = _value.Get<float>() * (!isGamepad
		                                           ? (invertY ? -sensibilityY : sensibilityY)
		                                           : gamepadInvertY
		                                           ? -gamepadSensibilityY
		                                           : gamepadSensibilityY);

	USpringArmComponent* _arm = cameraComponent->GetSpringArm();
	int _pitch = cameraComponent->GetSpringArm()->GetRelativeRotation().Pitch;
	int _newPitchX = _pitch + _axis;

	if (_newPitchX <= -cameraClampMax)
	{
		_arm->SetRelativeRotation(FRotator(-cameraClampMax, _arm->GetRelativeRotation().Yaw,
		                                   _arm->GetRelativeRotation().Roll));
		return;
	}
	if (_newPitchX >= cameraClampMin)
	{
		_arm->SetRelativeRotation(FRotator(cameraClampMin, _arm->GetRelativeRotation().Yaw,
		                                   _arm->GetRelativeRotation().Roll));
		return;
	}
	_arm->AddLocalRotation(FRotator(1, 0, 0) * _axis);
}

void AMainCharacter::SetupAimStarted()
{
	currentRotationSpeed = setupAimRotationSpeed;
	finalRotation = FRotator(0.0f, targetYaw = GetCameraComponent()->GetSpringArm()->GetRelativeRotation().Yaw, 0.0f);
	onRotationEnd.AddUniqueDynamic(this, &AMainCharacter::SetupAimFinished);
}

void AMainCharacter::SetupAimFinished()
{
	currentRotationSpeed = aimRotationSpeed;
	onRotationEnd.RemoveDynamic(this, &AMainCharacter::SetupAimFinished);
}

void AMainCharacter::ServerRpc_SetWeightState_Implementation(const EWeightState& _state)
{
	currentWeightState = weightModifiers[_state];
}

void AMainCharacter::ServerRpc_SetClientSpeedMode_Implementation(const ESpeedMode& _mode)
{
	currentSpeedMode = _mode;
}

void AMainCharacter::Run(const FInputActionValue& _value)
{
	if (isStun)
		return;

	//LOG_COLOR(MAIN_CHARA_USE_LOG, "RUN", MAGENTA);
	if (HasAuthority())
		Run_Internal();
	else
		ServerRpc_Run();
}

void AMainCharacter::Run_Internal()
{
	if (!isRunning)
	{
		LOG_COLOR(MAIN_CHARA_USE_LOG, "RUN", RED);
		isRunning = true;
		if (HasAuthority())
			currentSpeedMode = ESpeedMode::RUN;
		else
			ServerRpc_SetClientSpeedMode(ESpeedMode::RUN);
		return;
	}
	else
	{
		LOG_COLOR(MAIN_CHARA_USE_LOG, "WALK", RED);
		isRunning = false;
		if (HasAuthority())
			currentSpeedMode = ESpeedMode::WALK;
		else
			ServerRpc_SetClientSpeedMode(ESpeedMode::WALK);
		return;
	}
}

void AMainCharacter::ServerRpc_Run_Implementation()
{
	Run_Internal();
}

void AMainCharacter::Run_Hold(const FInputActionValue& _value)
{
}

void AMainCharacter::CustomJump(const FInputActionValue& _value)
{
	if (gameInstance->GetGameStatus() == EGameStatus::HUB || isStun || !canJump) return;
	if (GetCharacterMovement()->IsFalling()) return;
	if (charaAnim)
		charaAnim->SetIsJumping(true);
	onJump.Broadcast(true);
	GoodJump(alpha, duration, height);
}

void AMainCharacter::GoodJump(float _alpha, float _duration, float _height)
{
	if (HasAuthority())
		Jump_Internal(_alpha, _duration, _height);
	else
		ServerRpc_CustomJump(_alpha, _duration, _height);
}

void AMainCharacter::Jump_Internal(float _alpha, float _duration, float _height)
{
	if (GetCharacterMovement()->IsFalling()) return;
	Ts = _alpha * _duration;
	float _Vi = (2 * _height) / Ts;
	GetCharacterMovement()->JumpZVelocity = _Vi;
	GetWorld()->GetTimerManager().SetTimer(jumpTimer, this, &AMainCharacter::MultiRpc_EndJump, Ts, false);

	float _Gi = ((-2 * _height) / FMath::Pow(Ts, 2)) / -980;
	GetCharacterMovement()->GravityScale = _Gi;

	Jump();
}

void AMainCharacter::MultiRpc_CustomJump_Implementation(float _alpha, float _duration, float _height)
{
	Jump_Internal(_alpha, _duration, _height);
}

void AMainCharacter::CheckForJumpBuffer()
{
	FHitResult _result;
	UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(), GetActorLocation(),
	                                                GetActorLocation() - GetActorUpVector(), {}, true, {},
	                                                EDrawDebugTrace::ForDuration, _result, true);
}

void AMainCharacter::StopJumping()
{
	Super::StopJumping();
	FTimerManager& _timerManager = GetWorld()->GetTimerManager();
	if (_timerManager.IsTimerActive(jumpTimer))
	{
		_timerManager.ClearTimer(jumpTimer);
		if (charaAnim)
			charaAnim->SetIsJumping(false);
		onJump.Broadcast(false);
	}
	else if (_timerManager.IsTimerActive(resetTimer))
	{
		onJump.Broadcast(false);
		_timerManager.ClearTimer(resetTimer);
	}
}

void AMainCharacter::ServerRpc_CustomJump_Implementation(float _alpha, float _duration, float _height)
{
	MultiRpc_CustomJump(_alpha, _duration, _height);
}

void AMainCharacter::MultiRpc_EndJump_Implementation()
{
	LOG_COLOR(MAIN_CHARA_USE_LOG, "End JUMP", GREEN);
	if (charaAnim)
		charaAnim->SetIsJumping(false);
	float _Gf = ((-2 * height) / FMath::Pow(duration - Ts, 2)) / -980;
	GetCharacterMovement()->GravityScale = _Gf;
	LOG_COLOR(MAIN_CHARA_USE_LOG, FString::SanitizeFloat(_Gf), GREEN);

	GetWorld()->GetTimerManager().SetTimer(resetTimer, this, &AMainCharacter::MultiRpc_ResetGravity, duration - Ts,
	                                       false);
}

void AMainCharacter::MultiRpc_ResetGravity_Implementation()
{
	GetCharacterMovement()->GravityScale = defaultGravityScale;
	LOG_COLOR(MAIN_CHARA_USE_LOG, "Reset Gravity", GREEN);
}

void AMainCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AMainCharacter, meshRotation);
	DOREPLIFETIME(AMainCharacter, alphaSpeed);
	DOREPLIFETIME(AMainCharacter, currentSpeedMode);
	DOREPLIFETIME(AMainCharacter, currentSpeedModifier);
	DOREPLIFETIME(AMainCharacter, currentWeightState);

	DOREPLIFETIME(AMainCharacter, currentRightArm);
	DOREPLIFETIME(AMainCharacter, currentLeftArm);
	DOREPLIFETIME(AMainCharacter, currentCalf);
	DOREPLIFETIME(AMainCharacter, currentThigh);

	DOREPLIFETIME(AMainCharacter, usingRightArm);
	DOREPLIFETIME(AMainCharacter, usingLeftArm);
	DOREPLIFETIME(AMainCharacter, usingCalf);
	DOREPLIFETIME(AMainCharacter, usingThigh);

	DOREPLIFETIME(AMainCharacter, isShooting);

	DOREPLIFETIME(AMainCharacter, weatherManager);
}

void AMainCharacter::GiveUp(const FInputActionValue& _value)
{
	LOG_COLOR(MAIN_CHARA_USE_LOG, "YOU DIED", RED);
	isDown = false;
	lifeComponent->RestoreLife(lifeComponent->GetMaxLife());
	ReactivateMembers({});
}

void AMainCharacter::Pause(const FInputActionValue& _value)
{
	OnSwitchPauseState(true);
	ResetChara();
	Cast<AMainHUD>(GetWorld()->GetFirstPlayerController()->GetHUD())->SetState(EStateUI::MainPause);
	inPause = true;
	GetGameInstance<UCustomGameInstance>()->SetInPause(true);

	//TODO check comment mettre en pause sans que le menu pete un plomb
	//GetWorld()->GetFirstPlayerController()->SetPause(true);

	//UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0.f);

	/*DisablePlayerContext(currentPlayerContext);
	EnablePlayerContext(inputs->GetUIContextInput());*/
}

void AMainCharacter::Drop(const FInputActionValue& _value)
{
	if (isStun || !charaAnim->GetIsHolding())return;
	interactionComponent->SetHasInteracted(true);
	interactionComponent->DropInteractable();
	charaAnim->SetHasCarry(false);
}

void AMainCharacter::SetCurrentArena(AArena* _arena)
{
	currentArena = _arena;
	inCombat = !currentArena ? false : inCombat;
	onUpdateCurrentArena.Broadcast();
}

void AMainCharacter::InitArenaCamera()
{
	if (!currentArena)return;
	currentArena->OnClearArena().AddUniqueDynamic(this, &AMainCharacter::SetupEndCamera);
}

void AMainCharacter::SetupEndCamera(AAlienDoor* _exitGate, const bool _giveBonus)
{
	inCombat = false;
	if (_giveBonus)
	{
		Cast<AGameHUD>(GetWorld()->GetFirstPlayerController()->GetHUD())->SetState(EStateUI::BonusUI);
		onChooseBonus.Broadcast(bonusComponent->GetRandomBonus(3));
	}
	currentArena = nullptr;
	//DisableInput(GetWorld()->GetFirstPlayerController());
	//cameraComponent->GetCamera()->SetWorldRotation(UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), _exitGate->GetActorLocation()));
	//GetWorld()->GetFirstPlayerController()->SetViewTargetWithBlend(_exitGate, 3);
}

FHitResult AMainCharacter::GetGroundInformation()
{
	FHitResult _result;
	FVector _location = GetActorLocation();
	UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(), _location, _location - GetActorUpVector() * 1000,
	                                                groundLayer, false, {}, EDrawDebugTrace::None, _result, true);
	return _result;
}

void AMainCharacter::Destroyed()
{
	if (GetPlayerState())
		onQuit.Broadcast(GetPlayerState()->GetUniqueId());
	Super::Destroyed();

	if (usingRightArm)
		GetWorld()->DestroyActor(usingRightArm);
	if (usingLeftArm)
		GetWorld()->DestroyActor(usingLeftArm);
	if (usingCalf)
		GetWorld()->DestroyActor(usingCalf);
	if (usingThigh)
		GetWorld()->DestroyActor(usingThigh);

	if (!GetWorld()->GetGameState()) return;
	ACustomGameState* gameState = Cast<ACustomGameState>(GetWorld()->GetGameState());
	gameState->Unregister(this);
}

void AMainCharacter::Stun(const float _stunDuration)
{
	isStun = true;
	onStun.Broadcast(isStun);
	FTimerHandle _stunTimer = FTimerHandle();

	FTimerDelegate _stunDelegate = FTimerDelegate::CreateLambda([&]()
	{
		isStun = false;
		onStun.Broadcast(isStun);
	});

	GetWorld()->GetTimerManager().SetTimer(_stunTimer, _stunDelegate, _stunDuration, false);
}

void AMainCharacter::ResetChara()
{
	GetWorld()->GetTimerManager().ClearTimer(shootTimer);
	DeactivateMembers({usingThigh}, false);
	DeactivateMembers({usingLeftArm, usingRightArm, usingCalf});
	isShooting = false;
	currentRotationSpeed = rotationSpeed;
	isAiming = false;
	isAimingLocal = false;
	charaAnim->SetIsScoping(false);
	onScope.Broadcast(false);
	charaAnim->ClearAllAnimWeight();
}

void AMainCharacter::ConstructArmMembers(TSubclassOf<AArmMemberActor> _currentMember)
{
	usingArm = GetWorld()->SpawnActor<AArmMemberActor>(_currentMember, GetActorLocation(),
	                                                   GetMesh()->GetRelativeRotation());
	usingArm->SetMainCharacter(this);
	usingArm->SetupMember();

	usingArm->InitMember();
	usingArm->SetActorLocation(usingArm->GetMemberMesh()->GetRelativeLocation());
	usingArm->AttachToComponent(usingArm->GetMemberMesh(), FAttachmentTransformRules::KeepRelativeTransform);
}

void AMainCharacter::ConstructLegMembers(TSubclassOf<ALegMemberActor> _currentMember)
{
	usingLeg = GetWorld()->SpawnActor<ALegMemberActor>(_currentMember, GetActorLocation(), FRotator(0));
	usingLeg->SetMainCharacter(this);
	usingLeg->SetupMember();

	usingLeg->InitMember();
	usingLeg->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform);
}

//TODO AMainCharacter : Remove ServerRpc
void AMainCharacter::SetRightArm(TSubclassOf<ARightArmActor> _rightArm)
{
	if (!_rightArm)return;
	if (usingRightArm)
	{
		usingRightArm->GetMemberMesh()->DestroyComponent();
		usingMembers.Remove(usingRightArm);
		GetWorld()->DestroyActor(usingRightArm);
	}
	currentRightArm = _rightArm;
	ConstructArmMembers(_rightArm);

	usingRightArm = Cast<ARightArmActor>(usingArm);
	usingMembers.Add(usingRightArm);
	SaveMembers();

	onRightArmInit.Broadcast(usingRightArm);
	onChangeMember.Broadcast();
}

void AMainCharacter::SaveMembers()
{
	//if (!gameInstance)return;
	if (gameInstance->GetGameStatus() == EGameStatus::HUB)
	{
		USaveManagerSubsystem* _saveManager = GetWorld()->GetGameInstance()->GetSubsystem<USaveManagerSubsystem>();
		CHECK_NULL(_saveManager, "Save manager is null");
		UGameplaySave* _save = _saveManager->GetCurrentSave();
		CHECK_NULL(_save, "Pas de save ! Lance depuis le main menu fdp !");
		_save->SaveMembers(this);
	}
}

void AMainCharacter::ServerRpc_SetClientRightArm_Implementation(TSubclassOf<ARightArmActor> _rightArm)
{
	currentRightArm = _rightArm;
	ConstructArmMembers(_rightArm);
}

void AMainCharacter::SetLeftArm(TSubclassOf<ALeftArmActor> _leftArm)
{
	if (!_leftArm)return;
	if (usingLeftArm)
	{
		usingLeftArm->GetMemberMesh()->DestroyComponent();
		usingMembers.Remove(usingLeftArm);
		GetWorld()->DestroyActor(usingLeftArm);
	}
	currentLeftArm = _leftArm;
	ConstructArmMembers(_leftArm);

	usingLeftArm = Cast<ALeftArmActor>(usingArm);
	usingMembers.Add(usingLeftArm);

	SaveMembers();
	onLeftArmInit.Broadcast(usingLeftArm);
	onChangeMember.Broadcast();
}

void AMainCharacter::ServerRpc_SetClientLeftArm_Implementation(TSubclassOf<ALeftArmActor> _leftArm)
{
	currentLeftArm = _leftArm;
	ConstructArmMembers(_leftArm);
}

void AMainCharacter::SetCalf(TSubclassOf<ACalfActor> _calf)
{
	if (!_calf)return;
	if (usingCalf)
	{
		usingMembers.Remove(usingCalf);
		GetWorld()->DestroyActor(usingCalf);
	}
	currentCalf = _calf;
	ConstructLegMembers(_calf);

	usingCalf = Cast<ACalfActor>(usingLeg);
	usingMembers.Add(usingCalf);
	SaveMembers();
	onCalfInit.Broadcast(usingCalf);
	onChangeMember.Broadcast();
	if (inMenu)
		charaAnim->SetIsOpenCalf(true);
}

void AMainCharacter::ServerRpc_SetClientCalf_Implementation(TSubclassOf<ACalfActor> _calf)
{
	currentCalf = _calf;
	ConstructLegMembers(_calf);
}

void AMainCharacter::SetThigh(TSubclassOf<AThighActor> _thigh)
{
	if (!_thigh)return;
	if (usingThigh)
	{
		usingThigh->GetLeftMemberMesh()->DestroyComponent();
		usingThigh->GetRightMemberMesh()->DestroyComponent();
		usingMembers.Remove(usingThigh);
		GetWorld()->DestroyActor(usingThigh);
	}
	currentThigh = _thigh;
	ConstructLegMembers(_thigh);

	usingThigh = Cast<AThighActor>(usingLeg);
	SaveMembers();
	usingMembers.Add(usingThigh);
	onChangeMember.Broadcast();
}

void AMainCharacter::ServerRpc_SetClientThigh_Implementation(TSubclassOf<AThighActor> _thigh)
{
	currentThigh = _thigh;
	ConstructLegMembers(_thigh);
}

void AMainCharacter::OnRep_UsingRightArm(ARightArmActor* _oldArm)
{
	if (_oldArm)
	{
		_oldArm->GetMemberMesh()->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
		_oldArm->GetMemberMesh()->UnregisterComponent();
		_oldArm->GetMemberMesh()->DestroyComponent();
	}

	if (!usingRightArm)
	{
		LOG_ERROR(true, "RightArm is null!");
		return;
	}

	usingRightArm->SetMainCharacter(this);
	usingRightArm->SetupMember();
	if (isOwner)
		usingRightArm->InitMember();
	usingMembers.Add(usingRightArm);
	onRightArmInit.Broadcast(usingRightArm);
	if (charaAnim)
		charaAnim->BindRightArms();
}

void AMainCharacter::OnRep_UsingLeftArm(ALeftArmActor* _oldArm)
{
	if (_oldArm)
	{
		_oldArm->GetMemberMesh()->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
		_oldArm->GetMemberMesh()->UnregisterComponent();
		_oldArm->GetMemberMesh()->DestroyComponent();
	}
	if (!usingLeftArm)
	{
		LOG_ERROR(true, "LeftArm is null!");
		return;
	}

	usingLeftArm->SetMainCharacter(this);
	usingLeftArm->SetupMember();
	if (isOwner)
		usingLeftArm->InitMember();
	usingMembers.Add(usingLeftArm);
	onLeftArmInit.Broadcast(usingLeftArm);
	if (charaAnim)
		charaAnim->BindLeftArms();
}

void AMainCharacter::OnRep_UsingCalf(ACalfActor* _oldCalf)
{
	if (!usingCalf)
	{
		LOG_ERROR(true, "Calf is null!");
		return;
	}

	usingCalf->SetMainCharacter(this);
	usingCalf->SetupMember();
	if (isOwner)
		usingCalf->InitMember();
	usingMembers.Add(usingCalf);
	if (charaAnim)
		charaAnim->BindCalfs();
	onCalfInit.Broadcast(usingCalf);
}

void AMainCharacter::OnRep_UsingThigh(AThighActor* _oldThigh)
{
	if (_oldThigh)
	{
		_oldThigh->GetLeftMemberMesh()->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
		_oldThigh->GetLeftMemberMesh()->UnregisterComponent();
		_oldThigh->GetLeftMemberMesh()->DestroyComponent();

		_oldThigh->GetRightMemberMesh()->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
		_oldThigh->GetRightMemberMesh()->UnregisterComponent();
		_oldThigh->GetRightMemberMesh()->DestroyComponent();
	}
	if (!usingThigh)
	{
		LOG_ERROR(true, "Thight is null!");
		return;
	}

	usingThigh->SetMainCharacter(this);
	usingThigh->SetupMember();
	if (isOwner)
		usingThigh->InitMember();
	usingMembers.Add(usingThigh);
}

void AMainCharacter::DeactivateMembers(const TArray<AMembersActor*>& _exceptionMembers, const bool _resetMembers)
{
	const int _memberCounts = usingMembers.Num();
	for (int i = 0; i < _memberCounts; i++)
	{
		if (_exceptionMembers.Contains(usingMembers[i]))
			continue;
		usingMembers[i]->SetIsActivated(false);
		_resetMembers ? usingMembers[i]->ResetMember() : usingMembers[i]->ReleaseBehavior();
	}
}

void AMainCharacter::ReactivateMembers(const TArray<AMembersActor*>& _exceptionMembers)
{
	const int _memberCounts = usingMembers.Num();
	for (int i = 0; i < _memberCounts; i++)
	{
		if (_exceptionMembers.Contains(usingMembers[i]))
			continue;
		usingMembers[i]->SetIsActivated(true);
	}
}

void AMainCharacter::Scope(const FInputActionValue& _value)
{
	if (CanScope()) return;

	if (!holdScope)
	{
		isAimingLocal = !isAimingLocal;
		if (HasAuthority())
			isAimingLocal ? MultiRpc_Scope(isAimingLocal) : MultiRpc_ScopeRelease();
		else
			isAimingLocal ? ServerRpc_Scope(isAimingLocal) : ServerRpc_ScopeRelease();
	}
	else
	{
		isAimingLocal = true;
		if (HasAuthority())
			MultiRpc_Scope(isAimingLocal);
		else
			ServerRpc_Scope(isAimingLocal);
	}
	onScopeSound.Broadcast(isAimingLocal);
	onScope.Broadcast(isAimingLocal);

	if (isAimingLocal && !isShooting)
		SetupAimStarted();
	else
	{
		currentRotationSpeed = rotationSpeed;
		onRotationEnd.RemoveDynamic(this, &AMainCharacter::SetupAimFinished);
	}
}

bool AMainCharacter::CanScope()
{
	const bool _isUsingCalf = usingCalf ? usingCalf->GetIsUsing() : true;
	const bool _hasLeftShoot = usingLeftArm->GetIsUsing();

	return scopeDisable || gameInstance->GetGameStatus() == EGameStatus::HUB || isStun ||
		_isUsingCalf || _hasLeftShoot || interactionComponent->GetHasInteracted();
}

void AMainCharacter::MultiRpc_Scope_Implementation(const bool _value)
{
	if (gameInstance->GetGameStatus() == EGameStatus::HUB) return;
	if (charaAnim->GetIsKicking()) return;

	isAiming = _value;
	if (!isShooting && (charaAnim = Cast<UCharaAnim>(GetMesh()->GetAnimInstance())))
	{
		charaAnim->SetAnimWeight(isAiming ? 1 : -1);
		//charaAnim->SetAnimWeight(isAiming);
		charaAnim->SetIsScoping(isAiming);
	}

	LOG_COLOR(MAIN_CHARA_USE_LOG, "SCOPE MULTI", RED);
}

void AMainCharacter::ServerRpc_Scope_Implementation(const bool _value)
{
	MultiRpc_Scope(_value);
}

void AMainCharacter::ScopeRelease(const FInputActionValue& _value)
{
	LOG_COLOR(false, "RELEASE", RED);
	if (gameInstance->GetGameStatus() == EGameStatus::HUB || isStun) return;
	//SetCameraFightMode();

	if (holdScope && isAimingLocal)
	{
		if (HasAuthority())
			MultiRpc_ScopeRelease();
		else
			ServerRpc_ScopeRelease();
		onScope.Broadcast(isAimingLocal);
		onScopeSound.Broadcast(isAimingLocal);

		currentRotationSpeed = rotationSpeed;
		onRotationEnd.RemoveDynamic(this, &AMainCharacter::SetupAimFinished);
	}
}

void AMainCharacter::ServerRpc_ScopeRelease_Implementation()
{
	MultiRpc_ScopeRelease();
}

void AMainCharacter::MultiRpc_ScopeRelease_Implementation()
{
	if (CanScope()) return;
	isAimingLocal = false;
	isAiming = false;
	if (usingRightArm && !usingRightArm->GetIsPressed())
	{
		isShooting = false;
		if (charaAnim)
		{
			charaAnim->SetIsScoping(isAiming);
			charaAnim->SetAnimWeight(-1);
		}
	}
}

void AMainCharacter::ActivateScope()
{
	canScope = true;
	if (isAimingLocal && !isAiming)
		charaAnim->SetAnimWeight(1);
	isAiming = isAimingLocal;
	charaAnim->SetIsScoping(isAimingLocal);
}

void AMainCharacter::DeactivateScope(const bool _canHold)
{
	canScope = false;
	isAiming = false;
	isShooting = false;
	isAimingLocal = _canHold ? isAimingLocal : false;
	charaAnim->SetIsScoping(false);
}

void AMainCharacter::RightArmStart(const FInputActionValue& _value)
{
	if (!usingRightArm || isStun || inventoryComponent->GetHasEgg())return;
	if (gameInstance->GetGameStatus() == EGameStatus::HUB) return;

	if (HasAuthority())
		MultiRpc_RightArm(_value);
	else
		ServerRpc_RightArm(_value);
	GetWorldTimerManager().ClearTimer(shootTimer);
}

void AMainCharacter::RightArm(const FInputActionValue& _value)
{
	if (!usingRightArm || isStun)return;
	if (gameInstance->GetGameStatus() == EGameStatus::HUB) return;

	if (usingRightArm->CanUse())
	{
		usingRightArm->PressBehavior();
		if (!isShooting)
		{
			if (HasAuthority())
				MultiRpc_RightArm(_value);
			else
				ServerRpc_RightArm(_value);

			if (!isAiming)
				SetupAimStarted();
		}
		GetWorldTimerManager().ClearTimer(shootTimer);
	}

	//PlayAnimMontage(animMontage, 1.f, "Charge");
}

void AMainCharacter::MultiRpc_RightArm_Implementation(const FInputActionValue& _value)
{
	isShooting = true;
	if (!isAiming)
	{
		//LOG_COLOR(true, "Right Arm", YELLOW);
		charaAnim->SetAnimWeight(1);
		charaAnim->SetIsScoping(isShooting);
	}
}

void AMainCharacter::ServerRpc_RightArm_Implementation(const FInputActionValue& _value)
{
	MultiRpc_RightArm(_value);
}

void AMainCharacter::RightArmRelease(const FInputActionValue& _value)
{
	if (gameInstance->GetGameStatus() == EGameStatus::HUB) return;
	if (!usingRightArm || !usingRightArm->GetIsPressed() || isStun)return;
	usingRightArm->ReleaseBehavior();
	GetWorldTimerManager().SetTimer(shootTimer, this, &AMainCharacter::StopShoot, shootTime, false);

	//if (isAiming)
	//	GetWorldTimerManager().SetTimer(_animTimer,this,&AMainCharacter::PlayAimMontage,0.3f,false);
}

void AMainCharacter::StopShoot()
{
	//LOG_COLOR(true, "STOP SHOOT !!", ORANGE);
	if (HasAuthority())
		MultiRpc_StopShoot();
	else
		ServerRpc_StopShoot();
}

void AMainCharacter::MultiRpc_StopShoot_Implementation()
{
	GetWorldTimerManager().ClearTimer(shootTimer);
	if (!isAiming && isShooting)
	{
		charaAnim->SetIsScoping(false);
		charaAnim->SetAnimWeight(-1);

		currentRotationSpeed = rotationSpeed;
		onRotationEnd.RemoveDynamic(this, &AMainCharacter::SetupAimFinished);
	}
	isShooting = false;
}

void AMainCharacter::ServerRpc_StopShoot_Implementation()
{
	MultiRpc_StopShoot();
}

bool AMainCharacter::HasNailGun()
{
	if (!usingRightArm)return false;
	return (usingRightArm->GetMemberType() == EMemberType::NAILGUN);
}

bool AMainCharacter::HasFireflies()
{
	if (!usingRightArm)return false;
	return (usingRightArm->GetMemberType() == EMemberType::FIREFLIES);
}

bool AMainCharacter::HasSnapFlash()
{
	if (!usingLeftArm)return false;
	return (usingLeftArm->GetMemberType() == EMemberType::SNAPFLASH);
}

bool AMainCharacter::HasZBB()
{
	if (!usingLeftArm)return false;
	return (usingLeftArm->GetMemberType() == EMemberType::ZEUSBATTERYBOLT);
}

bool AMainCharacter::HasHauler()
{
	if (!usingThigh)return false;
	return (usingThigh->GetMemberType() == EMemberType::HAULER);
}

bool AMainCharacter::HasRoller()
{
	if (!usingThigh)return false;
	return (usingThigh->GetMemberType() == EMemberType::OFFROADROLLERDRIVES);
}

bool AMainCharacter::HasEjectKick()
{
	if (!usingCalf)return false;
	return (usingCalf->GetMemberType() == EMemberType::EJECTKICK);
}

bool AMainCharacter::HasWCK()
{
	if (!usingCalf)return false;
	return (usingCalf->GetMemberType() == EMemberType::WCKICK);
}

bool AMainCharacter::HasSK()
{
	if (!usingCalf)return false;
	return (usingCalf->GetMemberType() == EMemberType::SEISMICKICK);
}

void AMainCharacter::LeftArmStart(const FInputActionValue& _value)
{
}

void AMainCharacter::LeftArm(const FInputActionValue& _value)
{
	if (gameInstance->GetGameStatus() == EGameStatus::HUB) return;
	if (!usingLeftArm || isStun)return;
	if (usingLeftArm->CanUse())
	{
		if (!isAiming)
			SetupAimStarted();

		usingLeftArm->PressBehavior();

		DeactivateMembers({usingLeftArm, usingThigh, usingCalf});

		FTimerDelegate _delegate = FTimerDelegate::CreateLambda([&]
		{
			if (!usingLeftArm->CanUse()) return;
			isAiming = true;
			charaAnim->SetIsScoping(true);
		});
		GetWorld()->GetTimerManager().SetTimer(leftArmAimTimer, _delegate, leftArmAimDelay, false);

		isAimingLocal = false;
		isShooting = false;
		canScope = false;
		charaAnim->SetIsScoping(false);
		onScope.Broadcast(false);
	}
}

void AMainCharacter::MultiRpc_LeftArm_Implementation(const FInputActionValue& _value)
{
	/*if (!isAiming)
	{
	    if (isOwner)
	    {
	        GetWorldTimerManager().ClearTimer(shootTimer);
	        GetWorldTimerManager().SetTimer(shootTimer, this, &AMainCharacter::StopShoot, shootTime, false);
	    }
	    isShooting = true;
	    charaAnim->SetIsScoping(true);
	}*/
}

void AMainCharacter::ServerRpc_LeftArm_Implementation(const FInputActionValue& _value)
{
	MultiRpc_LeftArm(_value);
}

void AMainCharacter::LeftArmRelease(const FInputActionValue& _value)
{
	if (gameInstance->GetGameStatus() == EGameStatus::HUB) return;
	if (!usingLeftArm || !usingLeftArm->GetIsPressed() || isStun) return;
	usingLeftArm->ReleaseBehavior();
	currentRotationSpeed = rotationSpeed;
	GetWorld()->GetTimerManager().ClearTimer(leftArmAimTimer);
	isAiming = false;
	isAimingLocal = false;
	//ActivateScope();
	onRotationEnd.RemoveDynamic(this, &AMainCharacter::SetupAimFinished);
}

void AMainCharacter::Calf(const FInputActionValue& _value)
{
	if (!usingCalf || isStun) return;
	if (gameInstance->GetGameStatus() == EGameStatus::HUB) return;
	if (usingCalf->CanUse())
	{
		GetWorld()->GetTimerManager().ClearTimer(leftArmAimTimer);
		usingCalf->PressBehavior();
		isAiming = false;
		isAimingLocal = false;
		isShooting = false;
		charaAnim->SetIsScoping(false);
		onScope.Broadcast(false);
		charaAnim->ClearAllAnimWeight();
		currentRotationSpeed = rotationSpeed;
		DeactivateMembers({usingCalf});
		GetWorldTimerManager().ClearTimer(shootTimer);
	}
}

void AMainCharacter::CalfRelease(const FInputActionValue& _value)
{
	if (gameInstance->GetGameStatus() == EGameStatus::HUB) return;
	if (!usingCalf || !usingCalf->GetIsPressed() || isStun)return;
	usingCalf->ReleaseBehavior();
}

void AMainCharacter::Thigh(const FInputActionValue& _value)
{
	if (gameInstance->GetGameStatus() == EGameStatus::HUB) return;
	if (!usingThigh || isStun)return;
	if (usingThigh->CanUse())
	{
		//ça reste
		GetWorld()->GetTimerManager().ClearTimer(leftArmAimTimer);
		canScope = false;
		isAiming = false;
		isShooting = false;
		isAimingLocal = false;
		charaAnim->SetIsScoping(false);
		onScope.Broadcast(false);
		usingThigh->PressBehavior();
		charaAnim->ClearAllAnimWeight();
		currentRotationSpeed = rotationSpeed;
		GetWorldTimerManager().ClearTimer(shootTimer);
		//
		// 
		//ça degage 
	}
}

void AMainCharacter::ThighRelease(const FInputActionValue& _value)
{
	if (gameInstance->GetGameStatus() == EGameStatus::HUB) return;
	if (!usingThigh || !usingThigh->GetIsPressed() || isStun)return;
	usingThigh->ReleaseBehavior();
}

void AMainCharacter::SetCameraExploMode()
{
	if (gameInstance->GetGameStatus() == EGameStatus::EXPLORATION) return;
	LOG_COLOR(MAIN_CHARA_USE_LOG, "Explo Mode", GREEN);
	gameInstance->SetGameStatus(EGameStatus::EXPLORATION);
	//cameraComponent->SetIsDown(false);
	cameraComponent->UpdateCurrentSettings("Exploration");
	if (HasAuthority())
		currentSpeedMode = ESpeedMode::RUN;
	else
		ServerRpc_SetClientSpeedMode(ESpeedMode::RUN);
}

void AMainCharacter::SetCameraHUBMode()
{
	LOG_COLOR(MAIN_CHARA_USE_LOG, "Set HUB Mode", YELLOW);
	//cameraComponent->SetIsDown(false);
	cameraComponent->UpdateCurrentSettings("HUB");
}

void AMainCharacter::SetCameraStealthMode()
{
	if (gameInstance->GetGameStatus() == EGameStatus::STEALTH) return;
	LOG_COLOR(MAIN_CHARA_USE_LOG, "Stealth Mode", RED);
	gameInstance->SetGameStatus(EGameStatus::STEALTH);
	//cameraComponent->SetIsDown(false);
	cameraComponent->UpdateCurrentSettings("S");
	if (HasAuthority())
		currentSpeedMode = ESpeedMode::RUN;
	else
		ServerRpc_SetClientSpeedMode(ESpeedMode::RUN);
}

void AMainCharacter::SetCameraFightMode()
{
	if (gameInstance->GetGameStatus() == EGameStatus::FIGHTING) return;
	LOG_COLOR(MAIN_CHARA_USE_LOG, "Fight Mode", RED);
	gameInstance->SetGameStatus(EGameStatus::FIGHTING);
	//cameraComponent->SetIsDown(false);
	cameraComponent->UpdateCurrentSettings("C");
	if (HasAuthority())
		currentSpeedMode = ESpeedMode::RUN;
	else
		ServerRpc_SetClientSpeedMode(ESpeedMode::RUN);
}

void AMainCharacter::SetCameraShootMode(FString _cameraKey)
{
	if (gameInstance->GetGameStatus() == EGameStatus::SHOOTING) return;
	LOG_COLOR(MAIN_CHARA_USE_LOG, "Shoot Mode", RED);
	gameInstance->SetGameStatus(EGameStatus::SHOOTING);
	//cameraComponent->SetIsDown(false);
	cameraComponent->UpdateCurrentSettings(_cameraKey);
	if (HasAuthority())
		currentSpeedMode = ESpeedMode::SHOOT;
	else
		ServerRpc_SetClientSpeedMode(ESpeedMode::SHOOT);
}

void AMainCharacter::SetDeadMode(const FInputActionValue& _value)
{
	SetDeath();
}

void AMainCharacter::LaunchDeathTransition()
{
	AGameHUD* _hud = GETFPC->GetHUD<AGameHUD>();
	CHECK_NULL(_hud, "HUD is null");
	_hud->SetState(EStateUI::DayEnd);
	UEndDay* _endDayUI = _hud->GetCurrentUI<UEndDay>();
	if (_endDayUI)
	{
		CHECK_NULL(_endDayUI, "End day HUD is null");
		_endDayUI->Callback_OnDeathPlayer();
	}

	if (!_endDayUI)
	{
		UCustomGameInstance* _gi = Cast<UCustomGameInstance>(GetWorld()->GetGameInstance());
		_gi->OnLevelLoad().AddUniqueDynamic(this, &AMainCharacter::ToHub);
		_gi->CallbackOnLevelChange(ETypeChange::DeathToHub);
		
	}
	//UGameplayStatics::OpenLevel(GetWorld(), FName(_gi->GetHubLevel().GetAssetName()), false);
}

void AMainCharacter::ToHub()
{
	if (UOnlineManagerSubsystem* _oms = GetWorld()->GetGameInstance()->GetSubsystem<UOnlineManagerSubsystem>())
		_oms->LoadMap(GetWorld()->GetGameInstance<UCustomGameInstance>()->GetHubLevel().GetAssetName());
}

void AMainCharacter::SetDeath()
{
	if (lifeComponent)
		if (lifeComponent->GetCanRevive())
			return;

	if (isDown) return;


	isShooting = false;
	isAiming = false;
	isAimingLocal = false;
	charaAnim->SetIsScoping(false);
	DeactivateMembers({});
	charaAnim->ClearAllAnimWeight();
	isDown = true;

	if (dayManager->GetCurrentDay() == 3)
		UnlockAchievement("D-3");

	USaveManagerSubsystem* _saveManager = GetWorld()->GetGameInstance()->GetSubsystem<USaveManagerSubsystem>();
	CHECK_NULL(_saveManager, "Save manager is null");
	UGameplaySave* _save = _saveManager->GetCurrentSave();
	CHECK_NULL(_save, "Pas de save ! Lance depuis le main menu fdp !");
	_save->SaveGameplay(this);
	_save->SaveBonus(this);
	LOG_COLOR_TIME(true, "YOU ARE DOWN !", RED, 20);

	FTimerHandle _deathTimer;
	GetWorld()->GetTimerManager().SetTimer(_deathTimer, this, &AMainCharacter::LaunchDeathTransition, deathTime);
}

void AMainCharacter::SetExploMode(const FInputActionValue& _value)
{
	//SetCameraExploMode();
}

void AMainCharacter::SetStealthMode(const FInputActionValue& _value)
{
	//SetCameraStealthMode();
}

void AMainCharacter::SetFightMode(const FInputActionValue& _value)
{
	//SetCameraFightMode();
}

void AMainCharacter::SetShootMode(const FInputActionValue& _value)
{
	//SetCameraShootMode("C");
}

void AMainCharacter::SetBIG(const FInputActionValue& _value)
{
	LOG_COLOR(MAIN_CHARA_USE_LOG, "TIER1! => Weight > 15Kg", RED);
	if (HasAuthority())
		currentWeightState = EWeightState::TIER1;
	else
		ServerRpc_SetWeightState(EWeightState::TIER1);
}

void AMainCharacter::SetFAT(const FInputActionValue& _value)
{
	LOG_COLOR(MAIN_CHARA_USE_LOG, "FAT! => Weight > 30Kg", RED);
	if (HasAuthority())
		currentWeightState = EWeightState::TIER2;
	else
		ServerRpc_SetWeightState(EWeightState::TIER2);
}

void AMainCharacter::SetHEAVY(const FInputActionValue& _value)
{
	LOG_COLOR(MAIN_CHARA_USE_LOG, "HEAVY! => Weight > 60Kg", RED);
	if (HasAuthority())
		currentWeightState = EWeightState::TIER3;
	else
		ServerRpc_SetWeightState(EWeightState::TIER3);
}

void AMainCharacter::SetMASSIVE(const FInputActionValue& _value)
{
	LOG_COLOR(MAIN_CHARA_USE_LOG, "MASSIVE! => Weight > 90Kg", RED);
	if (HasAuthority())
		currentWeightState = EWeightState::TIER4;
	else
		ServerRpc_SetWeightState(EWeightState::TIER4);
}

void AMainCharacter::SetNOWEIGHT(const FInputActionValue& _value)
{
	LOG_COLOR(MAIN_CHARA_USE_LOG, "LIGHT! => Weight < 15Kg", RED);
	if (HasAuthority())
		currentWeightState = EWeightState::NOWEIGHT;
	else
		ServerRpc_SetWeightState(EWeightState::NOWEIGHT);
}

void AMainCharacter::AddMemberToChara()
{
	ResetCharaMember();
	SetRightArm(rightArmEditor);
	SetLeftArm(leftArmEditor);
	SetCalf(calfEditor);
	SetThigh(thighEditor);
}

void AMainCharacter::ResetCharaMember()
{
	if (usingRightArm)
	{
		usingRightArm->GetMemberMesh()->DestroyComponent();
		usingMembers.Remove(usingRightArm);
		GetWorld()->DestroyActor(usingRightArm);
	}

	if (usingLeftArm)
	{
		usingLeftArm->GetMemberMesh()->DestroyComponent();
		usingMembers.Remove(usingLeftArm);
		GetWorld()->DestroyActor(usingLeftArm);
	}

	if (usingCalf)
	{
		usingMembers.Remove(usingCalf);
		GetWorld()->DestroyActor(usingCalf);
	}

	if (usingThigh)
	{
		usingThigh->GetLeftMemberMesh()->DestroyComponent();
		usingThigh->GetRightMemberMesh()->DestroyComponent();
		usingMembers.Remove(usingThigh);
		GetWorld()->DestroyActor(usingThigh);
	}
}

//void AMainCharacter::ServerRpc_InitVoiceChat_Implementation()
//{
//	MultiRpc_InitVoiceChat();
//}
//
//void AMainCharacter::MultiRpc_InitVoiceChat_Implementation()
//{
//	voiceChatComponent->InitVoiceChat();
//}

void AMainCharacter::PlayerDestroyVoiceChat()
{
	//GetVoiceChatComponent()->Mute(true);
	//GetVoiceChatComponent()->DestroyVoip();
	ServerRpc_DestroyVoiceChat();
}

void AMainCharacter::ServerRpc_DestroyVoiceChat_Implementation()
{
	//GetVoiceChatComponent()->Mute(true);
	//GetVoiceChatComponent()->DestroyVoip();
}

void AMainCharacter::PlayerInitVoiceChat()
{
	if (!isOwner) return;
	//voiceChatComponent->InitVoiceChat();
}

void AMainCharacter::CreateUI()
{
	//voiceChatUIObj = CreateWidget(GetWorld(), voiceChatUI);
}

void AMainCharacter::OnPlayerStateChanged(APlayerState* NewPlayerState, APlayerState* OldPlayerState)
{
	if (!GetPlayerState()) return;

	APlayerController* _ctrl = GetWorld()->GetFirstPlayerController();
	isOwner = _ctrl == Controller;
	if (ACustomGameState* _gamestate = Cast<ACustomGameState>(GetWorld()->GetGameState()))
		_gamestate->Register(this, isOwner);
	PlayerInitVoiceChat();
}

void AMainCharacter::PushToTalk(const FInputActionValue& _value)
{
	bool _isPush = _value.Get<bool>();
	//voiceChatComponent->PushToTalk(_isPush);
}

void AMainCharacter::ServerRpc_AddModifier_Implementation(const ESpeedModifiers& _modifier)
{
	currentSpeedModifier *= (speedModifiers[_modifier] / 100);
}

void AMainCharacter::ServerRpc_RemoveModifier_Implementation(const ESpeedModifiers& _modifier)
{
	currentSpeedModifier /= (speedModifiers[_modifier] / 100);
}

void AMainCharacter::ServerRPC_SetModifier_Implementation(const ESpeedModifiers& _modifier)
{
	currentSpeedModifier = speedModifiers[_modifier];
}

void AMainCharacter::ServerRpc_SetCurrentSpeedModifier_Implementation(const ESpeedModifiers& _modifier,
                                                                      const float _percent)
{
	const float _baseValue = speedModifiers[NOMODIFIERS];
	const float _percentValue = (_baseValue - speedModifiers[_modifier]) * (_percent / 100.0f);
	const float _newValue = _baseValue - _percentValue;
	currentSpeedModifier = _newValue < currentSpeedModifier ? _newValue : currentSpeedModifier;
}

void AMainCharacter::ServerRpc_ResetCurrentSpeedModifier_Implementation()
{
	const float _baseValue = speedModifiers[NOMODIFIERS];
	currentSpeedModifier = _baseValue;
}


void AMainCharacter::UnlockAchievement(const FName& _achievementName)
{
	ACustomPlayerController* _pc = Cast<ACustomPlayerController>(GetWorld()->GetFirstPlayerController());
	if (_pc)
		_pc->UpdateLeaderboard(_achievementName,1);
}
