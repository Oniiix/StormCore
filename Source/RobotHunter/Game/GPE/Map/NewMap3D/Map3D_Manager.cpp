#include "Map3D_Manager.h"

#include "Components/SceneCaptureComponent2D.h"
#include "GameFramework/HUD.h"
#include "RobotHunter/Game/GPE/Handcar/CustomHandcarActor.h"
#include "RobotHunter/UI/HUD/MainHUD.h"
#include "RobotHunter/Utils/DebugUtils.h"

// Sets default values for this component's properties
UMap3D_Manager::UMap3D_Manager(): map3DPawnInstance(nullptr)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
}

// Called when the game starts
void UMap3D_Manager::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->GetTimerManager().SetTimer(
		HandleTimer,
		this,
		&UMap3D_Manager::InitiatePauseMenu,
		1.0f,
		false
	);
	const APlayerController* _pc = GetWorld()->GetFirstPlayerController();
	mainCharacter = Cast<AMainCharacter>(_pc->GetPawn());
	
}


// Called every frame
void UMap3D_Manager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UMap3D_Manager::InitMapVariable()
{
	if (mapActorInstance && map3DPawnInstance)
		return;
	const FString _currentLevelName = UGameplayStatics::GetCurrentLevelName(GetWorld());
	const int _numLevel = levelToMapIndex.Num();
	for (int i = 0; i < _numLevel; i++)
	{
		if (levelToMapIndex[i].GetAssetName() == _currentLevelName)
		{
			currentIndexMap = i;
		}
	}
	const int _numActor = registeredActors.Num();
	for (int i = 0; i < _numActor; i++)
	{
		if (const TObjectPtr<ACustomHandcarActor> _handcar = Cast<ACustomHandcarActor>(registeredActors[i]))
		{
			handcar = _handcar;
		}
	}
}

void UMap3D_Manager::InitiatePauseMenu()
{
	pauseMenu = Cast<AMainHUD>(GetWorld()->GetFirstPlayerController()->GetHUD())->GetUI<
		UPauseMenu>(EStateUI::MainPause);

	//TODO a changer MATHIEU
	pauseMenu->GetOnShowMap().AddDynamic(this, &UMap3D_Manager::OnShowMap);
	pauseMenu->GetOnHideMap().AddDynamic(this, &UMap3D_Manager::OnHideMap);


	mainCharacter->GetOnEnterInCombat().AddDynamic(this, &UMap3D_Manager::OnHideMap);
}

void UMap3D_Manager::OnShowMap()
{
	LOG_COLOR_TIME(true, "[Map3D_Manager] SHOW_MAP !!!!", YELLOW, 20);
	if (mainCharacter->GetInCombat())
		return;
	InitMapVariable();
	SpawnMiniMap();
	SetMapView();
	map3DPawnInstance->EnablePlayerContext();
}

void UMap3D_Manager::OnHideMap()
{

	if (mapActorInstance && map3DPawnInstance)
	{
		LOG_COLOR_TIME(true, "[Map3D_Manager] HIDE_MAP !!!!", YELLOW, 20);
		mapActorInstance->GetMapMesh()->SetVisibility(false, true);
		APlayerController* _pc = GetWorld()->GetFirstPlayerController();
		_pc->Possess(mainCharacter);
		mainCharacter->EnablePlayerContext(mainCharacter->GetCurrentPlayerContext());
		mapSpawn = false;
		if (handcar)
		{
			handcar->GetNitroActor()->SetInputsSetup(false);
			handcar->GetNitroActor()->SetupPlayerInputs(mainCharacter);
		}
		map3DPawnInstance->SetActorTickEnabled(false);
		map3DPawnInstance->GetMapCapture()->bCaptureEveryFrame = false;
		mapActorInstance->SetActorTickEnabled(false);
	}
}

void UMap3D_Manager::SpawnMiniMap()
{
	UWorld* _world = GetWorld();
	if (_world && !mapActorInstance && !map3DPawnInstance)
	{
		mapActorInstance = _world->SpawnActor<AMap3D>(map3DActor[currentIndexMap], FVector(0), FRotator(0));
		map3DPawnInstance = _world->SpawnActor<AMapPawn>(map3DPawn, *mapActorInstance->GetTravelPoint().Find(startSocket),FRotator(0));
		if (bStartInMainCharacter)
		{
			startSocket = "MainCharacter";
			mapActorInstance->AddTravelPoint("MainCharacter", mainCharacter->GetActorLocation() / 100);
		}
		mapActorInstance->SetMainCharacter(mainCharacter);
		mapActorInstance->SetActorMarker(registeredActors);
		mapActorInstance->InitActorMarkers();
		mapActorInstance->SetPauseMenu(pauseMenu);
		map3DPawnInstance->SetSocket(mapActorInstance->GetTravelPoint());
		map3DPawnInstance->SetCurrentSocketName(startSocket);
		map3DPawnInstance->AddShowOnlyActor(mapActorInstance);
		map3DPawnInstance->AddShowOnlyActor(map3DPawnInstance);
		map3DPawnInstance->SetSpringArmLength(mapActorInstance->GetSpringArmLength());
		mapSpawn = true;
	}
}

void UMap3D_Manager::SetMapView()
{
	if (!mapActorInstance || !map3DPawnInstance)
		return;
	APlayerController* _pc = GetWorld()->GetFirstPlayerController();
	map3DPawnInstance->SetMainCharacter(mainCharacter);
	_pc->Possess(map3DPawnInstance);
	mapActorInstance->GetMapMesh()->SetVisibility(true,true);
	map3DPawnInstance->SetActorTickEnabled(true);
	map3DPawnInstance->GetMapCapture()->bCaptureEveryFrame = true;
	mapActorInstance->SetActorTickEnabled(true);
}

void UMap3D_Manager::RegisterActor(AActor* _actorToRegister)
{
	registeredActors.Add(_actorToRegister);
}

void UMap3D_Manager::UnregisterActor(AActor* _actorToRemove)
{
	registeredActors.Remove(_actorToRemove);
}
