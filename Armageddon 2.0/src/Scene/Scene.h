#pragma once
#include <filesystem>
#include "entt/entt.hpp"
#include "../Log.h"
#include "../Macros.h"
#include "Components.h"
#include "../utils/Timer.h"
class Entity;

	enum class SceneState
	{
		Undefined,
		Editor,
		Runtime
	};

	class DECL Scene
	{
	public:
		Scene() = default;
		Entity CreateEntity();
		Entity& GetEntityByID(entt::entity ID);
		void ClearScene();
		void UpdateScene();
		void RenderScene();
		void OnRuntimeStart();
		void LoadScene(const std::filesystem::path& path);
		void DuplicateEntity(Entity RefEntity);
		void DeleteEntity(Entity& entity);
		void InitScene();

		const char* m_sceneName = "Untilted";
		entt::registry g_registry;
		std::map<uint64_t, Entity> EntityMap;
		SceneState m_SceneState = SceneState::Undefined;
		float Scenetime = 0.0f; //TODO: double
		float SceneDeltaTime =  1.0f/60.0f; //TODO: double	
		WorldBuffer m_SceneBuffer;
	private:
		friend class Entity;

	};

