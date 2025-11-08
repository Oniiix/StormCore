
#include "STTask_SelectMovement.h"

EStateTreeRunStatus USTTask_SelectMovement::EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition)
{
	Super::EnterState(Context, Transition);

    APlayerController* _playerController = GetWorld()->GetFirstPlayerController();
    if (!_playerController)
        return EStateTreeRunStatus::Failed;

    const FVector& _ownerLocation = owner->GetActorLocation();
    FVector _cameraLocation;
    FRotator _cameraRotation;
    _playerController->GetPlayerViewPoint(_cameraLocation, _cameraRotation);
    const FVector& _playerLocation = _playerController->GetPawn()->GetActorLocation();
    const FVector& _cameraToOwnerDirection = _ownerLocation - _cameraLocation;
    const FVector& _localDirection = _cameraRotation.UnrotateVector(_cameraToOwnerDirection).GetSafeNormal();

    if (FVector::Dist2D(_playerLocation, _ownerLocation) <= 400)
        return EStateTreeRunStatus::Failed;

    /// Check in frostum
    const float _angleX = FMath::RadiansToDegrees(FMath::Atan2(_localDirection.Y, _localDirection.X));
    const float _angleY = FMath::RadiansToDegrees(FMath::Atan2(_localDirection.Z, _localDirection.X));
    const bool _isInFrostum = FMath::Abs(_angleX) <= (FOV * 0.5f) && FMath::Abs(_angleY) <= (FOV * 0.5f);
    /// Check in front of player
    const float _distanceCameraToPlayer = FVector::Dist2D(_cameraLocation, _playerLocation);
    const float _distanceCameraToOwner = FVector::Dist2D(_cameraLocation, _ownerLocation);
    const bool _isInFront = _distanceCameraToPlayer < _distanceCameraToOwner;

	if (_isInFrostum && _isInFront)
        return EStateTreeRunStatus::Succeeded;
	else
        return EStateTreeRunStatus::Failed;
}
