// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

UENUM(Blueprintable)
enum ESpeedMode
{
	WALK = 0,
	RUN = 1,
	SHOOT = 2,
	SPRINT = 3,
	MINIMUM = 4,
	NOSPEED = 5
};

UENUM()
enum ESpeedModifiers
{
	NOMODIFIERS = 100,
	MUD = 70,
	COLD = 85,
	EARTH = 50
};

UENUM()
enum EWeightState
{
	NOWEIGHT = 100,
	TIER1 = 75,
	TIER2 = 50,
	TIER3 = 25,
	TIER4 = 0
};