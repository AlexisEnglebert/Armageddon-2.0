#include "MaterialEditor.h"
#include "EntityList.h"
void MaterialEditor::ImGuiDraw()
{
	ImGui::Begin("Material Editor");
	/*if (ImGui::BeginDragDropTarget())
	{
		if (auto payload = ImGui::AcceptDragDropPayload("ASSET", ImGuiInputTextFlags_ReadOnly))
		{
			const char* str = (const char*)payload->Data;
			Armageddon::Log::GetLogger()->trace(str);
		}

		ImGui::EndDragDropTarget();

	}*/

	if (EntityList::Seleceted != entt::null)
	{
		auto entity = m_Scene.GetEntityByID(EntityList::Seleceted);
		if (entity.HasComponent<MeshComponent>())
		{
			
				auto& component = entity.GetComponent<MeshComponent>();
				if (component.m_mesh.v_Materials.size() != 0)
				{
					for (auto& mat : component.m_mesh.v_Materials)
					{

						DrawMaterial(mat);
					}
				}
			
		}
	}
	ImGui::End();
}

void MaterialEditor::DrawMaterial(Armageddon::Material& mat)
{
	if (ImGui::TreeNodeEx(mat.m_name.c_str(), 0))
	{
		//ImGui::InputText("")
		const char* items[] = { "Opaque","Transparent" };
		 ImGui::Combo("Rendering mode ", &mat.RenderMode, items,ARRAYSIZE(items));
		 ImGui::DragFloat("Rougness", &mat.m_PBRBUFFER.Roughness, 0.01, 0, 1);
		 ImGui::DragFloat("Metalic", &mat.m_PBRBUFFER.Metalic, 0.01, 0, 1);
		DrawTextureTree(mat);
		

		ImGui::TreePop();
	}
}

