#pragma once
#include "../Macros.h"
#include "Texture.h"
#include "../Renderer/Shaders.h"
#include "../Renderer/ConstantBufferTypes.h"
#include "AssetManager.h"

#include <atomic>
#include <map>

#define AG_RENDERMODE_OPAQUE = 0
#define AG_RENDERMODE_TRANSPARENT = 1

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
	struct  Property
	{
		uint32_t ID;
		static uint32_t PropertyID;
		Property() : ID(PropertyID++)
		{};
	};



	struct TextureProperty : Property
	{
		std::string m_name;
		Texture m_Texture;
		TextureProperty(std::string name) : m_name(name) {};
		TextureProperty(const TextureProperty& copy) 
		{
			m_name = copy.m_name;
			m_Texture = copy.m_Texture;
		};

	};
	struct BoolProperty : Property
	{
		std::string m_name;
		bool boolean;
		BoolProperty() = default;
		BoolProperty(const BoolProperty& copy) { boolean = copy.boolean; m_name = copy.m_name; };
		BoolProperty(std::string name) : m_name(name)
		{

		};
		BoolProperty(bool value) : boolean(value) {};

	};
	struct FloatProperty : Property
	{
		float x;
		FloatProperty() = default;
		FloatProperty(const FloatProperty& copy)
		{};
		FloatProperty(float x) : x(x) {};

	};
	struct Float2Property : Property
	{
		DirectX::XMFLOAT2 f2 = DirectX::XMFLOAT2(0.0f, 0.0f);

		Float2Property() = default;
		Float2Property(const Float2Property& copy)
		{
			f2 = copy.f2;
		};

		Float2Property(float x, float y) :f2(x, y) {};

	};
	struct Float3Property : Property
	{
		DirectX::XMFLOAT3 f3 = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
		Float3Property() = default;
		Float3Property(const Float3Property& copy)
		{
			f3 = copy.f3;
		};
		Float3Property(float x, float y, float z) : f3(x, y, z) {};
	};

	struct MaterialProperty
	{
		MaterialProperty() = default;
		std::string name;

		void AddFloat3Property(float x, float y, float z)
		{
			m_Vfloat3.push_back(Float3Property(x, y, z));
		};
		void AddFloat2Property(float x, float y)
		{
			m_Vfloat2.push_back(Float2Property(x, y));
		};
		void AddFloatProperty(float x)
		{
			m_Vfloat.push_back(FloatProperty(x));
		};
		void AddTextureProperty(std::string name)
		{
			m_VTexure.push_back(TextureProperty(name));
		};
		void AddBoolProperty(std::string name)
		{
			m_Vbool.push_back(BoolProperty(name));
		};
		std::size_t GetSize()
		{
			return m_Vfloat3.size();
		};
		template<class T>
		bool HasPropery(T& value);

		std::vector<Float3Property> m_Vfloat3;
		std::vector<Float2Property> m_Vfloat2;
		std::vector<FloatProperty> m_Vfloat;
		std::vector<BoolProperty> m_Vbool;
		std::vector<TextureProperty> m_VTexure;

	};




	class DECL Material : public Asset
	{
	public:
		Material() = default;
		Material(const std::string& MaterialName);
		Material(Texture* m_albedoMap, Texture* m_normalMap, Texture* m_specularMap, Texture* m_metalicMap, Texture* m_ambiantOcclusionMap);
		Material(Texture& m_albedoMap, Texture& m_normalMap, Texture& m_specularMap, Texture& m_ambiantOcclusionMap);
		~Material();
		void SetAlbedoMap(const std::filesystem::path& m_albedoPath);
		void SetNormalMap(const std::filesystem::path& m_NormalPath);
		void SetSpecularMap(const std::filesystem::path& m_SpecularPath);
		void SetMetalicMap(const std::filesystem::path& m_MetalicPath);
		void SetEmisiveMap(const std::filesystem::path& emissivePath);
		void SetAOMap(const std::filesystem::path& m_AOPath);
		void SetVertexShader(const std::filesystem::path& ShaderPath);
		void SetPixelShader(const std::filesystem::path& ShaderPath);
		void BindRessources();
		void BindShaders();

		int RenderMode = 0; //Par défaut il est opaque

		Armageddon::VertexShaders m_VertexShader;
		Armageddon::PixelShaders m_PixelShader;

		std::string m_usedMaterialName;

		Texture m_albedoMap;
		Texture m_normalMap;
		Texture m_specularMap;
		Texture m_metalicMap;
		Texture m_ambiantOcclusionMap;
		Texture m_EmissiveMap;

		MaterialProperty m_MaterialProperty;

		MaterialBuffer m_PBRBUFFER;

		//Test d'un ConstBuffer Dynamique :
		char* TestConstBuffer;
	private:

	};

	template<class T>
	inline bool MaterialProperty::HasPropery(T& value)
	{

		return false;
	}

