#pragma once
#include "Scene/Scene.h"
class MaterialEditor
{
public:
	MaterialEditor() = default;
	MaterialEditor(Scene& scene) : m_Scene(scene) {};
	void ImGuiDraw();
	void DrawMaterial(Armageddon::Material& mat);
	void DrawTextureTree(Armageddon::Material& mat);
private:
	Scene& m_Scene;

};


