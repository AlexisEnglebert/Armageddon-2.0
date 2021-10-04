#pragma once
#include "entt/entt.hpp"
#include "../Log.h"
#include "../Macros.h"
#include "Components.h"
#include <filesystem>
	class Entity;
	class DECL Scene
	{
	public:
		Scene() = default;
		Entity CreateEntity();
		Entity& GetEntityByID(entt::entity ID);
		void ClearScene();
		void LoadScene(const std::filesystem::path& path);
		void DuplicateEntity(Entity& RefEntity);
		entt::registry g_registry;
		std::vector<Entity> v_Entity;
	private:
		friend class Entity;

	};
