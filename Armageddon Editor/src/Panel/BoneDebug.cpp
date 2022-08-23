#include "BoneDebug.h"
#include "EntityList.h"

void BoneDebug::ProcessNode(std::shared_ptr<Joint>& ParentJoint, uint8_t id)
{
	for (UINT i = 0; i < ParentJoint->m_child.size(); i++)
	{
			if (ImGui::TreeNode(ParentJoint->m_child[i]->m_name.c_str()))
			{
				uint8_t newid = id + 1 ;
				ProcessNode(ParentJoint->m_child[i], newid);
				ImGui::TreePop();
			}
	}
}

void BoneDebug::ImGuiDraw()
{
	ImGui::Begin("Bone hierarchy");
	if (EntityList::Seleceted != entt::null)
	{
		auto entity = m_Scene.GetEntityByID(EntityList::Seleceted);
		if (entity.HasComponent<MeshComponent>())
		{
			auto& component = entity.GetComponent<MeshComponent>();
			if (component.m_mesh.m_skeleton.m_JointsCount > 0) 
			{
				if (ImGui::TreeNode(component.m_mesh.m_skeleton.m_rootJoint->m_name.c_str()))
				{
					ProcessNode(component.m_mesh.m_skeleton.m_rootJoint, 1);
					ImGui::TreePop();
				}
			}
			/*for (UINT i = 0; i < component.m_mesh.m_skeleton.m_aJoints.size(); i++)
			{
				
				ImGui::Text(component.m_mesh.m_skeleton.m_aJoints[i].m_name.c_str());
				/*mGui::SameLine();
				ImGui::Text((char * )component.m_mesh.m_skeleton.m_aJoints[i].m_Parentid);
			}*/
		}
	}

	ImGui::End();
}
