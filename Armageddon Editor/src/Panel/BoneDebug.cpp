#include "BoneDebug.h"
#include "EntityList.h"

void BoneDebug::ImGuiDraw()
{
	ImGui::Begin("Bone hierarchy");
	if (EntityList::Seleceted != entt::null)
	{
		auto entity = m_Scene.GetEntityByID(EntityList::Seleceted);
		if (entity.HasComponent<MeshComponent>())
		{
			auto& component = entity.GetComponent<MeshComponent>();
			for (UINT i = 0; i < component.m_mesh.m_skeleton.m_aJoints.size(); i++)
			{
				ImGui::Text(component.m_mesh.m_skeleton.m_aJoints[i].m_name.c_str());
				/*mGui::SameLine();
				ImGui::Text((char * )component.m_mesh.m_skeleton.m_aJoints[i].m_Parentid);*/
			}
		}
	}

	ImGui::End();
}
