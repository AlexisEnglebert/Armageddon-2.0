#include "EntityProperties.h"
#include "EntityList.h"

void EntityProperties::ImGuiDraw()
{
	ImGui::Begin("Entity Properties");
	if (EntityList::Seleceted != entt::null)
	{
		auto entity = m_Scene.GetEntityByID(EntityList::Seleceted);
		
		
		if(ImGui::Button("AddComponent"))
			ImGui::OpenPopup("add_component");
		if (ImGui::BeginPopup("add_component"))
		{

			if (ImGui::MenuItem("MeshComponent") &&! entity.HasComponent<MeshComponent>())
			{
				entity.AddComponent<MeshComponent>();
			}
			if (ImGui::MenuItem("MaterialComponent") && !entity.HasComponent<MaterialComponent>())
			{
				entity.AddComponent<MaterialComponent>();
			}
			if (ImGui::MenuItem("TransformComponent") && !entity.HasComponent<TransformComponent>())
			{
				entity.AddComponent<TransformComponent>();

			}
			if (ImGui::MenuItem("LightComponent") && !entity.HasComponent<LightComponent>())
			{
				entity.AddComponent<LightComponent>();
				Armageddon::Renderer::g_PointLightsVector.push_back(entity.GetComponent<LightComponent>().m_pointLight);
			}
			//ImGui::MenuItem("Sphere");
		
			ImGui::EndPopup();
		}
		if (entity.HasComponent<TagComponent>())
		{
			DrawTagComponent(entity);
		}
		if (entity.HasComponent<MeshComponent>())
		{
			auto& component = entity.GetComponent<MeshComponent>();
			if(component.ShowComponent)
				DrawMeshComponent(entity);
			
		}
		if (entity.HasComponent<TransformComponent>())
		{
			DrawTransformComponent(entity);
		}
		if (entity.HasComponent<LightComponent>())
		{
			DrawLightComponent(entity);
		}
		
	}
	ImGui::End();
}

