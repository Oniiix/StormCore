// Fill out your copyright notice in the Description page of Project Settings.


#include "WindWeather.h"

AWindWeather::AWindWeather()
{
}

void AWindWeather::BeginPlay()
{
	Super::BeginPlay();
	direction = SelectDirection();
}

void AWindWeather::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

FVector AWindWeather::SelectDirection()
{
	float _x = FMath::RandRange(-1, 1);
	float _y = FMath::RandRange(-1, 1);
	return FVector(_x,_y,0);
}
