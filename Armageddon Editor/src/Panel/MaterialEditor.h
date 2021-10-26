#pragma once
#include "Scene/Scene.h"
class MaterialEditor
{
public:
	MaterialEditor() = default;
	MaterialEditor(Scene& scene) : m_Scene(scene) {};
	void ImGuiDraw();
	void DrawMaterial(uint64_t& matRef);
	void DrawTextureTree(uint64_t& matRef);
private:
	Scene& m_Scene;

};


