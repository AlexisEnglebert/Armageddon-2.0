#pragma once
#include "Scene/Scene.h"
#include "Scene/Entity.h"
#include "Material/AssetManager.h"
class RessourceManager
{
public:
	RessourceManager() = default;
	RessourceManager(Scene& scene) : m_Scene(scene) {};
	void ImGuiDraw();
private:
	Scene& m_Scene;
	std::map<std::string, std::string> m_assetMap;
};