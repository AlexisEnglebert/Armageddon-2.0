#include "Serializer.h"
#include "Entity.h"
#include "../Renderer/Renderer2D.h"

void Serializer::SerializeMaterial(const std::filesystem::path& FilePath, Material& mat)
{
	std::ofstream fileOut(FilePath);
	//TODO FINISH THIS
	for( int i = 0,TexIDX = 0 ; i < mat.m_testToken.size();i++)
	{ 
		if (mat.m_testToken[i].type == TokenType::ENTRYPOINT)
		{
			std::string result = "main(\"" + mat.m_testToken[i].m_content + "\")\n";
			fileOut << result;
		}
		if (mat.m_testToken[i].type == TokenType::TEXTURE2D)
		{
			std::string result = "Texture2D(\"" + mat.m_testToken[i].m_content + "\")\n{\n\t source = <"+mat.m_Textures[TexIDX].m_AssetName+">\n}\n";
			i+=3;
			TexIDX++;
			fileOut << result;
			continue;
		}
		if (mat.m_testToken[i].type == TokenType::ATTRIBUTE)
		{
			std::string result = mat.m_testToken[i].m_content + " = <" + mat.m_testToken[i].supplement + ">\n";
			fileOut << "\t" << result;

		}
		if (mat.m_testToken[i].type == TokenType::SCOPE)
		{
			fileOut <<mat.m_testToken[i].m_content + "\n";

		}

	}
	return;
	YAML::Emitter emitter;
	emitter << YAML::BeginMap;

	emitter << YAML::Key << "Name" << YAML::Value << mat.m_AssetName;
	emitter << YAML::Key << "PixelShader" << YAML::Value << mat.m_PixelShader.m_shaderPath.string();
	emitter << YAML::Key << "VertexShader" << YAML::Value << mat.m_VertexShader.m_shaderPath.string();

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
			emitter << YAML::Value << Component.ClassName.c_str();
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

		if (entity->second.HasComponent<DirectionalLightComponent>())
		{
			auto& Component = entity->second.GetComponent<DirectionalLightComponent>();
			emitter << YAML::Key << "DirectionalLightComponent";
			emitter << YAML::BeginMap;
			emitter << YAML::Key << "Color";
			emitter << YAML::Flow;
			emitter << YAML::Value << YAML::BeginSeq << Component.m_directionalLight.Color.x << Component.m_directionalLight.Color.y << Component.m_directionalLight.Color.z << YAML::EndSeq;

			emitter << YAML::Key << "Direction";
			emitter << YAML::Flow;
			emitter << YAML::Value << YAML::BeginSeq << Component.m_directionalLight.Direction.x << Component.m_directionalLight.Direction.y << Component.m_directionalLight.Direction.z << YAML::EndSeq;

			emitter << YAML::Key << "Intensity" << YAML::Value << Component.m_directionalLight.Intensity;
			

			emitter << YAML::EndMap;

		}
		if (entity->second.HasComponent<PointLightComponent>())
		{
			auto& Component = entity->second.GetComponent<PointLightComponent>();
			emitter << YAML::Key << "PointLightComponent";
			emitter << YAML::BeginMap;
			emitter << YAML::Key << "Color";
			emitter << YAML::Flow;
			emitter << YAML::Value << YAML::BeginSeq << Component.m_pointLight.Color.x << Component.m_pointLight.Color.y << Component.m_pointLight.Color.z << YAML::EndSeq;

			emitter << YAML::Key << "Position";
			emitter << YAML::Flow;
			emitter << YAML::Value << YAML::BeginSeq << Component.m_pointLight.Position.x << Component.m_pointLight.Position.y << Component.m_pointLight.Position.z << YAML::EndSeq;

			emitter << YAML::Key << "Intensity" << YAML::Value << Component.m_pointLight.Intensity;
			emitter << YAML::Key << "Radius" << YAML::Value << Component.m_pointLight.Radius;
			
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

				if (ent["PointLightComponent"])
				{
					n_entity.AddComponent<PointLightComponent>();
					auto& component = n_entity.GetComponent<PointLightComponent>();
					auto color = ent["PointLightComponent"]["Color"];
					auto Position = ent["PointLightComponent"]["Position"];
					component.m_pointLight.Color = { color[0].as<float>() , color[1].as<float>() ,color[2].as<float>() };
					component.m_pointLight.Position = { Position[0].as<float>() , Position[1].as<float>() ,Position[2].as<float>() };
					component.m_pointLight.Intensity = ent["PointLightComponent"]["Intensity"].as<float>();
					component.m_pointLight.Radius = ent["PointLightComponent"]["Radius"].as<float>();

					n_entity.AddComponent<MeshComponent>(Armageddon::Renderer2D::GeneratePlane());
					Armageddon::Renderer::g_PointLightsVector.push_back(component.m_pointLight);
					auto& MeshComp = n_entity.GetComponent<MeshComponent>();
					MeshComp.m_mesh.v_MaterialReference[0] = AssetManager::GetOrCreateMaterial("LightMaterial");

					MeshComp.ShowComponent = false;

				}
				if (ent["DirectionalLightComponent"])
				{
					n_entity.AddComponent<DirectionalLightComponent>();
					auto& component = n_entity.GetComponent<DirectionalLightComponent>();
					auto color = ent["DirectionalLightComponent"]["Color"];
					auto Direction = ent["DirectionalLightComponent"]["Direction"];
					component.m_directionalLight.Color = { color[0].as<float>() , color[1].as<float>() ,color[2].as<float>() };
					component.m_directionalLight.Direction = { Direction[0].as<float>() , Direction[1].as<float>() ,Direction[2].as<float>() };
					component.m_directionalLight.Intensity = ent["DirectionalLightComponent"]["Intensity"].as<float>();

					n_entity.AddComponent<MeshComponent>(Armageddon::Renderer2D::GeneratePlane());
					Armageddon::Renderer::g_DirectLightsVector.push_back(component.m_directionalLight);
					auto& MeshComp = n_entity.GetComponent<MeshComponent>();
					MeshComp.m_mesh.v_MaterialReference[0] = AssetManager::GetOrCreateMaterial("LightMaterial");


					MeshComp.ShowComponent = false;
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
	Tokenizer m_tokenizer;
	std::vector<Token> tokens = m_tokenizer.Parse(FilePath);
	Material n_material;
	//TODO WITH ASSET MANAGER
	uint64_t Hash= HashUtils::_64BitHash(FilePath.stem().string().c_str());
	if (AssetManager::MaterialExist(FilePath.stem().string().c_str()))
	{
		return 0;
	}

	for (size_t i = 0; i < tokens.size(); i++)
	{
		if (tokens[i].type == TokenType::TEXTURE2D)
		{
			i += 2;
			if (tokens[i].m_content == "source")
			{
				if (tokens[i].supplement.size() > 0)
				{
					if (tokens[i].supplement == "Depth")
					{
						//TODO IMPROVE WITH TEXTURES HASMAP
						n_material.m_Textures.push_back(Armageddon::Application::GetApplicationInsatnce()->GetWindow()->GetRenderer().m_DepthPass);
					}
					else
					{
						n_material.m_Textures.push_back(AssetManager::GetOrCreateTexture(tokens[i].supplement));
					}
				}
				else
				{
					n_material.m_Textures.push_back(Texture());
				}
			}
			else
			{
				n_material.m_Textures.push_back(Texture());
			}
		}
		if (tokens[i].type == TokenType::ENTRYPOINT)
		{
			n_material.m_AssetName = tokens[i].m_content;

			i += 2;
			if (tokens[i].type == TokenType::ATTRIBUTE)
			{
				if (tokens[i].m_content == "ps")
				{
					n_material.m_PixelShader = AssetManager::GetOrCreatePixelShader(tokens[i].supplement);
				}
				i++;
				if (tokens[i].m_content == "vs")
				{
					n_material.m_VertexShader = AssetManager::GetOrCreateVertexShader(tokens[i].supplement);

				}
			}

		}
	}
	n_material.m_testToken = tokens;

	AssetManager::m_AssetMap[Hash] = n_material;
	AssetManager::m_MaterialMap[Hash] = n_material;

	return 0;
}
