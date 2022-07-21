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
				if (component.m_mesh.v_MaterialReference.size() != 0)
				{
					for (auto& mat : component.m_mesh.v_MaterialReference)
					{

						DrawMaterial(mat);
					}
				}
			
		}
	}
	ImGui::End();
}

void MaterialEditor::DrawMaterial(uint64_t& matRef)
{
	if (ImGui::TreeNodeEx(AssetManager::m_MaterialMap[matRef].m_AssetName.c_str(), 0))
	{
		//ImGui::InputText("")
		if (ImGui::Button("Reload"))
		{
			if (!AssetManager::m_MaterialMap[matRef].m_VertexShader.ReloadShader("main", "vs_5_0"))
			{
				OutputWindow::AddLog(LogType::DANGER,AssetManager::m_MaterialMap[matRef].m_VertexShader.GetErrorMessage().c_str());
			}
			if (!AssetManager::m_MaterialMap[matRef].m_PixelShader.ReloadShader("main", "ps_5_0"))
			{
				OutputWindow::AddLog(LogType::DANGER,AssetManager::m_MaterialMap[matRef].m_PixelShader.GetErrorMessage().c_str());
			}
		}
		for (size_t i = 0; i < AssetManager::m_MaterialMap[matRef].m_VertexShader.v_ShaderBindTexture.size(); i++)
		{
			ImGui::Text(AssetManager::m_MaterialMap[matRef].m_VertexShader.v_ShaderBindTexture[i].name);
		}
		for (size_t i = 0; i < AssetManager::m_MaterialMap[matRef].m_PixelShader.v_ShaderBindTexture.size(); i++)
		{
			ImGui::Text(AssetManager::m_MaterialMap[matRef].m_PixelShader.v_ShaderBindTexture[i].name);
		}

		const char* items[] = { "Opaque","Transparent" };
		 ImGui::Combo("Rendering mode ", &AssetManager::m_MaterialMap[matRef].RenderMode, items,ARRAYSIZE(items));
		 ImGui::DragFloat("Rougness", &AssetManager::m_MaterialMap[matRef].m_PBRBUFFER.Roughness, 0.01, 0, 1);
		 ImGui::DragFloat("Metalic", &AssetManager::m_MaterialMap[matRef].m_PBRBUFFER.Metalic, 0.01, 0, 1);
		 RenderMaterial(matRef);
		 ImGui::Image(
			 m_previewTexture.GetRessourceView()
			 , { m_previewTexture.GetImageX(),m_previewTexture.GetImageY() });





		 for (size_t i = 0, texture_idx = 0; i < AssetManager::m_MaterialMap[matRef].m_testToken.size(); i++)
		 {


			 //TODO REFAIRE CA CORRECTEMENT
			 switch (AssetManager::m_MaterialMap[matRef].m_testToken[i].type)
			 {
			 case TokenType::FLOAT4:
				 break;

			 case TokenType::FLOAT3:
				 break;

			 case TokenType::FLOAT2:
				 break;

			 case TokenType::FLOAT:
				 break;

			 case TokenType::INT4:
				 break;

			 case TokenType::INT3:
				 break;

			 case TokenType::INT2:
				 break;

			 case TokenType::INT:
				 break;

			 case TokenType::COLOR:
				 break;

			 case TokenType::TEXTURE2D:
			 {
				 std::string label = "##" + AssetManager::m_MaterialMap[matRef].m_testToken[i].m_content;
				 ImGui::Columns(2, 0, false);
				 ImGui::Image(AssetManager::m_MaterialMap[matRef].m_Textures[texture_idx].GetRessourceView(), { 50,50 });
				 ImGui::SameLine();
				 ImGui::Text(AssetManager::m_MaterialMap[matRef].m_testToken[i].m_content.c_str());
				 ImGui::NextColumn();
				 ImGui::InputText(label.c_str(), AssetManager::m_MaterialMap[matRef].m_Textures[texture_idx].m_AssetName.data(), AssetManager::m_MaterialMap[matRef].m_Textures[texture_idx].m_AssetName.size(), ImGuiInputTextFlags_AllowTabInput | ImGuiInputTextFlags_ReadOnly);

				 if (ImGui::BeginDragDropTarget())
				 {
					 if (auto payload = ImGui::AcceptDragDropPayload("ASSET", ImGuiInputTextFlags_ReadOnly))
					 {
						 const char* str = (const char*)payload->Data;

						 //
						 AssetManager::m_MaterialMap[matRef].m_Textures[texture_idx] = AssetManager::GetOrCreateTexture(str);

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

				 texture_idx++;
				 break;
			 }
			 default:
				 break;
			 }




		 }

		DrawTextureTree(matRef);
		ImGui::TreePop();
	}
}

void MaterialEditor::DrawTextureTree(uint64_t& matRef)
{
	ImGui::Columns(2, 0, false);

	float color[] = { AssetManager::m_MaterialMap[matRef].m_PBRBUFFER.AlbedoTint.x ,AssetManager::m_MaterialMap[matRef].m_PBRBUFFER.AlbedoTint.y,AssetManager::m_MaterialMap[matRef].m_PBRBUFFER.AlbedoTint.z };
	ImGui::SameLine();
	ImGui::ColorEdit3("Color", color, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);
	AssetManager::m_MaterialMap[matRef].m_PBRBUFFER.AlbedoTint.x = color[0];
	AssetManager::m_MaterialMap[matRef].m_PBRBUFFER.AlbedoTint.y = color[1];
	AssetManager::m_MaterialMap[matRef].m_PBRBUFFER.AlbedoTint.z = color[2];
	
	ImGui::Columns();

;
	ImGui::Checkbox("Use MetalMap", &AssetManager::m_MaterialMap[matRef].m_PBRBUFFER.UseMetalMap);
	if (AssetManager::m_MaterialMap[matRef].m_PBRBUFFER.UseMetalMap)
	{
		ImGui::Columns(2, 0, false);

		ImGui::Text("Metal");
		ImGui::SameLine();
		ImGui::NextColumn();

		ImGui::InputText("##Metalmap", AssetManager::m_MaterialMap[matRef].m_metalicMap.m_AssetName.data(), AssetManager::m_MaterialMap[matRef].m_metalicMap.m_AssetName.size(), ImGuiInputTextFlags_AllowTabInput | ImGuiInputTextFlags_ReadOnly);
		if (ImGui::BeginDragDropTarget())
		{
			if (auto payload = ImGui::AcceptDragDropPayload("ASSET", ImGuiInputTextFlags_ReadOnly))
			{
				const char* str = (const char*)payload->Data;
				AssetManager::m_MaterialMap[matRef].m_metalicMap.m_AssetName = str;
				AssetManager::m_MaterialMap[matRef].SetMetalicMap(AssetManager::m_MaterialMap[matRef].m_metalicMap.m_AssetName);
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

	ImGui::Checkbox("Use Emission", &AssetManager::m_MaterialMap[matRef].m_PBRBUFFER.UseEmisive);
	if (AssetManager::m_MaterialMap[matRef].m_PBRBUFFER.UseEmisive)
	{
		ImGui::Columns(2, 0, false);
		ImGui::Image(AssetManager::m_MaterialMap[matRef].m_EmissiveMap.GetRessourceView(), { 50,50 });
		ImGui::SameLine();
		ImGui::Text("Emisive");

		ImGui::NextColumn();

		ImGui::InputText("##Emisive", AssetManager::m_MaterialMap[matRef].m_EmissiveMap.m_AssetName.data(), AssetManager::m_MaterialMap[matRef].m_EmissiveMap.m_AssetName.size(), ImGuiInputTextFlags_AllowTabInput | ImGuiInputTextFlags_ReadOnly);
		if (ImGui::BeginDragDropTarget())
		{
			if (auto payload = ImGui::AcceptDragDropPayload("ASSET", ImGuiInputTextFlags_ReadOnly))
			{
				const char* str = (const char*)payload->Data;
				AssetManager::m_MaterialMap[matRef].m_EmissiveMap.m_AssetName = str;
				AssetManager::m_MaterialMap[matRef].SetEmisiveMap(AssetManager::m_MaterialMap[matRef].m_EmissiveMap.m_AssetName);
				Armageddon::Log::GetLogger()->trace(str);
			}

			ImGui::EndDragDropTarget();

		}
		ImGui::SameLine();
		if (ImGui::Button("##5...", { 30.0f,20.0f }))
		{

		}

		float color[] = { AssetManager::m_MaterialMap[matRef].m_PBRBUFFER.EmisiveTint.x ,AssetManager::m_MaterialMap[matRef].m_PBRBUFFER.EmisiveTint.y,AssetManager::m_MaterialMap[matRef].m_PBRBUFFER.EmisiveTint.z };
		ImGui::ColorEdit3("Tint", color, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);
		AssetManager::m_MaterialMap[matRef].m_PBRBUFFER.EmisiveTint.x = color[0];
		AssetManager::m_MaterialMap[matRef].m_PBRBUFFER.EmisiveTint.y = color[1];
		AssetManager::m_MaterialMap[matRef].m_PBRBUFFER.EmisiveTint.z = color[2];

		ImGui::DragFloat("Emisive Scale", &AssetManager::m_MaterialMap[matRef].m_PBRBUFFER.EmisiveFactor, 0.1f, 0.0f, 20.0f);
		ImGui::Columns();
	}

	
}

void MaterialEditor::RenderMaterial(uint64_t& matRef)
{
	ID3D11ShaderResourceView* null[] = { nullptr, nullptr , nullptr , nullptr , nullptr , nullptr , nullptr , nullptr , nullptr , nullptr };
	Armageddon::Interface::GetDeviceContext()->PSSetShaderResources(0, 10, null);
	Armageddon::Interface::GetDeviceContext()->OMSetRenderTargets(1, &m_previewTexture.RenderTargetView ,nullptr);	
	float color[] = { 0.1f,0.8f,0.1f,1.0f };
	Armageddon::Interface::GetDeviceContext()->ClearRenderTargetView(m_previewTexture.RenderTargetView, color);


	// Render Sphere With Current Material applied;

	AssetManager::m_MaterialMap[matRef].BindRessources();
	AssetManager::m_MaterialMap[matRef].BindShaders();
	m_previewSphere.UpdtateTransform(&Armageddon::Application::GetApplicationInsatnce()->GetWindow()->GetRenderer().m_camera);

	Armageddon::Renderer::g_TransformCBuffer.SetDynamicData(m_previewSphere.GetTransform());
	Armageddon::Renderer::g_TransformCBuffer.BindPS();
	Armageddon::Renderer::g_TransformCBuffer.BindVS();
	for (auto& submesh : m_previewSphere.v_SubMeshes)
	{
		submesh.BindVertexBuffer();
		submesh.BindIndexBuffer();
		submesh.DrawIndexed();
	}


}
