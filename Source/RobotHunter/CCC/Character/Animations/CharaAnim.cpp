// Fill out your copyright notice in the Description page of Project Settings.


#include "CharaAnim.h"
#include "RobotHunter/Utils/DebugUtils.h"
#include "RobotHunter/Utils/CustomMathBlueprintLibrary.h"
#include <RobotHunter/Game/GPE/Members/RightArmActors/NailGun/NailGunActor.h>
#include <RobotHunter/Game/GPE/Members/RightArmActors/Fireflies/FirefliesActor.h>
#include <RobotHunter/Game/GPE/Members/CalfActors/EjectKick/EjectKickActor.h>
#include <RobotHunter/Game/GPE/Members/CalfActors/WorldChampionsKick/WorldChampionsKickActor.h>
//#include <RobotHunter/Network/Component/NetworkGPEComponent.h>
#include "RobotHunter/Utils/FileLogger.h"


void UCharaAnim::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	chara = Cast<AMainCharacter>(GetOwningActor());

	if (!chara)
	{
		LOG_ERROR(CHARA_ANIM_USE_LOG, "CHARA NUL EN ANIM");
		return;
	}
	else
		chara->InitAnimInstance(this);
}

void UCharaAnim::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	if (!lerpAnimWeight) return;

	//LERP
	animWeight = FMath::FInterpConstantTo(animWeight, targetAnimWeight, DeltaSeconds, lerpSpeed);

	if (UCustomMathBlueprintLibrary::IsFloatEqual(animWeight, targetAnimWeight, 0.01f))
	{
		animWeight = targetAnimWeight;
		lerpAnimWeight = false;
	}
}
void UCharaAnim::DrawSphereAroundCharacter()
{
	if (chara)
		DRAW_SPHERE(chara->GetActorLocation(), 50, FColor::Blue);
	else
		LOG_COLOR_TICK(true, "Chara is null", RED);
}


void UCharaAnim::Bind()
{
	chara->OnMove().AddUniqueDynamic(this, &UCharaAnim::SetSpeed);
	chara->OnJump().AddUniqueDynamic(this, &UCharaAnim::SetIsJumping);
	//chara->OnScope().AddUniqueDynamic(this, &UCharaAnim::SetIsScoping);
	chara->OnInteract().AddUniqueDynamic(this, &UCharaAnim::SetHasInteract);
	chara->OnBump().AddUniqueDynamic(this, &UCharaAnim::SetIsBumped);
	chara->OnStrafeTurn().AddUniqueDynamic(this, &UCharaAnim::SetStrafeYaw);
	BindRightArms();
	BindLeftArms();
	BindCalfs();
	LOG_COLOR(CHARA_ANIM_USE_LOG, FString::FromInt(chara->OnScope().GetAllObjects().Num()), BLUE);
}

void UCharaAnim::BindCalfs()
{
	if (chara->HasEjectKick())
	{
		//Cast<AEjectKickActor>(chara->GetUsingCalf())->OnEjectKick().AddUniqueDynamic(this, &UCharaAnim::SetHasStartKick);
		Cast<AEjectKickActor>(chara->GetUsingCalf())->OnReachKick().AddUniqueDynamic(this, &UCharaAnim::SetHasReachKick);
	}
	if (chara->HasWCK())
	{
		Cast<AWorldChampionsKickActor>(chara->GetUsingCalf())->OnWCKick().AddUniqueDynamic(this, &UCharaAnim::SetHasStartWCKick);
	}
}

void UCharaAnim::BindLeftArms()
{
	if (!chara->GetUsingLeftArm())return;
	/*chara->GetUsingLeftArm()->OnUseLeftArm().AddUniqueDynamic(this, &UCharaAnim::SetHasLeftArmShoot);*/
}

void UCharaAnim::BindRightArms()
{
	if (!chara->GetUsingRightArm())
	{
		return;
	}
	chara->GetUsingRightArm()->OnUseRightArm().AddUniqueDynamic(this, &UCharaAnim::UpdateRecoil);
	if (chara->HasNailGun())
		Cast<ANailGunActor>(chara->GetUsingRightArm())->OnHeavyRecoil().AddUniqueDynamic(this, &UCharaAnim::SetIsHeavy);
	if (chara->HasFireflies())
	{
		Cast<AFirefliesActor>(chara->GetUsingRightArm())->OnOverheat().AddUniqueDynamic(this, &UCharaAnim::SetIsOverHeat);
		Cast<AFirefliesActor>(chara->GetUsingRightArm())->OnPerfectCool().AddUniqueDynamic(this, &UCharaAnim::SetIsCool);
	}
}

