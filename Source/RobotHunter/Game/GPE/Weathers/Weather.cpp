// Fill out your copyright notice in the Description page of Project Settings.


#include "Weather.h"
#include <NiagaraFunctionLibrary.h>
#include "NiagaraComponent.h"
#include <Kismet/GameplayStatics.h>
#include "RobotHunter/Game/GameMode/PrincipalGameMode.h"
#include "RobotHunter/Game/CustomGameState.h"
#include "RobotHunter/Network/OnlineManagerSubsystem.h"
#include "RobotHunter/Network/Tree/TreeManagerSubSystem.h"
#include <RobotHunter/Utils/DebugUtils.h>

AWeather::AWeather()
{
	PrimaryActorTick.bCanEverTick = false;
	niagaraSceneComp = CreateDefaultSubobject<USceneComponent>("Niagara");
	stats.earthNiagaraSceneComp = CreateDefaultSubobject<USceneComponent>("EarthNiagara");
	stats.windNiagaraSceneComp = CreateDefaultSubobject<USceneComponent>("WindNiagara");
	RootComponent = niagaraSceneComp;
	stats.earthNiagaraSceneComp->SetupAttachment(RootComponent);
	stats.windNiagaraSceneComp->SetupAttachment(RootComponent);
	index = 0;
}

void AWeather::BeginPlay()
{
	Super::BeginPlay();
	LOG_COLOR_TIME(WEATHER_USE_LOG, name, YELLOW, 10);
	onSubLevelLoad.AddDynamic(this, &AWeather::LoadAllLevels);
	CHECK_NULL(GetWorld(), "World is null!!");
	CHECK_NULL(GetWorld()->GetGameState<ACustomGameState>(), "gamestate is null!!");
	//if (!GetWorld()->GetAuthGameMode<ACustomGameMode>()->GetIsInGame()) return;
	LoadAllLevels();
	InitWorld();
}

void AWeather::InitWorld()
{
	//InitEarthQuake();
	InitMeteo();
	//InitWind();
}

void AWeather::InitMeteo()
{
	ACustomGameState* _gs = Cast<ACustomGameState>(GetWorld()->GetGameState());
	SpawnNiagara(niagara, niagaraSceneComp, niagaraComp);
	/*if (hasEarthQuake)
		SpawnNiagara(stats.earthNiagara, stats.earthNiagaraSceneComp, stats.earthNiagaraComp);*/
	if (!_gs)
	{
		LOG_COLOR_TIME(WEATHER_USE_LOG, "GAMEMODE = NULL", ORANGE, 2);
		return;
	}
	_gs->SetCurrentMeteo(this);
}

void AWeather::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//CHECK_NULL(GetWorld()->GetAuthGameMode(), "gamemode is null!!");
	DrawDebugSphere(GetWorld(), GetActorLocation(), 10, 32, FColor::Red);
}

void AWeather::LoadAllLevels()
{
	if (levelsToLoad.Num() < 1)
		LoadTree();
	else
		LoadLevel(levelsToLoad[index]);
	//LOG_COLOR_TIME(WEATHER_USE_LOG, "LOAD LEVELS " + levelsToLoad[index].GetAssetName(), ORANGE, 10)
}

void AWeather::LoadTree()
{
	TObjectPtr<ATreeCell> _tree = GetWorld()->GetSubsystem<UTreeManagerSubSystem>()->GetMainTree();
	if (!_tree)return;
	LOG_COLOR(true, "UPdate Octree", CYAN);
	_tree->UpdateTree();
}

void AWeather::LoadLevel(TSoftObjectPtr<UWorld> _level)
{
	int _uniqueID = FMath::RandRange(10000,90000);
	FLatentActionInfo LatentInfo(0, _uniqueID, TEXT("LoadLevelCallback"), this);
	UGameplayStatics::LoadStreamLevelBySoftObjectPtr(this, _level, true, false, LatentInfo);
}

void AWeather::UnloadLevel(TSoftObjectPtr<UWorld> _level)
{
	FLatentActionInfo LatentInfo;
	UGameplayStatics::UnloadStreamLevelBySoftObjectPtr(this, _level, LatentInfo, true);
}

void AWeather::LoadLevelCallback()
{
	LOG_COLOR(true, "Load Level callback", GREEN);
	if (index == levelsToLoad.Num() - 1)
	{
		LoadTree();
		return;
	}
	//LOG_COLOR_TIME(WEATHER_USE_LOG, "CALLBACK", ORANGE, 10)
	index++;
	onSubLevelLoad.Broadcast();

}

void AWeather::InitEarthQuake()
{
	int _result = FMath::RandRange(0, 100);
	if (_result <= stats.earthQuakePercent)
		hasEarthQuake = true;
	else
		hasEarthQuake = false;
	float _intensity = FMath::FRandRange(stats.minEarthQuakeIntensity, stats.maxEarthQuakeIntensity);
	onEarthQuake.Broadcast(_intensity);
}

void AWeather::InitWind()
{
	SpawnNiagara(stats.windNiagara, stats.windNiagaraSceneComp, stats.windNiagaraComp);
	UpdateWind();
}

void AWeather::UpdateWind()
{
	float _result = FMath::FRandRange(stats.minWindIntensity, stats.maxWindIntensity);
	onWindChanged.Broadcast(SelectDirection(), _result);

}

FVector AWeather::SelectDirection()
{
	float _x = FMath::RandRange(-1, 1);
	float _y = FMath::RandRange(-1, 1);
	return FVector(_x, _y, 0);
}

void AWeather::SpawnNiagara(TObjectPtr<UNiagaraSystem> _niagaraSystem, USceneComponent* _sceneComponent, UNiagaraComponent* _niagaraComponent)
{
	if (!_niagaraSystem)
	{
		LOG_ERROR(true, "PAS DE NIAGARA SYSTEM");
		return;
	}
	niagaraComp = UNiagaraFunctionLibrary::SpawnSystemAttached(_niagaraSystem, _sceneComponent, NAME_None, FVector(0.f, 0.f, 400.f), FRotator(0.f), EAttachLocation::Type::KeepRelativeOffset, true);
}
