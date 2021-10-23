#pragma once
#include "Scene/Scene.h"
#include "Scene/Entity.h"
class BoneDebug
{
public:
	BoneDebug() = default;
	BoneDebug(Scene& scene) : m_Scene(scene){};
	void ProcessNode(Skeleton* skeleton,uint8_t id);
	void ImGuiDraw();
private:
	Scene& m_Scene;

};