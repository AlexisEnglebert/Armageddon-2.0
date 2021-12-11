#include "Serializer.h"
#include "Entity.h"
#include "../Renderer/Renderer2D.h"

void Serializer::SerializeMaterial(const std::filesystem::path& FilePath, Material& mat)
{
	YAML::Emitter emitter;
	emitter << YAML::BeginMap;

	emitter << YAML::Key << "Name" << YAML::Value << mat.m_AssetName;
	emitter << YAML::Key << "PixelShader" << YAML::Value << mat.m_PixelShader.ShaderPath.string();
	emitter << YAML::Key << "VertexShader" << YAML::Value << mat.m_VertexShader.ShaderPath.string();

	/*emitter << YAML::Key << "Materials" << YAML::Value << YAML::BeginSeq;

	for (UINT i = 0; i < mat.m_MaterialProperty.m_VTexure.size(); i++)
	{
		emitter << YAML::BeginMap;

		emitter << YAML::Key << "Name" << YAML::Value << mat.m_MaterialProperty.m_VTexure[i].m_name;
		emitter << YAML::Key << "ID" << YAML::Value << mat.m_MaterialProperty.m_VTexure[i].ID;
		emitter << YAML::Key << "Texture" << mat.m_MaterialProperty.m_VTexure[i].m_Texture.m_AssetName.c_str();
		emitter << YAML::EndMap;

	}*/
	emitter << YAML::Key << "AlbedoMap" << YAML::Value << mat.m_albedoMap.m_AssetName.c_str();

	emitter << YAML::Key << "NormalMap";
	emitter << YAML::Value << mat.m_normalMap.m_AssetName.c_str();

	emitter << YAML::Key << "SpecMap";
	emitter << YAML::Value << mat.m_specularMap.m_AssetName.c_str();

	emitter << YAML::Key << "AOMap";
	emitter << YAML::Value << mat.m_ambiantOcclusionMap.m_AssetName.c_str();

	emitter << YAML::Key << "UseMetal";
	emitter << YAML::Value << mat.m_PBRBUFFER.UseMetalMap;
	if (mat.m_PBRBUFFER.UseMetalMap)
	{
		emitter << YAML::Key << "MetalMap";
		emitter << YAML::Value << mat.m_metalicMap.m_AssetName.c_str();
	}
	emitter << YAML::Key << "UseEmisive";
	emitter << YAML::Value << mat.m_PBRBUFFER.UseEmisive;
	if (mat.m_PBRBUFFER.UseEmisive)
	{
		emitter << YAML::Key << "EmisiveMap";
		emitter << YAML::Value << mat.m_metalicMap.m_AssetName.c_str();
	}

	emitter << YAML::EndMap;
	std::ofstream fileOut(FilePath);
	fileOut << emitter.c_str();
}
void Serializer::SerializeScene(const std::filesystem::path& FilePath,Entity& ent, entt::registry& ere)
{
	
	YAML::Emitter emitter;
	//emitter.SetMapFormat(YAML::Flow);
	emitter.SetIndent(4);

	emitter << YAML::BeginMap;
	SerializeRessourceMap(FilePath.string() + (".aressource"));
	emitter << YAML::Key << "Ressources" << YAML::Value  << FilePath.string() + (".aressource");

	emitter << YAML::Key << "Scene" << YAML::Value << FilePath.string().c_str() << YAML::Newline;
	emitter << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;
	for (auto entity = m_Scene->EntityMap.begin(); entity != m_Scene->EntityMap.cend(); entity++)
	{
		emitter << YAML::BeginMap;
		emitter << YAML::Key << "Entity";
		emitter << YAML::Value << (int)entity->second.GetHandle() << YAML::Newline;
		if (entity->second.HasComponent<TagComponent>())
		{
			auto& Component = entity->second.GetComponent<TagComponent>();
			emitter << YAML::Key << "Tag";
			emitter << YAML::Value << Component.Tag << YAML::Newline;
		}
		if (entity->second.HasComponent<ScriptComponent>())
		{
			auto& Component = entity->second.GetComponent<ScriptComponent>();
			emitter << YAML::Key << "Script";
			emitter << YAML::Value << Component.moduleName.c_str();
		}

		if (entity->second.HasComponent<MeshComponent>())
		{	
			auto& Component = entity->second.GetComponent<MeshComponent>();
			emitter << YAML::Key << "Mesh";
			emitter << YAML::Value << Component.m_path.c_str();
			emitter << YAML::Key << "Materials";
			emitter << YAML::Value<<  YAML::BeginSeq;
			for (auto& mesh : Component.m_mesh.v_SubMeshes)
			{
				SerializeMaterial("Assets/Materials/"+AssetManager::m_MaterialMap[Component.m_mesh.v_MaterialReference[mesh.m_materialIndex]].m_AssetName + ".mat", AssetManager::m_MaterialMap[Component.m_mesh.v_MaterialReference[mesh.m_materialIndex]]);
				emitter << YAML::Key << mesh.m_materialIndex << YAML::Value << YAML::Hex  << Component.m_mesh.v_MaterialReference[mesh.m_materialIndex] << YAML::Newline;
			}
			emitter << YAML::EndSeq;

		}

		if (entity->second.HasComponent<LightComponent>())
		{
			auto& Component = entity->second.GetComponent<LightComponent>();
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
		if (entity->second.HasComponent<TransformComponent>())
		{
			auto& Component = entity->second.GetComponent<TransformComponent>();
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
void Serializer::SerializeRessourceMap(const std::filesystem::path& FilePath)
{
	//Material Asset
	std::ofstream fileOut(FilePath);
	fileOut << "Hash , Name \n";
	for (auto iterator = AssetManager::m_MaterialMap.begin(); iterator != AssetManager::m_MaterialMap.cend(); iterator++)
	{
		fileOut << iterator->first << "," << iterator->second.m_AssetName <<"\n";
	}
	fileOut.close();


}
void Serializer::DeserializeRessourceMap(const std::filesystem::path& Filepath)
{
 	std::ifstream stream(Filepath);
	//std::stringstream m_stringStream;
	//m_stringStream << stream.rdbuf();
	if (!stream.is_open()){Armageddon::Log::GetLogger()->error("CAN NOT OPEN THE FILE");}
	std::string line, colname;
	std::getline(stream, line);

	while (std::getline(stream, line))
	{
		  
		std::stringstream ss(line);
		std::string Name;
		std::string Hash;
		std::getline(ss, Hash, ',');
		std::getline(ss, Name, ',');
		DeserializeMaterial("Assets/Materials/"+Name+".mat");

		
	}

}
void Serializer::DeserializeScene(const std::filesystem::path& FilePath)
{
	std::ifstream stream(FilePath);
	std::stringstream m_stringStream;
	m_stringStream << stream.rdbuf();
	YAML::Node node = YAML::Load(m_stringStream);
	if (node["Ressources"])
	{
		DeserializeRessourceMap(node["Ressources"].as<std::string>());
	}
	if (node["Scene"])
	{
		auto entities = node["Entities"];
		for (auto ent : entities)
		{
			
			if (ent["Entity"]) {
				//Armageddon::Log::GetLogger()->trace("Entity {0}", ent["Entity"].as<std::string>().c_str());
				///Armageddon::Log::GetLogger()->trace("Tag {0}", ent["Tag"].as<std::string>().c_str());
				auto n_entity = m_Scene->CreateEntity();
				n_entity.AddComponent<TagComponent>(ent["Tag"].as<std::string>());
				if (ent["Mesh"] && ent["Mesh"].as<std::string>() != "")
				{
					//Armageddon::Log::GetLogger()->trace("Mesh {0}", ent["Mesh"].as<std::string>().c_str());
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
						//MeshComp.m_mesh.v_Materials[0].SetVertexShader(L"..\\bin\\Debug-x64\\Armageddon 2.0\\BillBoardVertex.cso");
						//MeshComp.m_mesh.v_Materials[0].SetPixelShader(L"..\\bin\\Debug-x64\\Armageddon 2.0\\BillBoardPixel.cso");
						//MeshComp.m_mesh.v_Materials[0].SetAlbedoMap(L"Ressources//Icones//Editor//icone_point_light.png");
						//MeshComp.m_mesh.v_Materials[0].RenderMode = 1;
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
					//	MeshComp.m_mesh.v_Materials[0].SetVertexShader(L"..\\bin\\Debug-x64\\Armageddon 2.0\\BillBoardVertex.cso");
						//MeshComp.m_mesh.v_Materials[0].SetPixelShader(L"..\\bin\\Debug-x64\\Armageddon 2.0\\BillBoardPixel.cso");
						//MeshComp.m_mesh.v_Materials[0].SetAlbedoMap(L"Ressources//Icones//Editor//icone_point_light.png");
						//MeshComp.m_mesh.v_Materials[0].RenderMode = 1;
						MeshComp.ShowComponent = false;
					}
				}
				if (ent["Script"])
				{
					auto a = ent["Script"];
					n_entity.AddComponent<ScriptComponent>(a.as<std::string>());
				}
				if (ent["Transform"])
				{
					auto a = ent["Transform"];
					//Armageddon::Log::GetLogger()->trace("Transform {0} {1} {2}", a[0].as<std::string>().c_str(), a[1].as<std::string>().c_str(), a[2].as<std::string>().c_str());
					//Armageddon::Log::GetLogger()->trace("Rotation {0} {1} {2}" , a[3].as<std::string>().c_str(), a[4].as<std::string>().c_str(), a[5].as<std::string>().c_str());
					//Armageddon::Log::GetLogger()->trace("Scale {0} {1} {2}"    , a[6].as<std::string>().c_str(), a[7].as<std::string>().c_str(), a[8].as<std::string>().c_str());
					n_entity.AddComponent<TransformComponent>(a[0].as<float>(), a[1].as<float>(), a[2].as<float>() ,
															  a[3].as<float>(), a[4].as<float>(), a[5].as<float>(),
															  a[6].as<float>(), a[7].as<float>(), a[8].as<float>());
				}
				
			}
		}
	}


	
}
uint64_t Serializer::DeserializeMaterial(const std::filesystem::path& FilePath)
{
	std::ifstream stream(FilePath);
	std::stringstream m_stringStream;
	if (stream.is_open()) {
		m_stringStream << stream.rdbuf();
		YAML::Node node = YAML::Load(m_stringStream);



		std::string MatName = FilePath.stem().string();
		Material mat(MatName);
		auto Hash = HashUtils::_64BitHash(MatName);

		if (node["Name"])
		{
					for (auto material : node["Materials"])
					{
						//if (material["Name"]) { Armageddon::Log::GetLogger()->trace("Texture Load From material : {0}", node["Name"].as<std::string>()); }
						
					}


				if (node["PixelShader"]) AssetManager::m_MaterialMap[Hash].m_PixelShader = AssetManager::GetOrCreatePixelShader(node["PixelShader"].as<std::string>());
				if (node["VertexShader"]) AssetManager::m_MaterialMap[Hash].m_VertexShader = AssetManager::GetOrCreateVertexShader(node["VertexShader"].as<std::string>());
				if (node["AlbedoMap"])
				{
					Armageddon::Log::GetLogger()->trace(MatName.c_str());
					//	std::string test = material["AlbedoMap"].as<std::string>();
					AssetManager::m_MaterialMap[Hash].m_albedoMap = AssetManager::GetOrCreateTexture(node["AlbedoMap"].as<std::string>() );
				}
				if (node["NormalMap"]) AssetManager::m_MaterialMap[Hash].m_normalMap = AssetManager::GetOrCreateTexture(node["NormalMap"].as<std::string>());
				if (node["SpecMap"]) AssetManager::m_MaterialMap[Hash].m_specularMap = AssetManager::GetOrCreateTexture(node["SpecMap"].as<std::string>());
				if (node["AOMap"]) AssetManager::m_MaterialMap[Hash].m_ambiantOcclusionMap = AssetManager::GetOrCreateTexture(node["AOMap"].as<std::string>());
				if (node["MetalMap"]) AssetManager::m_MaterialMap[Hash].m_metalicMap = AssetManager::GetOrCreateTexture(node["MetalMap"].as<std::string>());

			
			
		}

		return Hash;
	}
	return 0;
}
