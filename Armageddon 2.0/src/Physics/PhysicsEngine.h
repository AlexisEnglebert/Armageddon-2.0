#pragma once
#include "../Macros.h"
#include "../Log.h"
#include "PxPhysicsAPI.h"
namespace Armageddon
{

	class DECL PhysicsInterface
	{
	public:
		static physx::PxFoundation* g_PhysxFoundation;
		static physx::PxErrorCallback* g_PhysxErrorCallback;
		static physx::PxDefaultAllocator g_PhysxAllocator;
	private:
	};
	class DECL PhyicsEngine
	{
	public:
		PhyicsEngine() = default;
		void init();
		~PhyicsEngine() {};
	private:
	};
}