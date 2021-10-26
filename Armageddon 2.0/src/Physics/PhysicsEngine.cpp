#include "PhysicsEngine.h"


physx::PxFoundation* Armageddon::PhysicsInterface::g_PhysxFoundation = nullptr;
physx::PxErrorCallback* Armageddon::PhysicsInterface::g_PhysxErrorCallback = nullptr;
physx::PxDefaultAllocator Armageddon::PhysicsInterface::g_PhysxAllocator;

void Armageddon::PhyicsEngine::init()
{
	Armageddon::PhysicsInterface::g_PhysxFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, Armageddon::PhysicsInterface::g_PhysxAllocator, *Armageddon::PhysicsInterface::g_PhysxErrorCallback);
}
