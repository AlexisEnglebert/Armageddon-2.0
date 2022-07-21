#pragma once
#include "../Macros.h"
#include "Texture.h"
#include "../Renderer/Shaders.h"
#include "../Renderer/ConstantBufferTypes.h"
#include "AssetManager.h"

#include <atomic>
#include <map>
#include "../Serialization/Tokenizer.h"

#define AG_RENDERMODE_OPAQUE = 0
#define AG_RENDERMODE_TRANSPARENT = 1



	class DECL Material : public Asset
	{
	public:
		Material() = default;
		Material(const std::string& MaterialName);
		Material(Texture* m_albedoMap, Texture* m_normalMap, Texture* m_specularMap, Texture* m_metalicMap, Texture* m_ambiantOcclusionMap);
		Material(Texture& m_albedoMap, Texture& m_normalMap, Texture& m_specularMap, Texture& m_ambiantOcclusionMap);
		~Material();
		//TODO ça doit changer et être définis dans le shader ou le material
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

		MaterialBuffer m_PBRBUFFER;

		//Test d'un ConstBuffer Dynamique :
		char* TestConstBuffer;

		std::vector<Token> m_testToken; //Need list of textures
		std::vector<Texture> m_Textures;
	private:

	};