void MaterialEditor::DrawTextureTree(Armageddon::Material& mat)
{
	ImGui::Columns(2, 0, false);
	//ImGui::SetColumnWidth(0, 150.0f);

	ImGui::Image(mat.m_albedoMap.GetRessourceView(), { 50,50 });
	ImGui::SameLine();
	ImGui::Text("Albedo");
	float color[] = { mat.m_PBRBUFFER.AlbedoTint.x ,mat.m_PBRBUFFER.AlbedoTint.y,mat.m_PBRBUFFER.AlbedoTint.z };
	ImGui::SameLine();
	ImGui::ColorEdit3("Color", color, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);
	mat.m_PBRBUFFER.AlbedoTint.x = color[0];
	mat.m_PBRBUFFER.AlbedoTint.y = color[1];
	mat.m_PBRBUFFER.AlbedoTint.z = color[2];
	ImGui::NextColumn();
	ImGui::InputText("##Albedomap", mat.m_albedoMap.TexturePath.string().data(), mat.m_albedoMap.TexturePath.string().size(), ImGuiInputTextFlags_AllowTabInput | ImGuiInputTextFlags_ReadOnly);

	if (ImGui::BeginDragDropTarget())
	{
		if (auto payload = ImGui::AcceptDragDropPayload("ASSET", ImGuiInputTextFlags_ReadOnly))
		{
			const char* str = (const char*)payload->Data;
			mat.m_albedoMap.TexturePath = std::filesystem::path(str);
			mat.SetAlbedoMap(mat.m_albedoMap.TexturePath);
			Armageddon::Log::GetLogger()->trace(str);
		}

		ImGui::EndDragDropTarget();

	}

	ImGui::SameLine();
	ImGui::PushID("AlbedoButton");
	if (ImGui::Button("...", { 30.0f,20.0f }))
	{

	}

	ImGui::PopID();
	ImGui::Columns();

	ImGui::Columns(2, 0, false);
	ImGui::Image(mat.m_normalMap.GetRessourceView(), { 50,50 });
	ImGui::SameLine();
	ImGui::Text("Normal");
	ImGui::NextColumn();

	ImGui::InputText("##NormalMap", mat.m_normalMap.TexturePath.string().data(), mat.m_normalMap.TexturePath.string().size(), ImGuiInputTextFlags_AllowTabInput | ImGuiInputTextFlags_ReadOnly);
	if (ImGui::BeginDragDropTarget())
	{
		if (auto payload = ImGui::AcceptDragDropPayload("ASSET", ImGuiInputTextFlags_ReadOnly))
		{
			const char* str = (const char*)payload->Data;
			mat.m_normalMap.TexturePath = std::filesystem::path(str);
			mat.SetNormalMap(mat.m_normalMap.TexturePath);
			Armageddon::Log::GetLogger()->trace(str);
		}

		ImGui::EndDragDropTarget();

	}
	ImGui::SameLine();
	if (ImGui::Button("##2...", { 30.0f,20.0f }))
	{

	}

	ImGui::Columns();

	ImGui::Columns(2, 0, false);
	ImGui::Image(mat.m_specularMap.GetRessourceView(), { 50,50 });
	ImGui::SameLine();
	ImGui::Text("Specular map");
	ImGui::NextColumn();

	ImGui::InputText("##SpecularMap", mat.m_specularMap.TexturePath.string().data(), mat.m_specularMap.TexturePath.string().size(), ImGuiInputTextFlags_AllowTabInput | ImGuiInputTextFlags_ReadOnly);
	if (ImGui::BeginDragDropTarget())
	{
		if (auto payload = ImGui::AcceptDragDropPayload("ASSET", ImGuiInputTextFlags_ReadOnly))
		{
			const char* str = (const char*)payload->Data;
			mat.m_specularMap.TexturePath = std::filesystem::path(str);
			mat.SetSpecularMap(mat.m_specularMap.TexturePath);
			Armageddon::Log::GetLogger()->trace(str);
		}

		ImGui::EndDragDropTarget();

	}
	ImGui::SameLine();
	if (ImGui::Button("##3...", { 30.0f,20.0f }))
	{

	}
	ImGui::Columns();


	ImGui::Columns(2, 0, false);

	ImGui::Image(mat.m_ambiantOcclusionMap.GetRessourceView(), { 50,50 });
	ImGui::SameLine();
	ImGui::Text("Ambient occlusion");

	ImGui::NextColumn();

	ImGui::InputText("##AOmap", mat.m_ambiantOcclusionMap.TexturePath.string().data(), mat.m_ambiantOcclusionMap.TexturePath.string().size(), ImGuiInputTextFlags_AllowTabInput | ImGuiInputTextFlags_ReadOnly);
	if (ImGui::BeginDragDropTarget())
	{
		if (auto payload = ImGui::AcceptDragDropPayload("ASSET", ImGuiInputTextFlags_ReadOnly))
		{
			const char* str = (const char*)payload->Data;
			mat.m_ambiantOcclusionMap.TexturePath = std::filesystem::path(str);
			mat.SetAOMap(mat.m_ambiantOcclusionMap.TexturePath);
			Armageddon::Log::GetLogger()->trace(str);
		}

		ImGui::EndDragDropTarget();

	}
	ImGui::SameLine();
	if (ImGui::Button("##4...", { 30.0f,20.0f }))
	{

	}


	ImGui::Columns();


	//ImGui::Image(vMat[i].GetAOMap(), { 50,50 });
	//ImGui::SameLine();
	ImGui::Checkbox("Use MetalMap", &mat.m_PBRBUFFER.UseMetalMap);
	if (mat.m_PBRBUFFER.UseMetalMap)
	{
		ImGui::Columns(2, 0, false);

		ImGui::Text("Metal");
		ImGui::SameLine();
		ImGui::NextColumn();

		ImGui::InputText("##Metalmap", mat.m_metalicMap.TexturePath.string().data(), mat.m_metalicMap.TexturePath.string().size(), ImGuiInputTextFlags_AllowTabInput | ImGuiInputTextFlags_ReadOnly);
		if (ImGui::BeginDragDropTarget())
		{
			if (auto payload = ImGui::AcceptDragDropPayload("ASSET", ImGuiInputTextFlags_ReadOnly))
			{
				const char* str = (const char*)payload->Data;
				mat.m_metalicMap.TexturePath = std::filesystem::path(str);
				mat.SetMetalicMap(mat.m_metalicMap.TexturePath);
				Armageddon::Log::GetLogger()->trace(str);
			}

			ImGui::EndDragDropTarget();

		}
		ImGui::SameLine();
		if (ImGui::Button("##5...", { 30.0f,20.0f }))
		{

		}
		ImGui::Columns();
	}

	ImGui::Checkbox("Use Emission", &mat.m_PBRBUFFER.UseEmisive);
	if (mat.m_PBRBUFFER.UseEmisive)
	{
		ImGui::Columns(2, 0, false);
		ImGui::Image(mat.m_EmissiveMap.GetRessourceView(), { 50,50 });
		ImGui::SameLine();
		ImGui::Text("Emisive");

		ImGui::NextColumn();

		ImGui::InputText("##Emisive", mat.m_EmissiveMap.TexturePath.string().data(), mat.m_EmissiveMap.TexturePath.string().size(), ImGuiInputTextFlags_AllowTabInput | ImGuiInputTextFlags_ReadOnly);
		if (ImGui::BeginDragDropTarget())
		{
			if (auto payload = ImGui::AcceptDragDropPayload("ASSET", ImGuiInputTextFlags_ReadOnly))
			{
				const char* str = (const char*)payload->Data;
				mat.m_EmissiveMap.TexturePath = std::filesystem::path(str);
				mat.SetEmisiveMap(mat.m_EmissiveMap.TexturePath);
				Armageddon::Log::GetLogger()->trace(str);
			}

			ImGui::EndDragDropTarget();

		}
		ImGui::SameLine();
		if (ImGui::Button("##5...", { 30.0f,20.0f }))
		{

		}
		ImGui::Columns();
	}


}
