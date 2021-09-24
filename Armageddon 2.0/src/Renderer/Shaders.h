#pragma once

#include "../Macros.h"
#include "../Log.h"
#include <d3d11.h>
#include <wrl/client.h>
#include <string>
#include <d3dcompiler.h>
#include <filesystem>
#include "Interface.h"
#include <d3d11shader.h>
#pragma comment(lib,"D3DCompiler.lib")
#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"dxguid.lib")
namespace Armageddon
{
	class DECL VertexShaders
	{
	public:
		bool Init(Microsoft::WRL::ComPtr<ID3D11Device>& device, std::filesystem::path shaderPath);
		bool CreateVertexShader();
		ID3D11VertexShader* GetShader();
		ID3D10Blob* GetBuffer();
		ID3D11InputLayout* GetInputLayout();
		std::filesystem::path ShaderPath;

	private:
		Microsoft::WRL::ComPtr <ID3D11VertexShader> VertexShader;
		Microsoft::WRL::ComPtr <ID3D10Blob> VertexShaderBuffer;
		Microsoft::WRL::ComPtr<ID3D11InputLayout> InputLayout;
	};


	class DECL PixelShaders
	{
	public:
		bool Init(Microsoft::WRL::ComPtr<ID3D11Device>& device, std::filesystem::path shaderPath);
		ID3D11PixelShader* GetShader();
		ID3D10Blob* GetBuffer();
		std::filesystem::path ShaderPath;
	private:
		Microsoft::WRL::ComPtr <ID3D11PixelShader> PixelShader;
		Microsoft::WRL::ComPtr <ID3D10Blob> PixelShaderBuffer;

	};

	class DECL ComputeShader
	{
		bool Init(Microsoft::WRL::ComPtr<ID3D11Device>& device, std::filesystem::path shaderPath);
		void CreateComputeShader();
		ID3D11ComputeShader* GetShader();
		ID3D10Blob* GetBuffer();
		std::filesystem::path ShaderPath;
		Microsoft::WRL::ComPtr <ID3D11ComputeShader> m_ComputeShader;
		Microsoft::WRL::ComPtr <ID3D10Blob> m_ComputeShaderBuffer;
	};
}