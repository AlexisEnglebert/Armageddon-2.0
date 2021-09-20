#pragma once
#include "imgui.h"
#include "Scene/Scene.h"
#include "Scene/Entity.h"
#include "Renderer/Primitives.h"
#include "Renderer/Renderer2D.h"
#include "entt/entt.hpp"
class EntityList
{
public:
	EntityList() = default;
	EntityList(Scene& scene) : m_Scene(scene) {};
	static entt::entity Seleceted;

	void ImGuiDraw();
private:
	Scene& m_Scene;
};



