#pragma once
#include "../../Macros.h"
#include <vector>;
#include <string>
#include <DirectXMath.h>

struct DECL Joint
{
	Joint(std::string JointName, uint8_t parentId	, DirectX::XMMATRIX InverseBlindPos) : m_name(JointName), m_Parentid(parentId),m_inverseBlindPose(InverseBlindPos)
	{

	};
	DirectX::XMMATRIX m_inverseBlindPose;
	std::string m_name;
	uint8_t m_Parentid;
	
};

struct DECL Skeleton
{
	std::vector<Joint> m_aJoints;
	uint32_t m_JointsCount = 0;
};

struct DECL JoinPosition
{
	DirectX::XMVECTOR m_rotation;
	DirectX::XMVECTOR m_transform;
	//float32_t m_scale;
};


struct DECL SkeletonPosition
{
	Skeleton* m_pSkeleton;
	JoinPosition* m_aLocalPos;
	DirectX::XMMATRIX* m_aglobalPos;
};