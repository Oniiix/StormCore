// Objectif 3D Copyright

#pragma once

#include "CoreMinimal.h"
#include "MembersActor.h"
#include "ArmMemberActor.generated.h"

/**
 * 
 */
UCLASS()
class ROBOTHUNTER_API AArmMemberActor : public AMembersActor
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, Category = "Custom|Property|Mesh")
	TObjectPtr<USkeletalMeshComponent> memberMesh = nullptr;
	UPROPERTY(EditAnywhere, Category = "Custom|Property|Mesh")
	TObjectPtr<USkeletalMesh> memberMeshAsset = nullptr;
	UPROPERTY(EditAnywhere, Category = "Custom|Property|Mesh")
	FName armSocketName = "";

public:
	FORCEINLINE USkeletalMeshComponent* GetMemberMesh() const { return memberMesh; }
	FORCEINLINE FName GetSocketName() const { return armSocketName; }
	virtual TArray<USkeletalMeshComponent*> GetMemberMeshs() override { return TArray<USkeletalMeshComponent*>({memberMesh}); };

	virtual void SetupMember() override;
};
