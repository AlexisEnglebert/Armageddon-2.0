#pragma once
#include "../Log.h"
#include "../Macros.h"
#include "PxPhysics.h"
#include <DirectXMath.h>
namespace Armageddon
{
	class DECL PhysXUtils
	{
	public:
		physx::PxVec3 DirectXToPhysXVec3(DirectX::XMVECTOR& vector);
		physx::PxMat44 DirectXToPhysMat(DirectX::XMMATRIX& mat);
	//	static physx::PxTransform ToPhysTransform(const TransformComponent& component);
	private:
	};
}