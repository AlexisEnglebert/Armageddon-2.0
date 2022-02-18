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
	void RenderMaterial(uint64_t& matRef);
	bool m_showWindow = true;

private:
	Scene& m_Scene;
	RenderTexture m_previewTexture{1920.0f/2.0f,1080.0f / 2.0f,DXGI_FORMAT_R32G32B32A32_FLOAT};
	Mesh m_previewSphere = Mesh("..\\Armageddon 2.0\\Assets\\Models\\Defaultsphere.obj");

};


