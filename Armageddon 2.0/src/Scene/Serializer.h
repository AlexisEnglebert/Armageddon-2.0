#pragma once
#include "../Macros.h"
#include <filesystem>
#include <fstream>
#include "yaml-cpp/yaml.h"
#include "../Material/AssetManager.h"
#include "../Material/Material.h"
#include "Components.h"
#include "Scene.h"

class DECL Serializer
{
public:
	Serializer() = default;
	Serializer(Scene* Scene) : m_Scene(Scene) {};
	void SerializeMaterial(const std::filesystem::path& FilePath, Material& mat);
	void SerializeScene(const std::filesystem::path& FilePath, Entity& ent, entt::registry& ere);
	void SerializeRessourceMap(const std::filesystem::path& FilePath);
	void DeserializeScene(const std::filesystem::path& FilePath);
	uint64_t DeserializeMaterial(const std::filesystem::path& FilePath);
private:
	Scene* m_Scene;
};

