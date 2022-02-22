#pragma once
#include "../Macros.h"
#include <vector>
#include <string>
#if __linux__
//pass pour l'instant
#else
#include <DirectXMath.h>
#endif
/*

Local Pos = Coordonates System from the parent Joint
p(j) => parent Index from Joint j 
Local Pos of joint j (Pj) depend of p(j) -> Parent Index    



Global Pos = position in model/World Space
Multippy all P(j) by its parent Index so the code is : 
	0
	Π	P(i) depend if p(i) Parent of joint i 
	i=j
*/
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

struct DECL JoinPose // /!\ Local Pos joints pos , joint orientation , joint scale
{
	DirectX::XMVECTOR m_rotation;
	DirectX::XMVECTOR m_transform;
	float m_scale; //Uniform Scale
};


struct DECL SkeletonPosition // /!\ Local Pos
{
	Skeleton* m_pSkeleton;
	JoinPose* m_aLocalPos;
	DirectX::XMMATRIX* m_aglobalPos;
};