void EntityProperties::DrawTagComponent(Entity& entity)
{
	ImGui::PushStyleColor(ImGuiCol_Header, { 0.29,0.6,0.66,1 });
	auto& component = entity.GetComponent<TagComponent>();
	if (ImGui::TreeNodeEx("TagComponent",ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Text("Name:");
		ImGui::SameLine();
		ImGui::InputTextWithHint("##EntityName", "Name", component.Tag.data(), component.Tag.size());
		ImGui::TreePop();
	}
	ImGui::PopStyleColor();
}

void EntityProperties::DrawTransformComponent(Entity& entity)
{
	auto& component = entity.GetComponent<TransformComponent>();
	ImGui::PushStyleColor(ImGuiCol_Header, { 0.29,0.6,0.66,1 });
	if (ImGui::TreeNodeEx("TransformComponent", ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { 0,0 });
		ImGui::Columns(2, 0, false);
		ImGui::SetColumnWidth(0, 100.0f);
		ImGui::Text("Translate");
		//ImGui::SameLine();
		ImGui::PushStyleColor(ImGuiCol_Button, { 1.0f,0.3215686275f,0.3215686275f,1.0f });
		ImGui::NextColumn();
		ImGui::Button("X", { 20.0f,20.0f });
		ImGui::PopStyleColor();
		ImGui::SameLine();
		ImGui::PushItemWidth(50);
		ImGui::DragFloat("##X", &component.Translation.x, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, { 0.1333333333f,0.968627451f,0.1607843137,1.0f });
		ImGui::Button("Y", { 20.0f,20.0f });
		ImGui::PopStyleColor();
		ImGui::SameLine();
		ImGui::PushItemWidth(50);

		ImGui::DragFloat("##Y", &component.Translation.y, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();

		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, { 0.1333333333f,0.5764705882f,0.968627451,1.0f });
		ImGui::Button("Z", { 20.0f,20.0f });
		ImGui::PopStyleColor();
		ImGui::SameLine();
		ImGui::PushItemWidth(50);

		ImGui::DragFloat("##Z", &component.Translation.z, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		//ImGui::PopItemWidth();
		ImGui::Columns();

		ImGui::Columns(2, 0, false);
		ImGui::SetColumnWidth(0, 100.0f);
		ImGui::Text("Rotation");
		//	ImGui::SameLine();
		ImGui::NextColumn();
		ImGui::PushStyleColor(ImGuiCol_Button, { 1.0f,0.3215686275f,0.3215686275f,1.0f });
		ImGui::Button("X", { 20.0f,20.0f });
		ImGui::PopStyleColor();
		ImGui::SameLine();
		ImGui::PushItemWidth(50);
		ImGui::DragFloat("###X##", &component.Rotation.x, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();

		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, { 0.1333333333f,0.968627451f,0.1607843137,1.0f });
		ImGui::Button("Y", { 20.0f,20.0f });
		ImGui::PopStyleColor();
		ImGui::SameLine();
		ImGui::PushItemWidth(50);

		ImGui::DragFloat("###Y##", &component.Rotation.y, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();

		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, { 0.1333333333f,0.5764705882f,0.968627451,1.0f });
		ImGui::Button("Z", { 20.0f,20.0f });
		ImGui::PopStyleColor();
		ImGui::SameLine();
		ImGui::PushItemWidth(50);

		ImGui::DragFloat("###Z##", &component.Rotation.z, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::Columns();


		ImGui::Columns(2, 0, false);
		ImGui::SetColumnWidth(0, 100.0f);
		ImGui::Text("Scaling");
		//ImGui::SameLine();
		ImGui::NextColumn();
		ImGui::PushStyleColor(ImGuiCol_Button, { 1.0f,0.3215686275f,0.3215686275f,1.0f });
		ImGui::Button("X", { 20.0f,20.0f });
		ImGui::PopStyleColor();
		ImGui::SameLine();
		ImGui::PushItemWidth(50);
		ImGui::DragFloat("####X", &component.Scale.x, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();

		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, { 0.1333333333f,0.968627451f,0.1607843137,1.0f });
		ImGui::Button("Y", { 20.0f,20.0f });
		ImGui::PopStyleColor();
		ImGui::SameLine();
		ImGui::PushItemWidth(50);

		ImGui::DragFloat("####Y", &component.Scale.y, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();

		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, { 0.1333333333f,0.5764705882f,0.968627451,1.0f });
		ImGui::Button("Z", { 20.0f,20.0f });
		ImGui::PopStyleColor();
		ImGui::SameLine();
		ImGui::PushItemWidth(50);

		ImGui::DragFloat("####Z", &component.Scale.z, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::NewLine();

		ImGui::PopStyleVar();
		ImGui::Columns();

		ImGui::TreePop();

		

	}
	ImGui::PopStyleColor();
}

void EntityProperties::DrawMeshComponent(Entity& entity)
{
	auto& component = entity.GetComponent<MeshComponent>();
	ImGui::PushStyleColor(ImGuiCol_Header, { 0.29,0.6,0.66,1 });
	if (ImGui::TreeNodeEx("MeshComponent", ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Selected))
	{
		const char* items[] = { "Primitives","Model" };

		ImGui::Text("Type : ");
		ImGui::SameLine();
		ImGui::Combo("##Type", &component.m_type, items, ARRAYSIZE(items));
		if (component.m_type == 1)
		{
			ImGui::Text("Model :");
			ImGui::SameLine();
			ImGui::InputTextWithHint("##Path", "Path....", component.m_path.data(), component.m_path.size());
			if (ImGui::BeginDragDropTarget())
			{
				if (auto payload = ImGui::AcceptDragDropPayload("ASSET", ImGuiInputTextFlags_ReadOnly))
				{
					const char* str = (const char*)payload->Data;
					component.m_path = str;
					component.m_mesh = Mesh(str);
					Armageddon::Log::GetLogger()->trace(str);
				}
				ImGui::EndDragDropTarget();
			}
			ImGui::SameLine();
			ImGui::Button("...");
		}
		else if (component.m_type == 0)
		{
			const char* PrimitivesItems[] = { "Cube","Sphere","Torus" }; 
			int currentItem = 0;
			ImGui::Text("Primitive:");
			ImGui::SameLine();
			ImGui::Combo("##PrimitiveTtype",&currentItem,PrimitivesItems,ARRAYSIZE(PrimitivesItems));
		}

		if (entity.HasComponent<MaterialComponent>() || component.m_mesh.v_Materials.size() > 0)
		{

			if (ImGui::TreeNodeEx("Materials", ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Selected))
			{
				if (entity.HasComponent<MeshComponent>())
				{
					auto& component = entity.GetComponent<MeshComponent>();
					for (auto& mat : component.m_mesh.v_Materials)
					{
						ImGui::Columns(2);
						ImGui::Text(mat.m_name.c_str());
						ImGui::NextColumn();
						//ImGui::SameLine();
						ImGui::InputText("##Material", mat.m_name.data(), mat.m_name.size() * sizeof(char));
						if (ImGui::BeginDragDropTarget())
						{
							if (auto payload = ImGui::AcceptDragDropPayload("ASSET", ImGuiInputTextFlags_ReadOnly))
							{
								const char* str = (const char*)payload->Data;
								Serializer m_serial;
								auto material = m_serial.DeserializeMaterial(str);
								std::string name = mat.m_name;
								mat = material;
								mat.m_name = name;
								Armageddon::Log::GetLogger()->trace(str);
							}
							ImGui::EndDragDropTarget();
						}
						ImGui::Columns();

					}
				}

				ImGui::TreePop();

			}
		}
		ImGui::TreePop();
	}
	ImGui::PopStyleColor();
	//ImGui::Separator();
}

void EntityProperties::DrawMaterialComponent(Entity& entity)
{
	
	

}

void EntityProperties::DrawLightComponent(Entity& entity)
{
	ImGui::PushStyleColor(ImGuiCol_Header, { 0.29,0.6,0.66,1 });

	auto& component = entity.GetComponent<LightComponent>();
	if (ImGui::TreeNodeEx("LightComponent", ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Selected))
	{
		const char* items[] = { "Point Light","Directional Light" };
		ImGui::Combo("##LightType", &component.type, items, ARRAYSIZE(items));
		if (component.type == 0)
		{

			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { 0,0 });
			ImGui::Columns(2, 0, false);
			ImGui::SetColumnWidth(0, 100.0f);
			ImGui::Text("Position");
			//ImGui::SameLine();
			ImGui::PushStyleColor(ImGuiCol_Button, { 1.0f,0.3215686275f,0.3215686275f,1.0f });
			ImGui::NextColumn();
			ImGui::Button("X", { 20.0f,20.0f });
			ImGui::PopStyleColor();
			ImGui::SameLine();
			ImGui::PushItemWidth(50);
			ImGui::DragFloat("##X", &component.m_pointLight.Position.x, 0.1f, 0.0f, 0.0f, "%.2f");
			ImGui::PopItemWidth();
			ImGui::SameLine();

			ImGui::PushStyleColor(ImGuiCol_Button, { 0.1333333333f,0.968627451f,0.1607843137,1.0f });
			ImGui::Button("Y", { 20.0f,20.0f });
			ImGui::PopStyleColor();
			ImGui::SameLine();
			ImGui::PushItemWidth(50);

			ImGui::DragFloat("##Y", &component.m_pointLight.Position.y, 0.1f, 0.0f, 0.0f, "%.2f");
			ImGui::PopItemWidth();

			ImGui::SameLine();

			ImGui::PushStyleColor(ImGuiCol_Button, { 0.1333333333f,0.5764705882f,0.968627451,1.0f });
			ImGui::Button("Z", { 20.0f,20.0f });
			ImGui::PopStyleColor();
			ImGui::SameLine();
			ImGui::PushItemWidth(50);

			ImGui::DragFloat("##Z", &component.m_pointLight.Position.z, 0.1f, 0.0f, 0.0f, "%.2f");
			ImGui::PopItemWidth();
			//ImGui::PopItemWidth();
			ImGui::Columns();
			ImGui::PopStyleVar();



			float color[] = { component.m_pointLight.Color.x ,component.m_pointLight.Color.y,component.m_pointLight.Color.z };
			ImGui::Text("Color:");
			ImGui::SameLine();
			ImGui::ColorEdit3("Color",color, 0);
			component.m_pointLight.Color.x = color[0];
			component.m_pointLight.Color.y = color[1];
			component.m_pointLight.Color.z = color[2];

			ImGui::Text("Intensity");
			ImGui::SameLine();
			ImGui::DragFloat("##Intensité", &component.m_pointLight.Intensity, 0.5, 0, 500);

			ImGui::Text("Radius");
			ImGui::SameLine();
			ImGui::DragFloat("##Radius", &component.m_pointLight.Radius, 0.5, 0, 500);


		}
		else
		{
			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { 0,0 });
			ImGui::Columns(2, 0, false);
			ImGui::SetColumnWidth(0, 100.0f);
			ImGui::Text("Direction");
			//ImGui::SameLine();
			ImGui::PushStyleColor(ImGuiCol_Button, { 1.0f,0.3215686275f,0.3215686275f,1.0f });
			ImGui::NextColumn();
			ImGui::Button("X", { 20.0f,20.0f });
			ImGui::PopStyleColor();
			ImGui::SameLine();
			ImGui::PushItemWidth(50);
			ImGui::DragFloat("##X", &component.m_directionalLight.Direction.x, 0.1f, 0.0f, 0.0f, "%.2f");
			ImGui::PopItemWidth();
			ImGui::SameLine();

			ImGui::PushStyleColor(ImGuiCol_Button, { 0.1333333333f,0.968627451f,0.1607843137,1.0f });
			ImGui::Button("Y", { 20.0f,20.0f });
			ImGui::PopStyleColor();
			ImGui::SameLine();
			ImGui::PushItemWidth(50);

			ImGui::DragFloat("##Y", &component.m_directionalLight.Direction.y, 0.1f, 0.0f, 0.0f, "%.2f");
			ImGui::PopItemWidth();

			ImGui::SameLine();

			ImGui::PushStyleColor(ImGuiCol_Button, { 0.1333333333f,0.5764705882f,0.968627451,1.0f });
			ImGui::Button("Z", { 20.0f,20.0f });
			ImGui::PopStyleColor();
			ImGui::SameLine();
			ImGui::PushItemWidth(50);

			ImGui::DragFloat("##Z", &component.m_directionalLight.Direction.z, 0.1f, 0.0f, 0.0f, "%.2f");
			ImGui::PopItemWidth();
			ImGui::Columns();
			ImGui::PopStyleVar();


			float color[] = { component.m_directionalLight.Color.x ,component.m_directionalLight.Color.y,component.m_directionalLight.Color.z };
			ImGui::Text("Color:");
			ImGui::SameLine();
			ImGui::ColorEdit3("Color", color, 0);
			component.m_directionalLight.Color.x = color[0];
			component.m_directionalLight.Color.y = color[1];
			component.m_directionalLight.Color.z = color[2];

			ImGui::Text("Intensity");
			ImGui::SameLine();
			ImGui::DragFloat("##Intensité", &component.m_directionalLight.Intensity, 0.5, 0, 500);

				
		}
		ImGui::TreePop();

	}
	ImGui::PopStyleColor();

}
