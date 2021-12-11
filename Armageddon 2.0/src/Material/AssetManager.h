#pragma once
#include "../Macros.h"
#include "../Renderer/Shaders.h"
#include "../Utils/HashUtils.h"
#include <vector>
#include <filesystem>
class Texture;
class Material;

enum class AssetType
{
	UndefinedAsset,
	TextureAsset,
	MaterialAsset,
	ModelAsset

};
class DECL Asset
{
public:
	std::string m_AssetName;
	AssetType m_AssetType = AssetType::UndefinedAsset;
};
class DECL AssetManager
{
public:
	AssetManager();
	static Texture GetOrCreateTexture(const std::filesystem::path& TexturePath);
	//static Material GetOrCreateMaterial(const std::string& MaterialName);
	static uint64_t GetOrCreateMaterial(const std::string& MaterialName);
	static void CreateMaterial(const std::string& MaterialName);
	static void CreateMaterial(uint64_t hash,const std::string& MaterialName);
	static Armageddon::PixelShaders GetOrCreatePixelShader(const std::filesystem::path& ShaderPath);
	static Armageddon::VertexShaders GetOrCreateVertexShader(const std::filesystem::path& ShaderPath);
	static bool MaterialExist(const std::string& MaterialName);
	static std::vector<Armageddon::PixelShaders>	v_PixelShaders;
	static std::vector<Armageddon::VertexShaders>	v_VertexShaders;
	static std::vector<Material>		v_material;
	static std::vector<Texture>					v_texture;

	//static Material		m_DefaultMaterial;
	// 
	
	//Material m_DefaultMaterial = { m_defaultAlbedo ,m_defaultNormal,m_defaultRoughness,m_defaultAO };
	static std::unordered_map<std::size_t, Asset> m_AssetMap;
	static std::unordered_map<std::size_t, Material> m_MaterialMap;

private:
	
};