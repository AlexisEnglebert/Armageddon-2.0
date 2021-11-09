#include "PhysicsEngine.h"


physx::PxFoundation* Armageddon::PhysicsInterface::g_PhysxFoundation = nullptr;
physx::PxErrorCallback* Armageddon::PhysicsInterface::g_PhysxErrorCallback = nullptr;
physx::PxPhysics* Armageddon::PhysicsInterface::g_Physics = nullptr;
physx::PxDefaultAllocator Armageddon::PhysicsInterface::g_PhysxAllocator;
physx::PxScene* Armageddon::PhysicsInterface::g_PhysxScene = nullptr;
void Armageddon::PhyicsEngine::init()
{
	Armageddon::PhysicsInterface::g_PhysxFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, Armageddon::PhysicsInterface::g_PhysxAllocator, *Armageddon::PhysicsInterface::g_PhysxErrorCallback);
	if (!Armageddon::PhysicsInterface::g_PhysxFoundation)
	{
		Armageddon::Log::GetLogger()->error("PxCreateFoundation Failed");
	}

	Armageddon::PhysicsInterface::g_Physics = PxCreatePhysics(PX_PHYSICS_VERSION, *Armageddon::PhysicsInterface::g_PhysxFoundation, physx::PxTolerancesScale(), true, nullptr);
	if (!Armageddon::PhysicsInterface::g_Physics)
	{
		Armageddon::Log::GetLogger()->error("PxCreatePhysics Failed");
	}
	//physx::PxSceneDesc sDesc(Armageddon::PhysicsInterface::g_Physics->getTolerancesScale());
	//sDesc.filterShader = physx::PxDefaultSimulationFilterShader;
	//Armageddon::PhysicsInterface::g_PhysxScene = Armageddon::PhysicsInterface::g_Physics->createScene(sDesc);
}