void UCharaAnim::ServerRpc_SetSpeed_Implementation(FVector2D _speed)
{
	speed = _speed;
}

void UCharaAnim::SetSpeed(FVector2D _speed)
{
	if (chara->HasAuthority())
		speed = _speed;
	else
		ServerRpc_SetSpeed(_speed);
}

void UCharaAnim::UpdateMovement(const FVector2D& _speed, const FVector& _moveDir)
{
	speed = _speed;
	moveDir = _moveDir;
}

void UCharaAnim::Client_SetIsJumping_Implementation(bool _status)
{
	isJumping = _status;
}

void UCharaAnim::SetIsJumping(bool _status)
{
	if (chara->HasAuthority())
		isJumping = _status;
	else
		Client_SetIsJumping(_status);
}

void UCharaAnim::Client_SetIsScoping_Implementation(bool _status)
{
	isScoping = _status;
}

void UCharaAnim::SetIsScoping(bool _status)
{
	isScoping = _status;
}

void UCharaAnim::Client_SetIsHeavy_Implementation(bool _status)
{
	isHeavy = _status;
}

void UCharaAnim::SetIsHeavy(bool _status)
{
	if (chara->HasAuthority())
		isHeavy = _status;
	else
		Client_SetIsHeavy(_status);
}

void UCharaAnim::Client_SetIsCool_Implementation(bool _status)
{
	isCool = _status;
}

void UCharaAnim::SetIsCool(bool _status)
{
	if (chara->HasAuthority())
		isCool = _status;
	else
		Client_SetIsCool(_status);
}

void UCharaAnim::Client_SetIsOverHeat_Implementation(bool _status)
{
	isOverHeat = _status;
}

void UCharaAnim::SetIsOverHeat(bool _status)
{
	if (chara->HasAuthority())
		isOverHeat = _status;
	else
		Client_SetIsOverHeat(_status);
}

void UCharaAnim::Client_UpdateRecoil_Implementation(bool _status, int _numAnim)
{
	hasRightArmShoot = _status;
	numAnim = _numAnim;
}

void UCharaAnim::UpdateRecoil(bool _status,int _numAnim)
{
	LOG_COLOR(CHARA_ANIM_USE_LOG, "RECOIL", BLACK);
	if (chara->HasAuthority() && !hasRightArmShoot)
	{
		hasRightArmShoot = _status;
		numAnim = _numAnim;
	}
	else
		Client_UpdateRecoil(_status, _numAnim);
}

void UCharaAnim::Client_SetHasLeftArmShoot_Implementation(bool _status)
{
	hasLeftArmShoot = _status;
}

void UCharaAnim::SetHasLeftArmShoot(bool _status)
{
	if (chara->HasAuthority())
		hasLeftArmShoot = _status;
	else
		Client_SetHasLeftArmShoot(_status);
}

void UCharaAnim::Client_SetHasStartKick_Implementation(bool _status)
{
	hasStartKick = _status;
}

void UCharaAnim::SetHasStartKick(bool _status)
{
	if (chara->HasAuthority())
		hasStartKick = _status;
	else
		Client_SetHasStartKick(_status);
}

void UCharaAnim::Client_SetHasReachKick_Implementation(bool _status)
{
	hasReachKick = _status;
}

void UCharaAnim::SetHasReachKick(bool _status)
{
	if (chara->HasAuthority())
	hasReachKick = _status;
	else
		Client_SetHasReachKick(_status);
}

void UCharaAnim::Client_SetHasStartWCKick_Implementation(bool _status)
{
	hasStartWCKick = _status;
}

void UCharaAnim::SetHasStartWCKick(bool _status)
{
	if (chara->HasAuthority())
	
	hasStartWCKick = _status;
	else
		Client_SetHasStartWCKick(_status);
}

void UCharaAnim::SetHasStartSKKick(bool _status)
{
	hasStartSKKick = _status;
}

void UCharaAnim::SetHasSpawnBox(bool _status)
{
	hasSpawnBox = _status;
}

void UCharaAnim::Client_SetHasInteract_Implementation(bool _status)
{
	hasInteract = _status;
}

void UCharaAnim::SetHasInteract(bool _status)
{
	if (chara->HasAuthority())
		hasInteract = _status;
	else
		Client_SetHasInteract(_status);
}

