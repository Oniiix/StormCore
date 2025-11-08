// Objectif 3D Copyright


#include "LegMemberActor.h"
#include "RobotHunter/CCC/Character/MainCharacter.h"

void ALegMemberActor::SetupMember()
{
	if (!rightMemberMeshAsset || !leftMemberMeshAsset)return;
	rightMemberMesh = NewObject<USkeletalMeshComponent>(mainCharacter,FName(rightMemberMeshAsset->GetName()));
	rightMemberMesh->SetupAttachment(mainCharacter->GetMesh());
	mainCharacter->AddOwnedComponent(rightMemberMesh);
	rightMemberMesh->RegisterComponent();

	rightMemberMesh->AttachToComponent(mainCharacter->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, rightLegSocketName);
	rightMemberMesh->SetSkeletalMesh(rightMemberMeshAsset);

	leftMemberMesh = NewObject<USkeletalMeshComponent>(mainCharacter, FName(leftMemberMeshAsset->GetName()));
	leftMemberMesh->SetupAttachment(mainCharacter->GetMesh());
	mainCharacter->AddOwnedComponent(leftMemberMesh);
	leftMemberMesh->RegisterComponent();

	leftMemberMesh->AttachToComponent(mainCharacter->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, leftLegSocketName);
	leftMemberMesh->SetSkeletalMesh(leftMemberMeshAsset);

	rightMemberMesh->SetRelativeRotation(FRotator(-80, 180, 90));
	leftMemberMesh->SetRelativeRotation(FRotator(-80, 180, 90));

	//rightMemberMesh->SetIsReplicated(true);
	//leftMemberMesh->SetIsReplicated(true);
}
