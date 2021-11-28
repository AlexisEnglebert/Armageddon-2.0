#include "PhysicActor.h"
#include "../Scene/Components.h"

//Armageddon::PhysicActor::PhysicActor(Entity& ent) : m_ent(ent)
//{
//}

void Armageddon::PhysicActor::CreateRigidBody()
{
	//auto& component = m_ent.GetComponent<TransformComponent>();
	//m_actor = Armageddon::PhysicsInterface::g_Physics->createRigidDynamic(Armageddon::PhysXUtils::ToPhysTransform(component));
	//m_actor->setLinearVelocity(physx::PxVec3(0.0f, 1.0f, 1.0f));
	//m_actor->setAngularVelocity(physx::PxVec3(1.0f, 0.0f, 0.0f));
	//physx::PxRigidBodyExt::setMassAndUpdateInertia(*m_actor,5);
	m_PhysxMaterial = Armageddon::PhysicsInterface::g_Physics->createMaterial(0.5f, 0.5f, 0.1f); // static friction, dynamic friction,

	Armageddon::PhysicsInterface::g_PhysxScene->addActor(*m_actor);
}

void Armageddon::PhysicActor::update()
{
	//auto& component = m_ent.GetComponent<TransformComponent>();
	physx::PxTransform Transform = m_actor->getGlobalPose();
	//component.Translation = DirectX::XMFLOAT3(Transform.p.x, Transform.p.y, Transform.p.z);
	//component.Rotation = DirectX::XMFLOAT3(Transform.q.x, Transform.q.y, Transform.q.z);
}

