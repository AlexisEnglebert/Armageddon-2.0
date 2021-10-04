#include "Material.h"
#include "AssetManager.h"
#include "../Renderer/Renderer.h"

Armageddon::Material::Material(const std::string& MaterialName) : m_name(MaterialName) , m_usedMaterialName(m_usedMaterialName)
{
	Armageddon::Log::GetLogger()->error("POUF");
	 m_albedoMap					 = AssetManager::GetOrCreateTexture( "Ressources//Textures//DefaultAlbedo.png" );
	 m_normalMap					 = AssetManager::GetOrCreateTexture( "Ressources//Textures//DefaultNormal.tiff" );
	 m_specularMap					 = AssetManager::GetOrCreateTexture( "Ressources//Textures//DefaultAlbedo.png" );
	 m_ambiantOcclusionMap			 = AssetManager::GetOrCreateTexture( "Ressources//Textures//DefaultAlbedo.png" );

	 m_VertexShader					 = AssetManager::GetOrCreateVertexShader(L"..\\bin\\Debug-x64\\Armageddon 2.0\\DefaultVertexShader.cso");
	 m_PixelShader					 = AssetManager::GetOrCreatePixelShader(L"..\\bin\\Debug-x64\\Armageddon 2.0\\PBR.cso");
	 m_EmissiveMap					 = AssetManager::GetOrCreateTexture("Ressources//Textures//DefaultAlbedo.png");

	 //m_MaterialProperty.AddProperty(std::make_unique<Float3Property>(1.0f, 5.0f, 0.0f));
	 
	Texture m_metalicMap;

}

Armageddon::Material::Material(Texture& m_albedoMap, Texture& m_normalMap, Texture& m_specularMap, Texture& m_ambiantOcclusionMap)
{
	this->m_albedoMap = m_albedoMap;
	this->m_normalMap = m_normalMap;
	this->m_specularMap = m_specularMap;
	this->m_ambiantOcclusionMap = m_ambiantOcclusionMap;
}	


void Armageddon::Material::SetAlbedoMap(const std::filesystem::path& m_albedoPath)
{
	m_albedoMap = AssetManager::GetOrCreateTexture(m_albedoPath);
	
}

void Armageddon::Material::SetNormalMap(const std::filesystem::path& m_NormalPath)
{
	m_normalMap = AssetManager::GetOrCreateTexture(m_NormalPath);

}

void Armageddon::Material::SetSpecularMap(const std::filesystem::path& m_SpecularPath)
{
	m_specularMap = AssetManager::GetOrCreateTexture(m_SpecularPath);
}

void Armageddon::Material::SetMetalicMap(const std::filesystem::path& m_MetalicPath)
{
	m_metalicMap = AssetManager::GetOrCreateTexture(m_MetalicPath);

}

void Armageddon::Material::SetEmisiveMap(const std::filesystem::path& emissivePath)
{
	m_EmissiveMap = AssetManager::GetOrCreateTexture(emissivePath);

}

void Armageddon::Material::SetAOMap(const std::filesystem::path& m_AOPath)
{
	m_ambiantOcclusionMap = AssetManager::GetOrCreateTexture(m_AOPath);

}

void Armageddon::Material::SetVertexShader(const std::filesystem::path& ShaderPath)
{
	m_VertexShader = AssetManager::GetOrCreateVertexShader(ShaderPath);
}

void Armageddon::Material::SetPixelShader(const std::filesystem::path& ShaderPath)
{
	m_PixelShader = AssetManager::GetOrCreatePixelShader(ShaderPath);

}

Armageddon::Material::Material(Texture* m_albedoMap, Texture* m_normalMap, Texture* m_specularMap, Texture* m_metalicMap, Texture* m_ambiantOcclusionMap)
{
	
}

void Armageddon::Material::Bind()
{
	/*
	* 
	*/
	if (RenderMode == 1)
	{
		float blendFactor[] = { 0.0f,0.0f,0.0f,0.0f };
		Armageddon::Interface::GetDeviceContext()->OMSetBlendState(Armageddon::Renderer::AlphaBlendState.Get(), blendFactor, 0xffffffff);
	}
	else
	{
		Armageddon::Interface::GetDeviceContext()->OMSetBlendState(0, 0, 0xffffffff);

	}

	Armageddon::Renderer::g_PBRCBuffer.SetDynamicData(&m_PBRBUFFER);
	Armageddon::Renderer::g_PBRCBuffer.BindPS();
	Armageddon::Renderer::g_PBRCBuffer.BindVS();

	Armageddon::Interface::GetDeviceContext()->IASetInputLayout(m_VertexShader.GetInputLayout());
	Armageddon::Interface::GetDeviceContext()->PSSetSamplers(0, 1, Armageddon::Interface::GetSamplerState().GetAddressOf());
	Armageddon::Interface::GetDeviceContext()->PSSetSamplers(1, 1, Armageddon::Interface::GetClampSampler().GetAddressOf());

	Armageddon::Interface::GetDeviceContext()->PSSetShaderResources(0, 1, m_albedoMap.GetRessourceViewPtr());
	Armageddon::Interface::GetDeviceContext()->PSSetShaderResources(1, 1, m_normalMap.GetRessourceViewPtr());
	Armageddon::Interface::GetDeviceContext()->PSSetShaderResources(2, 1, m_specularMap.GetRessourceViewPtr());
	Armageddon::Interface::GetDeviceContext()->PSSetShaderResources(3, 1, m_metalicMap.GetRessourceViewPtr());
	Armageddon::Interface::GetDeviceContext()->PSSetShaderResources(4, 1, m_ambiantOcclusionMap.GetRessourceViewPtr());
	Armageddon::Interface::GetDeviceContext()->PSSetShaderResources(5, 1, m_EmissiveMap.GetRessourceViewPtr());
	Armageddon::Interface::GetDeviceContext()->VSSetShader(m_VertexShader.GetShader(), NULL, 0);
	Armageddon::Interface::GetDeviceContext()->PSSetShader(m_PixelShader.GetShader(), NULL, 0);


}
