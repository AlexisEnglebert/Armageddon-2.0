#include "AssetManager.h"
#include "Texture.h"
#include "Material.h"

 std::vector<Armageddon::PixelShaders>	AssetManager::v_PixelShaders;
 std::vector<Armageddon::VertexShaders>	AssetManager::v_VertexShaders;
 std::vector<Material>		AssetManager::v_material;
 std::vector<Texture>					AssetManager::v_texture;
 std::unordered_map<std::size_t, Asset> AssetManager::m_AssetMap;
 std::unordered_map<std::size_t, Material> AssetManager::m_MaterialMap;

AssetManager::AssetManager()
{
    
}

Texture AssetManager::GetOrCreateTexture(const std::filesystem::path& TexturePath)
{
    for (UINT i = 0; i < v_texture.size(); i++)
    {
        if (TexturePath == v_texture[i].m_AssetName) {
            Armageddon::Log::GetLogger()->trace("Texture alredy exist");
            return v_texture[i];
        }
    }
    Texture tex = Texture(TexturePath);
    v_texture.push_back(tex);
    return tex;
}

Material AssetManager::GetOrCreateMaterial(const std::string& MaterialName)
{
	for (UINT i = 0; i < v_material.size(); i++)
	{
        if (MaterialName == v_material[i].m_AssetName) {
            Armageddon::Log::GetLogger()->trace("Material alerady exist : {0}",MaterialName);
            return v_material[i];
        }
	}
    Material mat = Material(MaterialName);
    v_material.push_back(mat);
    return mat;
}

bool AssetManager::MaterialExist(const std::string& MaterialName)
{
	for (UINT i = 0; i < v_material.size(); i++)
	{
		if (MaterialName == v_material[i].m_AssetName) {
            Armageddon::Log::GetLogger()->trace("Material {0} Already Exist : ", MaterialName);
            return true;
		}
	}
    return false;
}

Armageddon::PixelShaders AssetManager::GetOrCreatePixelShader(const std::filesystem::path& ShaderPath)
{
    for (UINT i = 0; i < v_PixelShaders.size(); i++)
    {
        if (ShaderPath == v_PixelShaders[i].ShaderPath) {
			Armageddon::Log::GetLogger()->trace("PixelShader alerady exist : {0}", ShaderPath);
            return v_PixelShaders[i];
        }
    }
    Armageddon::PixelShaders px;
    px.Init(Armageddon::Interface::GetDevice(),ShaderPath);
    v_PixelShaders.push_back(px);
    return px;
}

Armageddon::VertexShaders AssetManager::GetOrCreateVertexShader(const std::filesystem::path& ShaderPath)
{
	for (UINT i = 0; i < v_VertexShaders.size(); i++)
	{
		if (ShaderPath == v_VertexShaders[i].ShaderPath) {
			Armageddon::Log::GetLogger()->trace("PixelShader alerady exist : {0}", ShaderPath);
			return v_VertexShaders[i];
		}
	}
	Armageddon::VertexShaders vx;
    vx.Init(Armageddon::Interface::GetDevice(), ShaderPath);
    v_VertexShaders.push_back(vx);
    return vx;
}