void UCharaAnim::SetIsBumped(bool _status, const bool _isBumpByAI)
{
	isBumped = _status;
}

void UCharaAnim::SetHasPickUp(bool _status)
{
	hasPickUp = _status;
}

void UCharaAnim::SetHasCarry(bool _status)
{
	hasCarry = _status;
	if (!hasCarry)
		onStopCarrying.Broadcast();
}

void UCharaAnim::SetLeftArmReleased(bool _status)
{
	isScoping = _status ? false : isScoping;
	hasLeftArmRelease = _status;
}

void UCharaAnim::SetIsKicking(bool _status)
{
	isKicking = _status;
}

void UCharaAnim::SetIsUsingBrake(bool _status)
{

	isUsingBrake = _status;
	useBrake = true;
}

void UCharaAnim::SetIsUsingLifeStock(bool _status)
{
	isUsingLifeStock = _status;
}

void UCharaAnim::SetIsUsingEggStock(bool _status)
{
	isUsingEggStock = _status;
}

void UCharaAnim::SetHandcarSpeed(float _speed)
{
	handcarSpeed = _speed;
}

void UCharaAnim::SetIsOpenRA(bool _status)
{
	isOpenRA = _status;
}

void UCharaAnim::SetIsOpenLA(bool _status)
{
	isOpenLA = _status;
}

void UCharaAnim::SetIsOpenThigh(bool _status)
{
	isOpenThigh = _status;
}

void UCharaAnim::SetIsOpenCalf(bool _status)
{
	isOpenCalf = _status;
}

void UCharaAnim::SetCanStrafeTurn(bool _status)
{
	canStrafeTurn = _status;
}

void UCharaAnim::SetStrafeYaw(float _yaw)
{
	strafeYaw = _yaw;
}



void UCharaAnim::SetAnimWeight(float _weight, const bool _lerpAnimWeight)
{
	weightToken = weightToken + _weight < 0 ? 0 : weightToken + _weight;
	targetAnimWeight = weightToken > 0 ? 1.0f : 0.0f;
	lerpAnimWeight = targetAnimWeight == animWeight ? false : _lerpAnimWeight;

	animWeight = _lerpAnimWeight ? animWeight : targetAnimWeight;
	LOG_COLOR(false, "Anim Token => " + FString::FromInt(weightToken), BLUE);
}

void UCharaAnim::ClearAllAnimWeight(const bool _lerpAnimWeight)
{
	weightToken = 0;
	targetAnimWeight = 0.0f;
	lerpAnimWeight = targetAnimWeight == animWeight ? false : _lerpAnimWeight;
	animWeight = _lerpAnimWeight ? animWeight : 0.0f;
	LOG_COLOR(false, "Anim Token => " + FString::FromInt(weightToken), ORANGE);
}


float UCharaAnim::GetSpeed()
{
	if (FMath::Abs(speed.X) > 0)
		return speed.X;
	else if (FMath::Abs(speed.Y) > 0)
		return speed.Y;
	else
		return 0;
}

bool UCharaAnim::GetIsUsingBrake()
{
	return isUsingBrake;
}

bool UCharaAnim::GetIsKicking()
{
	return isKicking;
}

void UCharaAnim::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UCharaAnim, chara);
	DOREPLIFETIME(UCharaAnim, speed);
	DOREPLIFETIME(UCharaAnim, isJumping);
	DOREPLIFETIME(UCharaAnim, isScoping); 
	DOREPLIFETIME(UCharaAnim, hasRightArmShoot);
	DOREPLIFETIME(UCharaAnim, hasLeftArmShoot);
	DOREPLIFETIME(UCharaAnim, hasLeftArmRelease);
	DOREPLIFETIME(UCharaAnim, isHeavy);
	DOREPLIFETIME(UCharaAnim, isOverHeat);
	DOREPLIFETIME(UCharaAnim, isCool);
	DOREPLIFETIME(UCharaAnim, numAnim);
	DOREPLIFETIME(UCharaAnim, hasStartKick);
	DOREPLIFETIME(UCharaAnim, hasReachKick);
	DOREPLIFETIME(UCharaAnim, hasInteract);
	DOREPLIFETIME(UCharaAnim, hasCarry);
	DOREPLIFETIME(UCharaAnim, hasPickUp);
	/*DOREPLIFETIME(UCharaAnim, hasStartWCKick);*/
	DOREPLIFETIME(UCharaAnim, animWeight);
}