#pragma once
#include "../Log.h"
#include "../Macros.h"
#include "PhysicsEngine.h"
#include "PxRigidBody.h"
#include "PhysXUtils.h"

namespace Armageddon
{
	class DECL PhysicActor
	{
	public:
		PhysicActor() = default;
		//PhysicActor(Entity& ent);
		void CreateRigidBody();
		void update();
		//Entity& m_ent;

	private:
		physx::PxMaterial* m_PhysxMaterial;
		physx::PxRigidDynamic* m_actor;
	};
}