

#pragma once

#include "CoreMinimal.h"
#include "LayeredBoneBlendLibrary.h"
#include "CustomLayeredBoneBlendLibrary.generated.h"

/**
 * 
 */
UCLASS()
class ROBOTHUNTER_API UCustomLayeredBoneBlendLibrary : public ULayeredBoneBlendLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "Layered Bone Blend", meta = (BlueprintThreadSafe))
	static bool SetMeshSpaceRotationBlend(const FLayeredBoneBlendReference& LayeredBoneBlend, const bool _value);

	UFUNCTION(BlueprintPure, Category = "Layered Bone Blend", meta = (BlueprintThreadSafe))
	static bool GetMeshSpaceRotationBlend(const FLayeredBoneBlendReference& LayeredBoneBlend);
};
