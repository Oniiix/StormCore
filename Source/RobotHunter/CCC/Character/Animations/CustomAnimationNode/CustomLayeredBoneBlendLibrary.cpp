


#include "CustomLayeredBoneBlendLibrary.h"
#include "AnimNodes/AnimNode_LayeredBoneBlend.h"

bool UCustomLayeredBoneBlendLibrary::SetMeshSpaceRotationBlend(const FLayeredBoneBlendReference& LayeredBoneBlend, const bool _value)
{
	bool _meshRotationBlend = false;

	LayeredBoneBlend.CallAnimNodeFunction<FAnimNode_LayeredBoneBlend>(
		TEXT("SetTest"),
		[&_meshRotationBlend, _value](FAnimNode_LayeredBoneBlend& InLayeredBoneBlend)
		{
			InLayeredBoneBlend.bMeshSpaceRotationBlend = _value;
			_meshRotationBlend = InLayeredBoneBlend.bMeshSpaceRotationBlend;
		});
	return _meshRotationBlend;
}

bool UCustomLayeredBoneBlendLibrary::GetMeshSpaceRotationBlend(const FLayeredBoneBlendReference& LayeredBoneBlend)
{
	bool _meshRotationBlend = false;

	LayeredBoneBlend.CallAnimNodeFunction<FAnimNode_LayeredBoneBlend>(
		TEXT("GetTest"),
		[&_meshRotationBlend](FAnimNode_LayeredBoneBlend& InLayeredBoneBlend)
		{
			_meshRotationBlend = InLayeredBoneBlend.bMeshSpaceRotationBlend;
		});
	return _meshRotationBlend;
}
