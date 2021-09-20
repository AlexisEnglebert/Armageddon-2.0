#pragma once
#include "../Macros.h"
#include "../Renderer/Shaders.h"
#include "Material.h"
#include <vector>
#include <filesystem>
#include "Texture.h"
class DECL AssetManager
{
public:
	AssetManager();
	static Texture GetOrCreateTexture(const std::filesystem::path& TexturePath);
	static Armageddon::Material GetOrCreateMaterial(const std::string& MaterialName);
	static bool MaterialExist(const std::string& MaterialName);
	static Armageddon::PixelShaders GetOrCreatePixelShader(const std::filesystem::path& ShaderPath);
	static Armageddon::VertexShaders GetOrCreateVertexShader(const std::filesystem::path& ShaderPath);

	static std::vector<Armageddon::PixelShaders>	v_PixelShaders;
	static std::vector<Armageddon::VertexShaders>	v_VertexShaders;
	static std::vector<Armageddon::Material>		v_material;
	static std::vector<Texture>					v_texture;

	//static Armageddon::Material		m_DefaultMaterial;
	// 
	
	//Armageddon::Material m_DefaultMaterial = { m_defaultAlbedo ,m_defaultNormal,m_defaultRoughness,m_defaultAO };

private:
	
};