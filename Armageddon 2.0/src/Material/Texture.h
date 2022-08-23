#pragma once
#if __linux__
//pass pour l'instant
#else
#include <d3d11.h>
#include <wrl\client.h>
#include "DirectXTex/DirectXTex.h"
#include "WICTextureLoader/WICTextureLoader11.h"
#endif
#include <vector>
#include <string>
#include <filesystem>
#include "../Macros.h"
#include "../Log.h"
#include "../Renderer/Interface.h"
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

	DXGI_FORMAT m_Format;
	Microsoft::WRL::ComPtr<ID3D11Resource> TextureRessource;

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> TextureRessourceView;
	float ImageX = 0, ImageY = 0;
};

class DECL Texture3D : public Texture
{
public:
	Texture3D() = default;
	bool Init(UINT width, UINT height, UINT depth, UINT miplevel, DXGI_FORMAT format);

	ID3D11UnorderedAccessView* m_UAV; 
private:

};

class DECL RenderTexture : public Texture
{
public:
	RenderTexture() = default;
	RenderTexture(float width, float height, DXGI_FORMAT format);
	bool Init(float width, float height, DXGI_FORMAT format);
	ID3D11RenderTargetView* RenderTargetView;
	ID3D11ShaderResourceView* GetRessourceView() { return TextureRessourceView.Get(); };
	ID3D11ShaderResourceView** GetRessourceViewPtr() { return TextureRessourceView.GetAddressOf(); };
	bool ResizeTexture(float& width, float& height);
	//TODO BIND AND UNBIND

private : 


};


//TODO RENDERTEXUTREONLYDEPHT
class DECL DepthTexture : public Texture
{
public:
	DepthTexture() = default;
	bool Init(float width, float height, DXGI_FORMAT format);
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> DephtStencilView;
	//Set RTV to null and bind the depth stencil

private:
};

class DECL RenderTextureDepht : public RenderTexture
{
public:
	RenderTextureDepht() = default;
	RenderTextureDepht(float width, float height, DXGI_FORMAT format);
	bool Init(float width, float height, DXGI_FORMAT format);
	bool ResizeTexture(float& width, float& height);

	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> DephtStencilView;
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