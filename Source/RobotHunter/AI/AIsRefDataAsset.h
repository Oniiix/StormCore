// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "RobotHunter/AI/AI_Base.h"
#include "RobotHunter/AI/AI_Protector/AI_Protector.h"
#include "RobotHunter/AI/AI_Scout/AI_Scout.h"
#include "RobotHunter/AI/AI_Hunter/AI_Hunter.h"
#include "RobotHunter/AI/AI_Torpedo/AI_Torpedo.h"
#include "RobotHunter/AI/AI_Giant/AI_Giant.h"
#include "RobotHunter/Utils/ENUM_AI.h"
#include "AIsRefDataAsset.generated.h"

/**
 * DataAsset that store all AI's references
 */
UCLASS(BlueprintType)
class ROBOTHUNTER_API UAIsRefDataAsset : public UDataAsset
{
	GENERATED_BODY()

	/**
	 * The references of AI's type "Paralarva"
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess))
	TSubclassOf<AAI_Protector> paralarvaRef = nullptr;
	/**
	 * The references of AI's type "Trapper"
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess))
	TSubclassOf<AAI_Hunter> trapperRef = nullptr;
	/**
	 * The references of AI's type "Titan"
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess))
	TSubclassOf<AAI_Giant> titanRef = nullptr;
	/**
	 * The references of AI's type "Pennant"
	 */
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess))
	//TSubclassOf<AAI_Scout> pennantRef = nullptr;
	 /**
	  * The references of AI's type "Torpedo"
	  */
	 //UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess))
	 //TSubclassOf<AAI_Torpedo> torpedoRef = nullptr;

public:
	FORCEINLINE TSubclassOf<AAI_Protector> GetParalarvaRef() { return paralarvaRef; }
	//FORCEINLINE TSubclassOf<AAI_Scout> GetPennantRef() { return pennantRef; }
	FORCEINLINE TSubclassOf<AAI_Hunter> GetTrapperRef() { return trapperRef; }
	//FORCEINLINE TSubclassOf<AAI_Torpedo> GetTorpedoRef() { return torpedoRef; }
	FORCEINLINE TSubclassOf<AAI_Giant> GetTitanRef() { return titanRef; }
	FORCEINLINE TArray<TSubclassOf<AAI_Base>> GetAllRefs() { return { paralarvaRef, trapperRef, titanRef }; }
	FORCEINLINE bool CheckAIsRef() { return paralarvaRef && trapperRef && titanRef; }

	/// <summary>
	/// Get the right reference of AI by AI's type
	/// </summary>
	/// <param name="_type"> The AI's type </param>
	/// <returns> A reference of AI </returns>
	TSubclassOf<AAI_Base> GetAIRefByType(EAIType _type);
};
