


#include "MapPawn.h"

#include "CommonInputSubsystem.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "FrameTypes.h"
#include "Android/AndroidPlatformCompilerSetup.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Engine/TextureRenderTarget2D.h"
#include "GameFramework/SpringArmComponent.h"
#include "RobotHunter/Game/GPE/Handcar/CustomHandcarActor.h"
#include "RobotHunter/Game/Interactable/Handcar/NitroHandcarActor.h"
#include "RobotHunter/UI/HUD/MainHUD.h"
#include "RobotHunter/Utils/DebugUtils.h"

// Sets default values
AMapPawn::AMapPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	springArm = CreateDefaultSubobject<USpringArmComponent>("springArm");
	mapCapture = CreateDefaultSubobject<USceneCaptureComponent2D>("mapCapture");
	RootComponent = CreateDefaultSubobject<UStaticMeshComponent>("meshTest");
	springArm->SetupAttachment(RootComponent);
	mapCapture->SetupAttachment(springArm);

	springArm->TargetArmLength = 500.f; // Distance de la caméra par rapport au Pawn
}



// Called when the game starts or when spawned
void AMapPawn::BeginPlay()
{
	Super::BeginPlay();
	ChangeSceneCaptureResolution(mapCapture,1920,1080);
	EnablePlayerContext();
	bIsLerping = true;
}

void AMapPawn::MoveNext(const FInputActionValue& _value)
{
	if (bIsLerping)
		return;
	if (socket.Num() == 0)
	{
		LOG_COLOR(DEBUG_AMAP_PAWN,"No sockets in the map!",RED);
		return;
	}

	TArray<FName> _socketKeys;
	socket.GetKeys(_socketKeys);

	int32 _currentIndex = _socketKeys.IndexOfByKey(currentSocketName);

	if (_currentIndex == INDEX_NONE)
	{
		LOG_COLOR(DEBUG_AMAP_PAWN, FString::Printf(TEXT("Current socket %s not found in the map!"), *currentSocketName.ToString()), FColor::Red);
		return;
	}

	int32 _nextIndex = (_currentIndex + 1) % _socketKeys.Num();
	currentSocketName = _socketKeys[_nextIndex];

	//const FVector& _targetLocation = socket[currentSocketName];
	startLocation = GetActorLocation();
	currentLerpAlpha = 0.0f;
	bIsLerping = true;
}

void AMapPawn::MovePrevious(const FInputActionValue& _value)
{
	if (bIsLerping)
		return;
	if (socket.Num() == 0)
	{
		LOG_COLOR(DEBUG_AMAP_PAWN,"No sockets in the map!",RED);
		return;
	}

	TArray<FName> _socketKeys;
	socket.GetKeys(_socketKeys);

	int32 _currentIndex = _socketKeys.IndexOfByKey(currentSocketName);

	if (_currentIndex == INDEX_NONE)
	{
		LOG_COLOR(DEBUG_AMAP_PAWN, FString::Printf(TEXT("Current socket %s not found in the map!"), *currentSocketName.ToString()), FColor::Red);
		return;
	}

	int32 _nextIndex = (_currentIndex - 1) % _socketKeys.Num();
	if (_nextIndex < 0)
		_nextIndex = _socketKeys.Num() - 1;
	
	currentSocketName = _socketKeys[_nextIndex];

	//const FVector& _targetLocation = socket[currentSocketName];
	startLocation = GetActorLocation();
	currentLerpAlpha = 0.0f;
	bIsLerping = true;
}

// Called every frame
void AMapPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (currentSocketName == "MainCharacter")
	{
		socket["MainCharacter"] = mainCharacter->GetActorLocation() / 100;
		bIsLerping = true;
	}
	if (bIsLerping)
	{
		currentLerpAlpha += DeltaTime / lerpTimeSpeed;
		const float _alpha = FMath::Clamp(currentLerpAlpha, 0.0f, 1.0f);
		const FVector _targetLocation = FMath::Lerp(startLocation, socket[currentSocketName], _alpha);
		SetActorLocation(_targetLocation);
		if (_alpha >= 1.0f)
		{
			bIsLerping = false;
		}
	}
}

void AMapPawn::Rotate(const FInputActionValue& _value)
{
	bool _isGamepad = false;
	if (UCommonInputSubsystem* _commonInputSystem = UCommonInputSubsystem::Get(GETFPC->GetLocalPlayer()))
	{
		_isGamepad = _commonInputSystem->GetCurrentInputType() == ECommonInputType::Gamepad;
	}
	
	const float _axis = _value.Get<float>() * speedRotateYaw ;
	
	USpringArmComponent* _arm = springArm;
	_arm->AddRelativeRotation(FRotator(0, _axis, 0));
}

