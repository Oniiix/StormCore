
#include "AIDebugInfoWidgetComponent.h"
#include "RobotHunter/AI/AI_Base.h"
#include "RobotHunter/CCC/Character/MainCharacter.h"
#include "RobotHunter/Utils/DebugUtils.h"
#include "RobotHunter/UI/AI/HitDamage.h"
#include "Net/UnrealNetwork.h"
#if WITH_EDITORONLY_DATA
#include "CustomEditor/Public/CustomEditorSettings.h"
#endif

void UAIDebugInfoWidgetComponent::InitAIDebugInfo()
{
	owner = Cast<AAI_Base>(GetOwner());
	if (!owner)
	{
		LOG_ERROR(AI_DEBUG_WIDGET_USE_LOG, "Failed to cast Owner in AAI_Base on AIDebugInfoWidget of " + OWNER_NAME);
		return;
	}
	owner->GetLifeComponent()->OnLifeChangeByDamage().AddUniqueDynamic(this, &UAIDebugInfoWidgetComponent::AIDamage);
	AIDebugInfoWidget = Cast<UAIDebugInfoWidget>(GetWidget());
	if (!AIDebugInfoWidget)
	{
		LOG_ERROR(AI_DEBUG_WIDGET_USE_LOG, "Failed to cast Widget in AIDebugInfoWidget of " + OWNER_NAME);
		return;
	}
	AIDebugInfoWidget->InitWidget();
	mainCharacter = Cast<AMainCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	if (!mainCharacter)
	{
		LOG_ERROR(AI_DEBUG_WIDGET_USE_LOG, "Failed to get MainCharacter on AIDebugInfoWidgetComponent of " + OWNER_NAME);
		return;
	}


#if WITH_EDITORONLY_DATA
	const bool _useDebug = GetMutableDefault<UCustomEditorSettings>()->GetAIState();
	if (_useDebug)
	{
		GetMutableDefault<UCustomEditorSettings>()->OnDebugAIState().AddUniqueDynamic(this, &UAIDebugInfoWidgetComponent::SetStateVisibility);
		GetMutableDefault<UCustomEditorSettings>()->OnDebugAITarget().AddUniqueDynamic(this, &UAIDebugInfoWidgetComponent::SetTargetVisibility);
		GetMutableDefault<UCustomEditorSettings>()->OnDebugAIThreat().AddUniqueDynamic(this, &UAIDebugInfoWidgetComponent::SetThreatVisibility);
		GetMutableDefault<UCustomEditorSettings>()->OnDebugAIDamage().AddUniqueDynamic(this, &UAIDebugInfoWidgetComponent::SetHitDamageVisibility);

		SetStateVisibility(GetMutableDefault<UCustomEditorSettings>()->GetAIState());
		SetTargetVisibility(GetMutableDefault<UCustomEditorSettings>()->GetAITarget());
		SetThreatVisibility(GetMutableDefault<UCustomEditorSettings>()->GetThreatValue());
		SetHitDamageVisibility(GetMutableDefault<UCustomEditorSettings>()->GetAIDamage());
	}
	else
	{
		//UKismetSystemLibrary::PrintString(this, "DESTROY DEBUG", true, true, RED, 8.f);
		DestroyComponent();
	}
#else
	DestroyComponent();
	//SetStateVisibility(false);
	//SetTargetVisibility(false);
	//SetThreatVisibility(false);
	//SetHitDamageVisibility(false);
#endif

	isInitializationGood = true;
}

void UAIDebugInfoWidgetComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!isInitializationGood)
		return;

	cameraLocation = mainCharacter->GetCameraComponent()->GetCamera()->GetComponentLocation();
	location = GetComponentLocation();
	const FRotator& _newRot = UKismetMathLibrary::FindLookAtRotation(location, cameraLocation);
	SetWorldRotation(_newRot);

	if (!AIDebugInfoWidget) // Use to reassign widget value client-side (idk erase after initialize)
		AIDebugInfoWidget = Cast<UAIDebugInfoWidget>(GetWidget());
}

void UAIDebugInfoWidgetComponent::SetStateVisibility(const bool _value)
{
	AIDebugInfoWidget->SetStateVisibility(_value);
}

void UAIDebugInfoWidgetComponent::SetTargetVisibility(const bool _value)
{
	AIDebugInfoWidget->SetTargetVisibility(_value);
}

void UAIDebugInfoWidgetComponent::SetThreatVisibility(const bool _value)
{
	AIDebugInfoWidget->SetThreatVisibility(_value);
}

void UAIDebugInfoWidgetComponent::SetWidgetVisibility(const bool _value)
{
	SetComponentTickEnabled(_value);
	AIDebugInfoWidget->SetVisibility(_value ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}

void UAIDebugInfoWidgetComponent::AIDamage(AActor* _owner, const int _currentLife, const int _damage, AActor* _damageSource)
{
	if (!hitDamageVisibility)
		return;

	const FVector& _cameraRight = mainCharacter->GetCameraComponent()->GetCamera()->GetRightVector();
	const FVector& _damageInfoLocation = owner->GetActorLocation() + (_cameraRight * 50);
	const FRotator& _newRot = UKismetMathLibrary::FindLookAtRotation(location, _damageInfoLocation);

	AHitDamage* _newHitDamageWidget = GetWorld()->SpawnActor<AHitDamage>(hitDamage, _damageInfoLocation, _newRot);
	_newHitDamageWidget->InitHitDamage(_damage);
}

void UAIDebugInfoWidgetComponent::SetStateInfo(const FText& _info, const FColor& _color)
{
	if (!isInitializationGood)
		return;

	stateInfo = _info;
	stateColor = _color;
	AIDebugInfoWidget->SetStateInfo(stateInfo, stateColor);
}

void UAIDebugInfoWidgetComponent::SetTargetInfo(const FString& _info)
{
	if (!isInitializationGood)
		return;

	targetInfo = _info;
	AIDebugInfoWidget->SetTargetInfo(targetInfo);
}

void UAIDebugInfoWidgetComponent::SetThreatInfo(const TArray<FString>& _infos)
{
	if (!isInitializationGood)
		return;

	FString _info;
	const int _infosCount = _infos.Num();
	for (int i = 0; i < _infosCount; i++)
	{
		_info += _infos[i] + " P" + FString::FromInt(i + 1);
		if (i + 1 != _infosCount)
			_info += " - ";
	}

	threatsInfo = _info;
	AIDebugInfoWidget->SetThreatInfo(threatsInfo);
}

void UAIDebugInfoWidgetComponent::OnRep_StateInfo()
{
	if (AIDebugInfoWidget)
		AIDebugInfoWidget->SetStateInfo(stateInfo, stateColor);
}

void UAIDebugInfoWidgetComponent::OnRep_TargetInfo()
{
	if (AIDebugInfoWidget)
		AIDebugInfoWidget->SetTargetInfo(targetInfo);
}

void UAIDebugInfoWidgetComponent::OnRep_ThreatInfo()
{
	if (AIDebugInfoWidget)
		AIDebugInfoWidget->SetThreatInfo(threatsInfo);
}

void UAIDebugInfoWidgetComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UAIDebugInfoWidgetComponent, AIDebugInfoWidget);
	DOREPLIFETIME(UAIDebugInfoWidgetComponent, stateInfo);
	DOREPLIFETIME(UAIDebugInfoWidgetComponent, stateColor);
	DOREPLIFETIME(UAIDebugInfoWidgetComponent, targetInfo);
	DOREPLIFETIME(UAIDebugInfoWidgetComponent, threatsInfo);
	DOREPLIFETIME(UAIDebugInfoWidgetComponent, hitDamage);
}
