#include "EntityList.h"
entt::entity EntityList::Seleceted = entt::null;

void EntityList::ImGuiDraw()
{
	ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.0862745098f, 0.0862745098f, 0.0862745098f, 1.0f));
	ImGui::Begin("Entity list");
	ImGui::PopStyleColor();

	
	for ( auto iterator = m_Scene.EntityMap.begin(); iterator != m_Scene.EntityMap.cend(); iterator++)
	{
		if (iterator->second.HasComponent<TagComponent>())
		{
			ImGui::PushID((int)iterator->second.GetHandle());
			//TODO mettre des selectables Node (tree) pour les prefabs par exemple
			/*/ImGuiTreeNodeFlags isSelected;
			if (Seleceted == ent.GetHandle()) {
				isSelected |= ImGuiTreeNodeFlags_Selected;
			}
			if (ImGui::TreeNodeEx(ent.GetComponent<TagComponent>().Tag.c_str(),ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_SpanAvailWidth| isSelected))
			{
				if (ImGui::IsItemClicked())
				{
					Seleceted = ent.GetHandle();
				}
				ImGui::TreePop();
			}*/
			if (ImGui::Selectable(iterator->second.GetComponent<TagComponent>().Tag.c_str(), Seleceted == iterator->second.GetHandle(), 0, {0,0}))
			{
				Seleceted = iterator->second.GetHandle();
			}
			ImGui::PopID();
		}
	}

	
	
	if (ImGui::BeginPopupContextWindow("Create.."))
	{
		ImGui::MenuItem("Create....", NULL, false, false);  
		if (ImGui::BeginMenu("Primitive"))
		{
			if (ImGui::MenuItem("Cube"))
			{
				auto entity = m_Scene.CreateEntity();
				entity.AddComponent<TagComponent>("Cube");
				entity.AddComponent<MeshComponent>(Armageddon::Primitives::GenerateCube());
				entity.AddComponent<MaterialComponent>();

			}
			if (ImGui::MenuItem("Sphere"))
			{
				auto entity = m_Scene.CreateEntity();
				entity.AddComponent<TagComponent>("Sphere");
				entity.AddComponent<MeshComponent>(Armageddon::Primitives::GenerateSphere(1,38,20));
				entity.AddComponent<MaterialComponent>();

			}
			if (ImGui::MenuItem("Plane"))
			{
				auto entity = m_Scene.CreateEntity();
				entity.AddComponent<TagComponent>("Plane");
				entity.AddComponent<MeshComponent>(Armageddon::Renderer2D::GeneratePlane());
				entity.AddComponent<MaterialComponent>();
				//m_quad.SetPixelShader(L"..\\bin\\Debug-x64\\Armageddon 2.0\\BillBoardPixel.cso");
				//m_quad.SetVertexShader(L"..\\bin\\Debug-x64\\Armageddon 2.0\\BillBoardVertex.cso");
			}
			ImGui::MenuItem("Torus");
			ImGui::MenuItem("Cone");

			ImGui::EndMenu();

		}
		if (ImGui::MenuItem("Debug Model"))
		{
			auto entity = m_Scene.CreateEntity();
			entity.AddComponent<TagComponent>("Debug Model");
			//entity.AddComponent<MeshComponent>("..\\Armageddon Editor\\Assets\\Models\\sponza.obj");
			//entity.AddComponent<MeshComponent>("..\\Armageddon Editor\\Assets\\Models\\suzanne.obj");
			entity.AddComponent<MeshComponent>("..\\Armageddon Editor\\Assets\\Models\\Apollo_MatBall_FinalChanges.fbx");
		//	entity.AddComponent<MeshComponent>("..\\Armageddon Editor\\Assets\\Models\\Fantasy Town Kit\\Models\\OBJ format\\chimney.obj");
		}
		if (ImGui::MenuItem("Raw Entity"))
		{
			auto entity = m_Scene.CreateEntity();
			entity.AddComponent<TagComponent>("Entity");
		}
		if (ImGui::BeginMenu("Light"))
		{
			
			if (ImGui::MenuItem("PointLight"))
			{
				auto entity = m_Scene.CreateEntity();
				entity.AddComponent<TagComponent>("PointLight");
				entity.AddComponent<LightComponent>();
				auto& component = entity.GetComponent<LightComponent>();
				component.type = 0;
				entity.AddComponent<MeshComponent>(Armageddon::Renderer2D::GeneratePlane());
				Armageddon::Renderer::g_PointLightsVector.push_back(component.m_pointLight);
				auto& MeshComp = entity.GetComponent<MeshComponent>();
				MeshComp.m_mesh.v_MaterialReference[0] = AssetManager::GetOrCreateMaterial("LightMaterial");
				MeshComp.ShowComponent = false;

			}
			if (ImGui::MenuItem("DirectionalLight"))
			{
				auto entity = m_Scene.CreateEntity();
				entity.AddComponent<TagComponent>("DirectionalLight");
				entity.AddComponent<LightComponent>();
				auto& component = entity.GetComponent<LightComponent>();
				component.type = 1;
				entity.AddComponent<MeshComponent>(Armageddon::Renderer2D::GeneratePlane());
				Armageddon::Renderer::g_DirectLightsVector.push_back(component.m_directionalLight);
				auto& MeshComp = entity.GetComponent<MeshComponent>();
				MeshComp.m_mesh.v_MaterialReference[0] = AssetManager::GetOrCreateMaterial("LightMaterial");
				MeshComp.ShowComponent = false;
			}
			ImGui::EndMenu();

		}
	/*	ImGui::Text("Edit name:");
		if (ImGui::Button("Close"))
			ImGui::CloseCurrentPopup();*/
		ImGui::EndPopup();
	}
	ImGui::End();
}