void AMapPawn::RotatePitchCamera(const FInputActionValue& _value)
{
	bool _isGamepad = false;
	if (const UCommonInputSubsystem* _commonInputSystem = UCommonInputSubsystem::Get(GETFPC->GetLocalPlayer()))
	{
		_isGamepad = _commonInputSystem->GetCurrentInputType() == ECommonInputType::Gamepad;
	}
	const float _axis = _value.Get<float>() * speedRotatePitch;

	USpringArmComponent* _arm = springArm;
	int _pitch = springArm->GetRelativeRotation().Pitch;
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

void AMapPawn::EscapeMenu(const FInputActionValue& input_action_value)
{
	mainCharacter->EscapeMenu(input_action_value);
}

void AMapPawn::ZoomCamera(const FInputActionValue& input_action_value)
{
	const float _zoomValue = input_action_value.Get<float>();
	if (springArm)
	{
		const float _newTargetArmLength = FMath::Clamp(springArm->TargetArmLength - _zoomValue * zoomSpeed, minZoom, maxZoom);
		springArm->TargetArmLength = _newTargetArmLength;
	}
}

void AMapPawn::FocusMap()
{
	LOG_COLOR(true,  bIsCameraFocused ? TEXT("true") : TEXT("false"), FColor::Red);
	
	const AMainHUD* hud = Cast<AMainHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
	APlayerController* _player = GetWorld()->GetFirstPlayerController();

	if (!bIsCameraFocused)
	{
		bIsCameraFocused = true;
		hud->GetFocusSystem()->SetFocusTo(EFocusMode::GAME,mainCharacter,false);		
		UGameplayStatics::SetViewportMouseCaptureMode(hud->GetWorld(), EMouseCaptureMode::CapturePermanently_IncludingInitialMouseDown);

	}
	else
	{
		bIsCameraFocused = false;
		hud->GetFocusSystem()->SetFocusTo(EFocusMode::UI,mainCharacter, false);		
		UGameplayStatics::SetViewportMouseCaptureMode(hud->GetWorld(), EMouseCaptureMode::CapturePermanently_IncludingInitialMouseDown);

	}
}

// Called to bind functionality to input
void AMapPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (!inputs)
		return;
	UEnhancedInputComponent* _input = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	_input->BindAction(inputs->GetNextInput(), ETriggerEvent::Completed, this , &AMapPawn::MoveNext);
	_input->BindAction(inputs->GetPreviousInput(), ETriggerEvent::Completed, this , &AMapPawn::MovePrevious);
	_input->BindAction(inputs->GetRotateMapCamera(), ETriggerEvent::Triggered, this, &AMapPawn::Rotate);
	_input->BindAction(inputs->GetRotateMapCameraPitch(), ETriggerEvent::Triggered, this, &AMapPawn::RotatePitchCamera);
	_input->BindAction(inputs->GetEscapeInput(), ETriggerEvent::Started, this, &AMapPawn::EscapeMenu);
	_input->BindAction(inputs->GetZoomMapCamera(),ETriggerEvent::Started, this, &AMapPawn::ZoomCamera);
	//_input->BindAction(inputs->GetFocusMap(), ETriggerEvent::Started, this, &AMapPawn::FocusMap);
	//_input->BindAction(inputs->GetFocusMap(), ETriggerEvent::Completed, this, &AMapPawn::FocusMap);
	//_input->BindAction(inputs->GetDeFocusMap(), ETriggerEvent::Completed, this, &AMapPawn::FocusMap);
}

void AMapPawn::ChangeSceneCaptureResolution(const USceneCaptureComponent2D* _sceneCapture, const int32 _newWidth, const int32 _newHeight) const
{
	if (_sceneCapture && _sceneCapture->TextureTarget)
	{
		// Récupère la Render Target associée
		UTextureRenderTarget2D* RenderTarget = _sceneCapture->TextureTarget;

		// Modifie les dimensions de la Render Target
		RenderTarget->ResizeTarget(_newWidth, _newHeight);

		LOG_COLOR(DEBUG_AMAP_PAWN, FString::Printf(TEXT("Render Target resolution changed to: %d x %d"), _newWidth, _newHeight), FLinearColor::Red);
	}

}

void AMapPawn::AddShowOnlyActor(AActor* _actor)
{
	mapCapture->ShowOnlyActors.Add(_actor);
}

void AMapPawn::EnablePlayerContext()
{
	if (!inputs)
		return;
	APlayerController* _player = GetWorld()->GetFirstPlayerController();
	UEnhancedInputLocalPlayerSubsystem* _inputSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(
		_player->GetLocalPlayer());
	_inputSystem->ClearAllMappings();
	_inputSystem->AddMappingContext(currentPlayerContext.LoadSynchronous(), 0);
}
