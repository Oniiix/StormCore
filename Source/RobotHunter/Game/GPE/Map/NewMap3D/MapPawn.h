

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "RobotHunter/CCC/Character/MainCharacter.h"
#include "RobotHunter/CCC/Character/Inputs/InputConfig.h"
#include "MapPawn.generated.h"

class USpringArmComponent;

#define DEBUG_AMAP_PAWN true

UCLASS()
class ROBOTHUNTER_API AMapPawn : public APawn
{
	GENERATED_BODY()

	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess) , Category = "Properties")
	TObjectPtr<AMainCharacter> mainCharacter = nullptr;
	UPROPERTY(EditAnywhere, Category = "Properties|Map")
	TObjectPtr<USceneCaptureComponent2D> mapCapture = nullptr;
	UPROPERTY(EditAnywhere, Category = "Camera", BluePrintReadWrite,
			meta = (AllowPrivateAccess, UIMin = 0, UIMax = 80, ClampMin = 0, ClampMax = 80))
	int cameraClampMin = 0;
	UPROPERTY(EditAnywhere, Category = "Camera", BluePrintReadWrite,
			meta = (AllowPrivateAccess, UIMin = 0, UIMax = 80, ClampMin = 0, ClampMax = 80))
	int cameraClampMax = 80;
	UPROPERTY(EditAnywhere, Category = "Properties|Camera")
	TObjectPtr<USpringArmComponent> springArm = nullptr;
	UPROPERTY(EditAnywhere, Category = "Properties|Camera")
	float speedRotateYaw = 1;
	UPROPERTY(EditAnywhere, Category = "Properties|Camera")
	float speedRotatePitch = 1;
	UPROPERTY(EditAnywhere, Category = "Properties|Camera")
	TObjectPtr<UCameraComponent> camera = nullptr;
	UPROPERTY(EditAnywhere, Category = "Properties|Input")
	TObjectPtr<UInputConfig> inputs = nullptr;
	UPROPERTY(EditAnywhere, Category = "Properties|Input")
	TSoftObjectPtr<UInputMappingContext> currentPlayerContext;
	UPROPERTY(EditAnywhere,Category = "Properties|Debug")
	TObjectPtr<UStaticMeshComponent> meshTest;
	UPROPERTY(EditAnywhere, Category = "Properties|Camera")
	float zoomSpeed = 50.f;
	UPROPERTY(EditAnywhere, Category = "Properties|Camera")
	float minZoom = 100.f;
	UPROPERTY(EditAnywhere, Category = "Properties|Camera")
	float maxZoom = 1500.f;
	UPROPERTY(VisibleAnywhere)
	TMap<FName,FVector> socket;
	UPROPERTY()
	FName currentSocketName;
	


#pragma region Lerp
	FVector startLocation;
	float currentLerpAlpha = 0.0f;
	bool bIsLerping = false;
	// Temps de lerp en seconde
	UPROPERTY(EditAnywhere, Category = "Properties")
	float lerpTimeSpeed = 1 ;
#pragma endregion
	bool bIsCameraFocused = false;
	
public:
#pragma region Getter/Setter
	FORCEINLINE AMainCharacter* GetMainCharacter() const { return mainCharacter; }
	FORCEINLINE void SetMainCharacter(const TObjectPtr<AMainCharacter>& _mainCharacter) { mainCharacter = _mainCharacter;}
	FORCEINLINE void SetSocket(const FName& _socketName, const FVector& _location) { socket.Add(_socketName, _location); }
	FORCEINLINE void SetSocket(const TMap<FName,FVector>& _socket) { socket = _socket; }
	FORCEINLINE TMap<FName,FVector>& GetSocket() { return socket; }
	FORCEINLINE FName GetCurrentSocketName() const { return currentSocketName; }
	FORCEINLINE void SetCurrentSocketName(const FName _currentSocketName) { currentSocketName = _currentSocketName;}
	FORCEINLINE USceneCaptureComponent2D* GetMapCapture() const { return mapCapture; }
	FORCEINLINE TObjectPtr<USpringArmComponent> GetSpringArm() const { return springArm;}
	FORCEINLINE void SetSpringArmLength(const int _springArmLength) const { springArm->TargetArmLength = _springArmLength; }
	
#pragma endregion
	
	AMapPawn();
protected:
	virtual void BeginPlay() override;
	void MoveNext(const FInputActionValue& _value);
	void MovePrevious(const FInputActionValue& _value);

public:	
	virtual void Tick(float DeltaTime) override;
	void Rotate(const FInputActionValue& _value);
	void RotatePitchCamera(const FInputActionValue& _value);
	void EscapeMenu(const FInputActionValue& input_action_value);
	void ZoomCamera(const FInputActionValue& input_action_value);
	UFUNCTION()
	void FocusMap();
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void ChangeSceneCaptureResolution(const USceneCaptureComponent2D* _sceneCapture, int32 _newWidth, int32 _newHeight) const;
	void AddShowOnlyActor(AActor* _actor);
	void EnablePlayerContext();
};
