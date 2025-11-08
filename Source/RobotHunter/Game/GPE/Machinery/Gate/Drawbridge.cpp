#include "Drawbridge.h"
#include "RobotHunter/Utils/DebugUtils.h"


ADrawbridge::ADrawbridge()
{
	//drawBridge = CreateDefaultSubobject<UStaticMeshComponent>("Drawbridge");
	//drawBridge->SetupAttachment(mesh);
}

//
//void ADrawbridge::BeginPlay()
//{
//	Super::BeginPlay();
//
//	roll = GetActorRotation().Roll;
//	yaw = GetActorRotation().Yaw;
//
//	if(!HasAuthority())
//		PrimaryActorTick.SetTickFunctionEnable(false);
//
//	if (receiverSystem)
//		receiverSystem->OnActivate().AddDynamic(this, &ADrawbridge::HowSwitch);
//}
//void ADrawbridge::Tick(float DeltaTime)
//{
//	Super::Tick(DeltaTime);
//	Move();
//}
//
//
//
//void ADrawbridge::Move()
//{
//
//	if (openGate)
//	{
//		DRAW_CIRCLE(GetActorLocation(), 100, FColor::Blue, GetActorForwardVector(), GetActorRightVector());
//		
//		SetActorRotation(GetRotationForGate(false));
//	}
//	else if (closeGate)
//	{
//		DRAW_CIRCLE(GetActorLocation(), 100, FColor::Red, GetActorForwardVector(), GetActorRightVector());
//		SetActorRotation(GetRotationForGate(true));
//	}
//}
//void ADrawbridge::Open()
//{
//	closeGate = false,
//	openGate = true;
//
//}
//void ADrawbridge::Close()
//{
//	openGate = false;
//	closeGate = true;
//}
//
//
//FRotator ADrawbridge::GetRotationForGate(const bool& _closeGate)
//{
//	const float& _speed = _closeGate==false? (90 / timeToOpen) * GetWorld()->DeltaTimeSeconds : (90 / timeToClose) * GetWorld()->DeltaTimeSeconds;
//
//	return FRotator(CalculateAngle(_speed, GetActorRotation().Pitch, _closeGate), yaw, roll);
//}
//float ADrawbridge::GetRotationDuringOpening(const float& _a, const float& _calculeAngle)
//{
//	if (_calculeAngle < 90.0f)
//	{
//		//LOG_WARNING(1, "GetRotationDuringOpening" + socketToRotate->GetActorRotation().Pitch);
//		return _calculeAngle + _a;
//	}
//	else
//	{
//		
//		openGate = false;
//		return 90.0f;
//
//	}
//}
//float ADrawbridge::GetRotationDuringClosing(const float& _a, const float& _calculeAngle)
//{
//	if (_calculeAngle > 0.0f)
//	{
//		
//		return _calculeAngle -_a;
//	}
//	else
//	{
//		
//
//		closeGate = false;
//		return 0.0f;
//	}
//}
//float ADrawbridge::CalculateAngle(const float& _a, const float& _calculeAngle, const bool& _inverse)
//{
//	if (_inverse) return GetRotationDuringClosing(_a, _calculeAngle);
//
//	return GetRotationDuringOpening(_a, _calculeAngle);
//
//}
//
//
//void ADrawbridge::HowSwitch()
//{
//	if (!activateRotation)
//		return;
//	float g = GetActorRotation().Pitch;
//
//
//	if (GetActorRotation().Pitch <= 0.0f) openGate ? Close() : Open();
//	
//	else if (GetActorRotation().Pitch >= 90.0f)closeGate ? Open() : Close();
//
//	else  closeGate ? Close() : Open();
//	
//}