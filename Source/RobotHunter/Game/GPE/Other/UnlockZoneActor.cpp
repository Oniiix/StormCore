#include "UnlockZoneActor.h"
#include "RobotHunter/Utils/DebugUtils.h"
#include <RobotHunter/SaveSystem/SaveManagerSubsystem.h>
#include "RobotHunter/SaveSystem/GameplaySave.h"

AUnlockZoneActor::AUnlockZoneActor()
{
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>("Root");

	hitBox = CreateDefaultSubobject<UBoxComponent>("HitBox");
	hitBox->SetupAttachment(RootComponent);
	hitBox->SetCollisionProfileName(FName("OverlapOnlyPlayer"), false);
}

void AUnlockZoneActor::BeginPlay()
{
	Super::BeginPlay();
	UGameplaySave* _save = GetGameInstance()->GetSubsystem<USaveManagerSubsystem>()->GetCurrentSave();
	if (needTuto && _save && !_save->GetTutoSave().hasStartTuto)
	{
		Destroy();
		return;
	}

	CheckUnlockFunction(unlockList);
	hitBox->OnComponentBeginOverlap.AddUniqueDynamic(this, &AUnlockZoneActor::OnHitBox);
}

void AUnlockZoneActor::OnHitBox(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	AMainCharacter* _chara = nullptr;
	if ((_chara = Cast<AMainCharacter>(OtherActor)) == nullptr) return;

	for (TFunction<void(const TObjectPtr<AMainCharacter>&)> _function : functionList)
	{
		if (_function == nullptr) continue;
		_function(_chara);
	}

	hitBox->SetCollisionProfileName(FName("IgnoreAll"), false);
	SetLifeSpan(2.0f);

	CHECK_NULL(tutorialAsset, "tutorial Asset is Null!!");

	LOG_COLOR(true, "Unlock zone", BLUE);
	tutorialAsset->LaunchTutorial(_chara);

}


//[!!!] NE PAS OUVRIR CETTE FONCTION SOUS PEINE DE DEVENIR AVEUGLE [!!!]
void AUnlockZoneActor::CheckUnlockFunction(const TArray<EUnlockType>& _unlockList)
{
	functionList.Empty();
	for (EUnlockType _type : _unlockList)
	{
		switch (_type)
		{
		case EUnlockType::Unlock_MovementMember:
		{
			functionList.Add({
				[this](const TObjectPtr<AMainCharacter>& _chara)
				{
					LOG_COLOR(UNLOCK_ZONE_USE_LOG, "Unlock MOvement", BLUE);
					_chara->ReactivateMembers({ _chara->GetUsingCalf(), _chara->GetUsingLeftArm(), _chara->GetUsingRightArm() });
				} });
			break;
		}
		case EUnlockType::Unlock_Kick:
		{
			functionList.Add({
				[this](const TObjectPtr<AMainCharacter>& _chara)
				{
					LOG_COLOR(UNLOCK_ZONE_USE_LOG, "Unlock Kick", ORANGE);

					_chara->ReactivateMembers({ _chara->GetUsingThigh(), _chara->GetUsingLeftArm(), _chara->GetUsingRightArm() });
				} });
			break;
		}
		case EUnlockType::Unlock_PrimaryWeapon:
		{
			functionList.Add({
				[this](const TObjectPtr<AMainCharacter>& _chara)
				{
					LOG_COLOR(UNLOCK_ZONE_USE_LOG, "Unlock Primary", CYAN);

					_chara->ReactivateMembers({ _chara->GetUsingThigh(), _chara->GetUsingLeftArm(), _chara->GetUsingCalf() });
				} });
			break;
		}
		case EUnlockType::Unlock_SecondaryWeapon:
		{
			functionList.Add({
				[this](const TObjectPtr<AMainCharacter>& _chara)
				{
					LOG_COLOR(UNLOCK_ZONE_USE_LOG, "Unlock Secondary", YELLOW);
					_chara->ReactivateMembers({ _chara->GetUsingThigh(), _chara->GetUsingRightArm(), _chara->GetUsingCalf() });
				} });
			break;
		}
		case EUnlockType::Unlock_Jump:
		{
			functionList.Add({
				[this](const TObjectPtr<AMainCharacter>& _chara)
				{
					LOG_COLOR(UNLOCK_ZONE_USE_LOG, "Unlock Jump", PINK);
					_chara->SetCanJump(true);
				} });
			break;
		}
		case EUnlockType::Unlock_Scope:
		{
			functionList.Add({
				[this](const TObjectPtr<AMainCharacter>& _chara)
				{
					LOG_COLOR(UNLOCK_ZONE_USE_LOG, "Unlock Scope", RED);
					_chara->SetScopeDisable(false);
				} });
			break;
		}
		}
	}
}
