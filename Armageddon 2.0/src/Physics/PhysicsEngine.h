#pragma once
#include "../Macros.h"
#include "../Log.h"
#include <PxPhysicsAPI.h>

namespace Armageddon
{

	//TODO HERE MEMORY LEAK

	class DECL PhysicsInterface
	{
	public:

		~PhysicsInterface()
		{
			g_PhysxFoundation->release();
			g_Physics->release();
		}
		static physx::PxFoundation* g_PhysxFoundation;
		static physx::PxErrorCallback* g_PhysxErrorCallback;
		static physx::PxDefaultAllocator g_PhysxAllocator;
		static physx::PxPhysics* g_Physics;
		static physx::PxScene* g_PhysxScene;
	private:
	};
	class DECL PhyicsEngine
	{
	public:
		PhyicsEngine() = default;
		void init();
		~PhyicsEngine() 
		{
			
		};
	private:
	};
}