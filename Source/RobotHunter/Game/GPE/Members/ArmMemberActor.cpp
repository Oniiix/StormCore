// Objectif 3D Copyright
#include "ArmMemberActor.h"
#include "RobotHunter/CCC/Character/MainCharacter.h"



void AArmMemberActor::SetupMember()
{
	memberMesh = NewObject<USkeletalMeshComponent>(mainCharacter,FName(memberMeshAsset->GetName()));
	memberMesh->SetupAttachment(mainCharacter->GetMesh());
	mainCharacter->AddOwnedComponent(memberMesh);
	memberMesh->RegisterComponent();
	memberMesh->SetSkeletalMesh(memberMeshAsset);

	memberMesh->SetRelativeRotation(FRotator(-80, 0, 0));
	memberMesh->AttachToComponent(mainCharacter->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, armSocketName);
	
	//memberMesh->SetIsReplicated(true);
}
