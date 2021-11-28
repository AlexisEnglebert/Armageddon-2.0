#pragma once
#include "Scene/Scene.h"
#include "Scene/Serializer.h"

class EntityProperties
{
public:
	EntityProperties() = default;
	EntityProperties(Scene& scene) : m_Scene(scene) {};

	void ImGuiDraw();
	void DrawTagComponent(Entity& entity);
	void DrawTransformComponent(Entity& entity);
	void DrawMeshComponent(Entity& entity);
	void DrawMaterialComponent(Entity& entity);
	void DrawLightComponent(Entity& entity);
	void DrawScriptComponent(Entity& entity);
private:
	Scene& m_Scene;
	char SavedBuffer[255] = "";
};