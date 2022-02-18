#pragma once
#include <d3d11.h>
#include <vector>
#include <wrl\client.h>
#include <string>
#include <filesystem>
#include "../Macros.h"
#include "../Log.h"
#include "../Renderer/Interface.h"
#include "WICTextureLoader/WICTextureLoader11.h"
#include "DirectXTex/DirectXTex.h"
#include "../Renderer/Shaders.h"	
#include "AssetManager.h"

class DECL Texture : public Asset
{
public:
	Texture() = default;
	Texture(std::filesystem::path path);
	void Create(const std::filesystem::path& path);
	
	float& GetImageX() {return ImageX;};
	float& GetImageY() {return ImageY;};
	ID3D11ShaderResourceView* GetRessourceView() {return TextureRessourceView.Get();};
	ID3D11ShaderResourceView** GetRessourceViewPtr() {return TextureRessourceView.GetAddressOf();};
	ID3D11Resource* GetRessource() {return TextureRessource.Get();};
	ID3D11Resource** GetRessourcePtr() {return TextureRessource.GetAddressOf();};
protected:
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> TextureRessourceView;
	Microsoft::WRL::ComPtr<ID3D11Resource> TextureRessource;
	float ImageX = 0, ImageY = 0;
};
class DECL RenderTexture : public Texture
{
public:
	RenderTexture() = default;
	RenderTexture(float width, float height, DXGI_FORMAT format);
	ID3D11RenderTargetView* RenderTargetView;
	ID3D11ShaderResourceView* GetRessourceView() { return TextureRessourceView.Get(); };
	ID3D11ShaderResourceView** GetRessourceViewPtr() { return TextureRessourceView.GetAddressOf(); };
	//TODO RESIZE 
	//TODO BIND AND UNBIND


	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> DephtStencilView;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> DephtStencilState;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> DephtStencilBuffer;

private : 


};
class DECL RenderTextureDepht : public RenderTexture
{
	RenderTextureDepht() = default;
	RenderTextureDepht(float width, float height, DXGI_FORMAT format);
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> DephtStencilView;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> DephtStencilState;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> DephtStencilBuffer;
};
class DECL EnvTexture : public Texture
{
public:
	EnvTexture() = default;
	void CreateCubeMap(const std::filesystem::path& path);
	void CreateIrradiancedMap(ID3D11ShaderResourceView** envmapRessource);
	void CreatePreFilteredMap(ID3D11ShaderResourceView** envmapRessource);
};