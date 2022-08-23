#pragma once
#include "../Macros.h"
#include <vector>
#include <string>
#include <memory>
#include <map>

#if __linux__
//pass pour l'instant
#else
#include <DirectXMath.h>
#endif

/*
* Join will stored like a tree via the parentID
* Local Pos = Coordonates System from the parent Joint 
* p(j) => parent Index from Joint j 
*
*Local Pos of joint j (Pj) depend of p(j) -> Parent Index
*
*Global Pos = position in model/World Space
*Multiply all P(j) by its parent Index so the code is :
*	0
*	Π	P(i) depend if p(i) Parent of joint i
*	i=j
*/
struct DECL JoinPose // /!\ Local Pos joints pos , joint orientation , joint scale
{
	JoinPose() = default;
	DirectX::XMFLOAT3 m_rotation;
	DirectX::XMFLOAT3 m_translation;
	float m_scale; //Uniform Scale

	DirectX::XMMATRIX GetLocalMat() 
	{
		return DirectX::XMMatrixTransformation(DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f), DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f)
			, DirectX::XMVectorSet(m_scale, m_scale, m_scale, 1.0f), DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f), DirectX::XMQuaternionRotationRollPitchYaw(m_rotation.x, m_rotation.y, m_rotation.z)
			, DirectX::XMVectorSet(m_translation.x, m_translation.y, m_translation.z, 1.0f));
	};
};
struct DECL Joint
{
	Joint(std::string JointName, uint8_t parentId	, DirectX::XMMATRIX InverseBlindPos) : m_name(JointName), m_Parentid(parentId),m_inverseBlindPose(InverseBlindPos)
	{

	};

	Joint() = default;
	Joint(const Joint& j) = default;
	std::string m_name;
	JoinPose m_position; //Local joint position (from the parent bone) !
	uint8_t m_Parentid;

	DirectX::XMMATRIX m_modelMatrix; //Parent Local * current  Local
	DirectX::XMMATRIX m_inverseBlindPose;

	std::vector<std::shared_ptr<Joint>> m_child; //TODO In a single array with Parentid as index 
	
};

struct DECL Skeleton
{
	std::shared_ptr<Joint> m_rootJoint;
	uint32_t m_JointsCount = 0;
	std::map<std::string, std::shared_ptr<Joint>> m_Bonemap;
	void UpdateJoint(std::shared_ptr<Joint>& Parentjoint, std::shared_ptr<Joint>& currentJoint);
};


