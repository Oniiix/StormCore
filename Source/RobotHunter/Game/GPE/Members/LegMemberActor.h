// Objectif 3D Copyright

#pragma once

#include "CoreMinimal.h"
#include "MembersActor.h"
#include "LegMemberActor.generated.h"

/**
 * 
 */
UCLASS()
class ROBOTHUNTER_API ALegMemberActor : public AMembersActor
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, Category = "Custom|Property|Mesh")
	TObjectPtr<USkeletalMeshComponent> rightMemberMesh = nullptr;
	UPROPERTY(EditAnywhere, Category = "Custom|Property|Mesh")
	TObjectPtr<USkeletalMeshComponent> leftMemberMesh = nullptr;
	UPROPERTY(EditAnywhere, Category = "Custom|Property|Mesh")
	TObjectPtr<USkeletalMesh> rightMemberMeshAsset = nullptr;
	UPROPERTY(EditAnywhere, Category = "Custom|Property|Mesh")
	TObjectPtr<USkeletalMesh> leftMemberMeshAsset = nullptr;
	UPROPERTY(EditAnywhere, Category = "Custom|Property|Mesh")
	FName rightLegSocketName = "";
	UPROPERTY(EditAnywhere, Category = "Custom|Property|Mesh")
	FName leftLegSocketName = "";

public:
	FORCEINLINE USkeletalMeshComponent* GetRightMemberMesh() const { return rightMemberMesh; }
	FORCEINLINE USkeletalMeshComponent* GetLeftMemberMesh() const { return leftMemberMesh; }
	FORCEINLINE FVector GetRightLegMeshLocation() const { return rightMemberMesh->GetSocketLocation(rightLegSocketName); }
	FORCEINLINE FVector GetLeftLegMeshLocation() const { return leftMemberMesh->GetSocketLocation(leftLegSocketName); }

	virtual void SetupMember() override;
};
