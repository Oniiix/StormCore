
#include "TutoManager.h"
#include "RobotHunter/AI/Arena/Arena.h"
#include "RobotHunter/AI/AI_Base.h"
#include "RobotHunter/AI/AI_Hunter/AI_Hunter.h"
#include "RobotHunter/CCC/Character/MainCharacter.h"
#include "RobotHunter/CCC/CustomPlayerController.h"
#include "RobotHunter/Network/OnlineManagerSubsystem.h"

ATutoManager::ATutoManager()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ATutoManager::BeginPlay()
{
	Super::BeginPlay();
	
	FTimerHandle _initTimer;
	GetWorld()->GetTimerManager().SetTimer(_initTimer, this, &ATutoManager::InitTutoManager, 0.3f);
}

void ATutoManager::InitTutoManager()
{

	//if (UOnlineManagerSubsystem* _gis = GetGameInstance()->GetSubsystem<UOnlineManagerSubsystem>())
	//	_gis->StartServer();

	if (GetWorld()->GetFirstPlayerController()->GetPawn()->IsA<AMainCharacter>())
	{
		mainCharacter = Cast<AMainCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
		mainCharacter->SetRightArm(rightArmRef);
		mainCharacter->SetLeftArm(leftArmRef);
		mainCharacter->SetCalf(calfRef);
		mainCharacter->SetThigh(thighRef);

		mainCharacter->DeactivateMembers({}, false);
		mainCharacter->SetCanJump(startWithJump);
		mainCharacter->GetLifeComponent()->SetStartLife(startLife);
		mainCharacter->SetScopeDisable(true);

		mainCharacter->GetLifeComponent()->OnDie().RemoveAll(mainCharacter);
		mainCharacter->GetLifeComponent()->OnDie().AddUniqueDynamic(this, &ATutoManager::CharaDown);
	}

	/// Check paralarvaArena availability
	if (!paralarvaArena)
	{
		LOG_ERROR(true, "paralarvaArena in " + ACTOR_NAME + " is null");
		return;
	}
	paralarvaArena->OnParalarvaDie().AddUniqueDynamic(this, &ATutoManager::CountDeadParalarva);
	paralarvaArena->CloseArena();

	///// Check trapper availability
	//if (!trapper)
	//{
	//	LOG_ERROR(true, "trapper in " + ACTOR_NAME + " is null");
	//	return;
	//}
	////trapper->OnSet().AddUniqueDynamic(this, &ATutoManager::SetTrapper);
	//SetTrapper();

	countDeadParalarva = 0;
	isInitializationGood = true;
}

void ATutoManager::ClearParalarvaArena()
{
	paralarvaArena->OnParalarvaDie().RemoveAll(this);
	paralarvaArena->ClearArena(false);
}

//void ATutoManager::SetTrapper()
//{
//	float _trapperLife = trapper->GetLifeComponent()->GetMaxLife();
//	LOG_COLOR(true, "SetLife in Tuto", PINK);
//	//trapper->GetLifeComponent()->SetLife(_trapperLife * trapperLifePercent, 0, nullptr);
//	trapper->GetLifeComponent()->SetStartLife(_trapperLife * trapperLifePercent);
//}

void ATutoManager::CountDeadParalarva()
{
	countDeadParalarva++;
	if (countDeadParalarva >= paralarvaToKill)
		ClearParalarvaArena();
}



void ATutoManager::CharaDown()
{
	mainCharacter->GetLifeComponent()->SetLife(1.0f, 0.0f, nullptr);
}