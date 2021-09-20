#include "Serializer.h"
#include "Entity.h"
#include "../Renderer/Renderer2D.h"

void Serializer::SerializeMaterial(const std::filesystem::path& FilePath, Armageddon::Material& mat)
{
	YAML::Emitter emitter;
	emitter << YAML::BeginMap;

	emitter << YAML::Key << "MATname";
	emitter << YAML::Value << mat.m_name;
	
	emitter << YAML::Key << "PixelShader";
	emitter << YAML::Value << mat.m_PixelShader.ShaderPath.string();

	emitter << YAML::Key << "VertexShader";
	emitter << YAML::Value << mat.m_VertexShader.ShaderPath.string();

	emitter << YAML::Key << "AlbedoMap";
	emitter << YAML::Value << mat.m_albedoMap.TexturePath.string();

	emitter << YAML::Key << "NormalMap";
	emitter << YAML::Value << mat.m_normalMap.TexturePath.string();

	emitter << YAML::Key << "SpecMap";
	emitter << YAML::Value << mat.m_specularMap.TexturePath.string();

	emitter << YAML::Key << "AOMap";
	emitter << YAML::Value << mat.m_ambiantOcclusionMap.TexturePath.string();

	emitter << YAML::Key << "MetalMap";
	emitter << YAML::Value << mat.m_metalicMap.TexturePath.string();

	std::ofstream fileOut(FilePath);
	fileOut << emitter.c_str();
}
void Serializer::SerializeScene(const std::filesystem::path& FilePath,Entity& ent, entt::registry& ere)
{
	
	YAML::Emitter emitter;
	//emitter.SetMapFormat(YAML::Flow);
	emitter.SetIndent(4);

	emitter << YAML::BeginMap;
	emitter << YAML::Key << "Scene" << YAML::Value << FilePath.string().c_str() << YAML::Newline;;
	emitter << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;
	for (auto& entity : m_Scene->v_Entity)
	{
		emitter << YAML::BeginMap;
		emitter << YAML::Key << "Entity";
		emitter << YAML::Value << (int)entity.GetHandle() << YAML::Newline;
		if (entity.HasComponent<TagComponent>())
		{
			auto& Component = entity.GetComponent<TagComponent>();
			emitter << YAML::Key << "Tag";
			emitter << YAML::Value << Component.Tag << YAML::Newline;
		}


		if (entity.HasComponent<MeshComponent>())
		{	
			Armageddon::Log::GetLogger()->trace("CA AMMMASAS");
			auto& Component = entity.GetComponent<MeshComponent>();
			emitter << YAML::Key << "Mesh";
			emitter << YAML::Value << Component.m_path.c_str() << YAML::Newline;

		}

		if (entity.HasComponent<LightComponent>())
		{
			auto& Component = entity.GetComponent<LightComponent>();
			emitter << YAML::Key << "Light";
			emitter << YAML::BeginMap;

			emitter << YAML::Key << "Type" << YAML::Value << Component.type;
			if (Component.type == 0)
			{
				emitter << YAML::Key << "Color";
				emitter << YAML::Flow;
				emitter << YAML::Value << YAML::BeginSeq << Component.m_pointLight.Color.x << Component.m_pointLight.Color.y << Component.m_pointLight.Color.z << YAML::EndSeq;
				
				emitter << YAML::Key << "Position";
				emitter << YAML::Flow;
				emitter << YAML::Value << YAML::BeginSeq << Component.m_pointLight.Position.x << Component.m_pointLight.Position.y << Component.m_pointLight.Position.z << YAML::EndSeq;
				
				emitter << YAML::Key << "Intensity" << YAML::Value << Component.m_pointLight.Intensity;
				emitter << YAML::Key << "Radius" << YAML::Value << Component.m_pointLight.Radius;

				

			}
			else if (Component.type == 1)
			{
					emitter << YAML::Key << "Color";
					emitter << YAML::Flow;
					emitter << YAML::Value << YAML::BeginSeq << Component.m_directionalLight.Color.x << Component.m_directionalLight.Color.y << Component.m_directionalLight.Color.z << YAML::EndSeq;

					emitter << YAML::Key << "Direction";
					emitter << YAML::Flow;
					emitter << YAML::Value << YAML::BeginSeq << Component.m_directionalLight.Direction.x << Component.m_directionalLight.Direction.y << Component.m_directionalLight.Direction.z << YAML::EndSeq;

					emitter << YAML::Key << "Intensity" << YAML::Value << Component.m_directionalLight.Intensity;
			}

			emitter << YAML::EndMap;

		}
		if (entity.HasComponent<TransformComponent>())
		{
			auto& Component = entity.GetComponent<TransformComponent>();
			emitter << YAML::Key << "Transform";
			emitter << YAML::Flow;
			emitter << YAML::Value << YAML::Newline  << YAML::BeginSeq
				<< Component.Translation.x << Component.Translation.y << Component.Translation.z << YAML::Newline 
				<< Component.Rotation.x << Component.Rotation.y << Component.Rotation.z << YAML::Newline
				<< Component.Scale.x << Component.Scale.y << Component.Scale.z  << YAML::EndSeq << YAML::Newline;

		}
		emitter <<  YAML::EndMap;

	}

	emitter << YAML::EndSeq;
	emitter << YAML::EndMap;

	std::ofstream fileOut(FilePath);
	fileOut << emitter.c_str();

}
void Serializer::DeserializeScene(const std::filesystem::path& FilePath)
{
	std::ifstream stream(FilePath);
	std::stringstream m_stringStream;
	m_stringStream << stream.rdbuf();
	YAML::Node node = YAML::Load(m_stringStream);

	if (node["Scene"])
	{
		auto entities = node["Entities"];
		for (auto ent : entities)
		{
			
			if (ent["Entity"]) {
				Armageddon::Log::GetLogger()->trace("Entity {0}", ent["Entity"].as<std::string>().c_str());
				Armageddon::Log::GetLogger()->trace("Tag {0}", ent["Tag"].as<std::string>().c_str());
				auto n_entity = m_Scene->CreateEntity();
				n_entity.AddComponent<TagComponent>(ent["Tag"].as<std::string>());
				if (ent["Mesh"] && ent["Mesh"].as<std::string>() != "")
				{
					Armageddon::Log::GetLogger()->trace("Mesh {0}", ent["Mesh"].as<std::string>().c_str());
					n_entity.AddComponent<MeshComponent>(ent["Mesh"].as<std::string>());
				}

				if (ent["Light"])
				{
					if (ent["Light"]["Type"].as<int>() == 0)
					{
						n_entity.AddComponent<LightComponent>();
						auto& component = n_entity.GetComponent<LightComponent>();
						component.type = 0;
						auto color = ent["Light"]["Color"];
						auto Position = ent["Light"]["Position"];
						component.m_pointLight.Color    = { color[0].as<float>() , color[1].as<float>() ,color[2].as<float>() };
						component.m_pointLight.Position = { Position[0].as<float>() , Position[1].as<float>() ,Position[2].as<float>() };
						component.m_pointLight.Intensity = ent["Light"]["Intensity"].as<float>();
						component.m_pointLight.Radius = ent["Light"]["Radius"].as<float>();

						n_entity.AddComponent<MeshComponent>(Armageddon::Renderer2D::GeneratePlane());
						Armageddon::Renderer::g_PointLightsVector.push_back(component.m_pointLight);
						auto& MeshComp = n_entity.GetComponent<MeshComponent>();
						MeshComp.m_mesh.v_Materials[0].SetVertexShader(L"..\\bin\\Debug-x64\\Armageddon 2.0\\BillBoardVertex.cso");
						MeshComp.m_mesh.v_Materials[0].SetPixelShader(L"..\\bin\\Debug-x64\\Armageddon 2.0\\BillBoardPixel.cso");
						MeshComp.m_mesh.v_Materials[0].SetAlbedoMap(L"Ressources//Icones//Editor//icone_point_light.png");
						MeshComp.m_mesh.v_Materials[0].RenderMode = 1;
						MeshComp.ShowComponent = false;

					}
					else
					{
						n_entity.AddComponent<LightComponent>();
						auto& component = n_entity.GetComponent<LightComponent>();
						component.type = 1;
						auto color = ent["Light"]["Color"];
						auto Direction = ent["Light"]["Direction"];
						component.m_directionalLight.Color = { color[0].as<float>() , color[1].as<float>() ,color[2].as<float>() };
						component.m_directionalLight.Direction = { Direction[0].as<float>() , Direction[1].as<float>() ,Direction[2].as<float>() };
						component.m_directionalLight.Intensity = ent["Light"]["Intensity"].as<float>();

						n_entity.AddComponent<MeshComponent>(Armageddon::Renderer2D::GeneratePlane());
						Armageddon::Renderer::g_DirectLightsVector.push_back(component.m_directionalLight);
						auto& MeshComp = n_entity.GetComponent<MeshComponent>();
						MeshComp.m_mesh.v_Materials[0].SetVertexShader(L"..\\bin\\Debug-x64\\Armageddon 2.0\\BillBoardVertex.cso");
						MeshComp.m_mesh.v_Materials[0].SetPixelShader(L"..\\bin\\Debug-x64\\Armageddon 2.0\\BillBoardPixel.cso");
						MeshComp.m_mesh.v_Materials[0].SetAlbedoMap(L"Ressources//Icones//Editor//icone_point_light.png");
						MeshComp.m_mesh.v_Materials[0].RenderMode = 1;
						MeshComp.ShowComponent = false;
					}
				}

				if (ent["Transform"])
				{
					auto a = ent["Transform"];
					Armageddon::Log::GetLogger()->trace("Transform {0} {1} {2}", a[0].as<std::string>().c_str(), a[1].as<std::string>().c_str(), a[2].as<std::string>().c_str());
					Armageddon::Log::GetLogger()->trace("Rotation {0} {1} {2}" , a[3].as<std::string>().c_str(), a[4].as<std::string>().c_str(), a[5].as<std::string>().c_str());
					Armageddon::Log::GetLogger()->trace("Scale {0} {1} {2}"    , a[6].as<std::string>().c_str(), a[7].as<std::string>().c_str(), a[8].as<std::string>().c_str());
					n_entity.AddComponent<TransformComponent>(a[0].as<float>(), a[1].as<float>(), a[2].as<float>() ,
															  a[3].as<float>(), a[4].as<float>(), a[5].as<float>(),
															  a[6].as<float>(), a[7].as<float>(), a[8].as<float>());
				}
				
			}
		}
	}


	
}
Armageddon::Material Serializer::DeserializeMaterial(const std::filesystem::path& FilePath)
{
	std::ifstream stream(FilePath);
	std::stringstream m_stringStream;
	m_stringStream << stream.rdbuf();
	YAML::Node node = YAML::Load(m_stringStream);
	if (node["MATname"]) 
	{
		if (!AssetManager::MaterialExist(node["MATname"].as<std::string>())) {
			auto mat = Armageddon::Material(node["MATname"].as<std::string>());
			if (node["PixelShader"]) mat.m_PixelShader = AssetManager::GetOrCreatePixelShader(node["PixelShader"].as<std::string>());
			if (node["VertexShader"]) mat.m_VertexShader = AssetManager::GetOrCreateVertexShader(node["VertexShader"].as<std::string>());
			if (node["AlbedoMap"]) mat.m_albedoMap = AssetManager::GetOrCreateTexture(node["AlbedoMap"].as<std::string>());
			if (node["NormalMap"]) mat.m_normalMap = AssetManager::GetOrCreateTexture(node["NormalMap"].as<std::string>());
			if (node["SpecMap"]) mat.m_specularMap = AssetManager::GetOrCreateTexture(node["SpecMap"].as<std::string>());
			if (node["AOMap"]) mat.m_ambiantOcclusionMap = AssetManager::GetOrCreateTexture(node["AOMap"].as<std::string>());
			if (node["MetalMap"]) mat.m_metalicMap = AssetManager::GetOrCreateTexture(node["MetalMap"].as<std::string>());
			return mat;
		}
		else
		{
			return AssetManager::GetOrCreateMaterial(node["MATname"].as<std::string>());
		}
	}

	return Armageddon::Material();
}
