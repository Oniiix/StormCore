// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

UENUM(BlueprintType)
enum EAIType
{
	Paralarva,
	Titan,
	Trapper,
	Torpedo,
	Pennant,

	Base
};

UENUM()
enum ERangeAttack
{
	Short,
	Mid,
	Long,
	RangeAttack_Count
};

UENUM()
enum EAIAnimation
{
	Spawn,
	BattleCry,
	Hit,
	Flung_Cycle,
	Flung_GetUp,
	Stun,
	PL_CQC,
	PL_Leap,
	PL_Leap_Attack,
	PL_Nap,
	TRP_CQC,
	TRP_Acid_Shot,
	TRP_Direct_Shot,
	TT_CQC,
	TT_Shield,
	TT_Charge,

	Null
};
