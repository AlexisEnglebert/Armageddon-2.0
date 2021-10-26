#pragma once
#include "entt/entt.hpp"
#include "../Log.h"
#include "../Macros.h"
#include "Scene.h"
class DECL Entity
{
public:
	Entity() = default;
	Entity(entt::entity handleEntity, Scene* scene) : HandleEntity(handleEntity), scene(scene) {  }


	template<typename T, typename... Args>
	T& AddComponent(Args&&... args)
	{
		return scene->g_registry.emplace<T>(HandleEntity, std::forward<Args>(args)...);
	};
	template<typename T>
	void DeleteComponent()
	{
		return scene->g_registry.remove<T>(HandleEntity);
	};
	template<typename T>
	bool HasComponent()
	{
		return scene->g_registry.has<T>(HandleEntity);
	};
	template<typename T>
	T& GetComponent()
	{
		return scene->g_registry.get<T>(HandleEntity);
	};
	void DeleteAllComponent()
	{
		
	}
	entt::entity GetHandle()
	{	
		return HandleEntity;
	};


private:
	entt::entity HandleEntity{ entt::null };
	Scene* scene = nullptr;

};