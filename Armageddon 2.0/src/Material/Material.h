#pragma once
#include "../Macros.h"
#include "Texture.h"
#include "../Renderer/Shaders.h"
#include "../Renderer/ConstantBufferTypes.h"

#include <map>

#define AG_RENDERMODE_OPAQUE = 0
#define AG_RENDERMODE_TRANSPARENT = 1
namespace Armageddon
{
	/*
	* Par convention dans l'engine , les Property des materials ont un ID , 
	* il faudrait trouver un moyen d'assigner un id automatiquement mais pour
	* l'instant on le fait à la main.
	* 
	* TextureProperty  ->  0 
	* BoolProperty     ->  1 
	* FloatProperty    -> 2
	* Float2Property    -> 3
	* Float3Property    -> 4
	*/
	struct Property
	{
		std::string name;
	};



	struct TextureProperty : Property
	{
		TextureProperty() = default;
	};
	struct BoolProperty : Property
	{
		BoolProperty() = default;

	};
	struct FloatProperty : Property
	{
		FloatProperty() = default;

	};
	struct Float2Property : Property
	{
		Float2Property() = default;

	};
	struct Float3Property : Property
	{
		DirectX::XMFLOAT3 f3 = DirectX::XMFLOAT3(0.0f,0.0f,0.0f);
		Float3Property() = default;
		Float3Property(Float3Property& copy)
		{
			copy.f3 = f3;
		};
		Float3Property(float x, float y, float z) : f3(x, y, z) {};
	};

	struct MaterialProperty
	{
		MaterialProperty() = default;
		std::string name;
		
		std::map<int,Property*> m_vProperties;  //Une map avec un indice pour savoir quel type c'est 
		
		void AddProperty(Property* pProperty, uint32_t PropertyType)
		{

			m_vProperties.emplace(PropertyType, reinterpret_cast<Float3Property*>(pProperty));
		};
		void GetFloat3Property()
		{
			auto itterator = m_vProperties.find(4);
			Float3Property* test = reinterpret_cast<Float3Property*>(itterator->first);
			//auto test = m_vProperties.find(4);
		};
	};




	class DECL Material
	{
	public:
		Material() = default;
		Material(const std::string& MaterialName);
		Material(Texture* m_albedoMap, Texture* m_normalMap, Texture* m_specularMap, Texture* m_metalicMap, Texture* m_ambiantOcclusionMap);
		Material(Texture& m_albedoMap, Texture& m_normalMap, Texture& m_specularMap, Texture& m_ambiantOcclusionMap);
		void SetAlbedoMap(const std::filesystem::path& m_albedoPath);
		void SetNormalMap(const std::filesystem::path& m_NormalPath);
		void SetSpecularMap(const std::filesystem::path& m_SpecularPath);
		void SetMetalicMap(const std::filesystem::path& m_MetalicPath);
		void SetEmisiveMap(const std::filesystem::path& emissivePath);
		void SetAOMap(const std::filesystem::path& m_AOPath);
		void SetVertexShader(const std::filesystem::path& ShaderPath);
		void SetPixelShader(const std::filesystem::path& ShaderPath);
		void Bind();

		int RenderMode = 0; //Par défaut il est opaque

		VertexShaders m_VertexShader;
		PixelShaders m_PixelShader;

		std::string m_name;
		std::string m_usedMaterialName;

		Texture m_albedoMap;
		Texture m_normalMap;
		Texture m_specularMap;
		Texture m_metalicMap;
		Texture m_ambiantOcclusionMap;
		Texture m_EmissiveMap;

		MaterialProperty m_MaterialProperty;

		MaterialBuffer m_PBRBUFFER;
	private:

	};

}