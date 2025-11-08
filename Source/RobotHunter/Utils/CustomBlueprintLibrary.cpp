// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomBlueprintLibrary.h"

#include "DebugUtils.h"
#include "FileLogger.h"
#include "RobotHunter/Game/TEST/DummyButton.h"
#include "RobotHunter/Game/GPE/Components/MovableComponent/MovableComponent.h"
#include "UObject/UnrealTypePrivate.h"
#if WITH_EDITOR
#include "FileHelpers.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "Kismet2/KismetEditorUtilities.h"
#include "Engine/SCS_Node.h"
#endif


void UCustomBlueprintLibrary::CustomAddComponent(AActor* _owner, TSubclassOf<UActorComponent> _componentClass)
{
	TArray<UActorComponent*> _comps; 
	_owner->GetComponents(_componentClass, _comps, true);
	FString _componentName = _comps.Num() > 0 ? _componentClass.Get()->GetName() + FString::FromInt(_comps.Num() + 1) : _componentClass.Get()->GetName();

	if (Cast<USceneComponent>(_componentClass.Get()->GetDefaultObject()))
	{
		USceneComponent* _comp = NewObject<USceneComponent>(_owner, _componentClass.Get(), FName(_componentName));
		_comp->SetupAttachment(_owner->GetRootComponent());
		_owner->AddInstanceComponent(_comp);
		_owner->AddOwnedComponent(_comp);
		_comp->RegisterComponent();
	}
	else
	{
		UActorComponent* _comp = NewObject<UActorComponent>(_owner, _componentClass.Get(), FName(_componentName));
		_owner->AddInstanceComponent(_comp);
		_owner->AddOwnedComponent(_comp);
		_comp->RegisterComponent();
	}
	_owner->Modify(true);
}

bool UCustomBlueprintLibrary::IsGreaterString( FString _a,  FString _b)
{
	return _a > _b;
}

bool UCustomBlueprintLibrary::IsLessString(FString _a,  FString _b)
{
	return _a < _b;
}

FExponentialHeightFogData UCustomBlueprintLibrary::SetCustomFogSecondFog(const float _fogDensity, const float _fogHeightFalloff, const float _fogHeightOffset)
{
	FExponentialHeightFogData _fogdata = FExponentialHeightFogData();
	_fogdata.FogDensity = _fogDensity;
	_fogdata.FogHeightFalloff = _fogHeightFalloff;
	_fogdata.FogHeightOffset = _fogHeightOffset;
	return _fogdata;
}

void UCustomBlueprintLibrary::GetKeyAndValueReturn(TMap<FString, FString> _map, TArray<FString>& _keys,
	TArray<FString>& _value)
{
	_map.GenerateKeyArray(_keys);
	_map.GenerateValueArray(_value);
}

#if WITH_EDITOR
bool UCustomBlueprintLibrary::SaveDataAsset(UDataAsset* _dataAsset)
{
	
	if (!_dataAsset)
		return false;
	_dataAsset->Modify(true);

	UPackage* _package = _dataAsset->GetPackage();
	if (!_package)
		return false;

	return UEditorLoadingAndSavingUtils::SavePackages({ _package }, false);
}
TArray<FBlueprintSearchResult> UCustomBlueprintLibrary::GetAllBlueprintsWithComponent(const FName& _path, const TSubclassOf<UActorComponent>& _componentClass)
{
	TArray<FBlueprintSearchResult> _results;

	FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");

	TArray<FAssetData> _assets;
	AssetRegistryModule.Get().GetAssetsByPath(_path, _assets, true, false);

	const int _assetsCount = _assets.Num();
	for (int i = 0; i < _assetsCount; i++)
	{
		if (_assets[i].IsValid())
		{
			if (UBlueprint* _blueprint = Cast<UBlueprint>(_assets[i].GetAsset()))
			{
				if (_blueprint->SimpleConstructionScript)
				{
					TArray<USCS_Node*> _nodes = _blueprint->SimpleConstructionScript->GetAllNodes();
					const int _nodesCount = _nodes.Num();
					for (int j = 0; j < _nodesCount; j++)
					{
						if (_nodes[j]->ComponentTemplate->GetClass()->GetName() == _componentClass.Get()->GetName())
							_results.Add(FBlueprintSearchResult(_blueprint, _nodes[j]->ComponentTemplate));
					}
				}
			}
		}
	}
	return _results;
}

bool UCustomBlueprintLibrary::OpenAssetWindow(UObject* _asset, FString& _outMessage)
{
	if (!_asset)
	{
		_outMessage = "Open Window Failed - Asset is null";
		return false;
	}

	UAssetEditorSubsystem* _editorSubsystem = GEditor ? GEditor->GetEditorSubsystem<UAssetEditorSubsystem>() : nullptr;
	if (!_editorSubsystem)
	{
		_outMessage = "Open Window Failed - Can't get Asset Editor Subsystem";
		return false;
	}

	const bool _success = _editorSubsystem->OpenEditorForAsset(_asset);
	_outMessage = _success ? "Open Window Success !" : "Open Window Failed - OpenEditorAsset error";

	return _success;
}
#endif

void UCustomBlueprintLibrary::WriteCustomLogFile(const ELogSeverity _severity, const ELogCategory _category, const FString& _msg, bool _printInConsole)
{
	UFileLogger::WriteLog(_severity, _category, nullptr, _msg);
	if (_printInConsole)
		GLog->Log(_msg);		
}

float UCustomBlueprintLibrary::GetAimYaw(const FRotator& _meshRotation, const FRotator& _armRotation)
{
	float _result = (_armRotation.Yaw < 0 ? (360 + _armRotation.Yaw) : _armRotation.Yaw) - (_meshRotation.Yaw < 0 ? (360 + _meshRotation.Yaw) : _meshRotation.Yaw);
	if (FMath::Abs(_result) > 180)
		_result = _result < 0 ? (_result + 360) : (_result - 360);
	return _result;
}

void UCustomBlueprintLibrary::AttachSpringArm(USpringArmComponent* _springArm, USceneComponent* _parentComponent, const FName _socketName, const bool _inheritRotation)
{
	_springArm->bInheritPitch = _inheritRotation;
	_springArm->bInheritYaw = _inheritRotation;
	_springArm->bInheritRoll = _inheritRotation;

	_springArm->AttachToComponent(_parentComponent, FAttachmentTransformRules::KeepRelativeTransform, _socketName);
}

//TEST Get Variable of a class
/*
UObject* object = _nodes[j];
							static FName FlagPropertyName = TEXT("Flag");
							if (UClass* cl = object->GetClass())
							{

								if (FProperty* Property = cl->FindPropertyByName(FlagPropertyName))
								{
									if (FBoolProperty* BoolProperty = Cast<FBoolProperty>(Property))
									{
										BoolProperty->SetPropertyValue(Property->ContainerPtrToValuePtr<float>(object), true);
									}
								}
							}*/