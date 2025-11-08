// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FileLogger.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Components/CustomExponentialHeightFogComponent.h"
#include "GameFramework/SpringArmcomponent.h"
#include "CustomBlueprintLibrary.generated.h"


USTRUCT(BlueprintType)
struct FBlueprintSearchResult
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UBlueprint> owner = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UActorComponent> component = nullptr;

	FBlueprintSearchResult(const TObjectPtr<UBlueprint>& owner, const TObjectPtr<UActorComponent>& component)
		: owner(owner), component(component)
	{
	}

	FBlueprintSearchResult()
	{
		owner = nullptr;
		component = nullptr;
	}

public:
	TObjectPtr<UBlueprint> GetBlueprintOwner() const { return owner; }
	TObjectPtr<UActorComponent> GetBlueprintComponent() const { return component; }
};

UCLASS()
class ROBOTHUNTER_API UCustomBlueprintLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	static void CustomAddComponent(AActor* _owner, TSubclassOf<UActorComponent> _componentClass);

	UFUNCTION(BlueprintCallable)
	static bool IsGreaterString(FString _a, FString _b);

	UFUNCTION(BlueprintCallable)
	static bool IsLessString(FString _a, FString _b);


	//TODO Rename to => CreateFogData
	UFUNCTION(BlueprintPure)
	static FExponentialHeightFogData SetCustomFogSecondFog(const float _fogDensity, const float _fogHeightFalloff,
	                                                       const float _fogHeightOffset);


	UFUNCTION(BlueprintCallable)
	static void GetKeyAndValueReturn(TMap<FString, FString> _map, TArray<FString>& _keys, TArray<FString>& _value);
#if WITH_EDITOR
	UFUNCTION(BlueprintCallable)
	static bool SaveDataAsset(UDataAsset* _dataAsset);
	UFUNCTION(BlueprintCallable)
	static TArray<FBlueprintSearchResult> GetAllBlueprintsWithComponent(
		const FName& _path, const TSubclassOf<UActorComponent>& _componentClass);
	UFUNCTION(BlueprintCallable)
	static bool OpenAssetWindow(UObject* _asset, FString& _outMessage);
	
#endif
	/**
	 * Write a log in custom file log. (Folder CustomLog)
	 * @param _severity Severity of log
	 * @param _category Category of log
	 * @param _msg msg in log
	 * @param _printInConsole Print in console of engine
	 */
	UFUNCTION(BlueprintCallable, Category = "Custom|Log")
	static void WriteCustomLogFile(ELogSeverity _severity, ELogCategory _category, const FString& _msg,
	                               bool _printInConsole);


	UFUNCTION(BlueprintPure)
	static float GetAimYaw(const FRotator& _meshRotation, const FRotator& _armRotation);

	UFUNCTION(BlueprintCallable)
	static void AttachSpringArm(USpringArmComponent* _springArm, USceneComponent* _parentComponent,
	                            const FName _socketName = "None", const bool _inheritRotation = false);
};
