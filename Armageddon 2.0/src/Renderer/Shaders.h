#pragma once

#include "../Macros.h"
#include "../Log.h"
#if __linux__
#else
#include <d3d11.h>
#include <wrl/client.h>
#include <d3dcompiler.h>
#include <d3d11shader.h>
#endif
#include <string>
#include <filesystem>
#include "Interface.h"
#pragma comment(lib,"D3DCompiler.lib")
#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"dxguid.lib")
namespace Armageddon
{
	typedef struct
	{
		const char* name;
		D3D_SRV_DIMENSION dimension;
	}shaderTexture_t;
	class Shader
	{
	public:
		bool virtual Init(Microsoft::WRL::ComPtr<ID3D11Device>& device, std::filesystem::path shaderPath) { return false; };
		virtual ID3D10Blob* GetBuffer();
		virtual bool ReloadShader(const char* entry_point, const char* type);
		virtual bool CreateShader() { return false; };
		virtual std::string GetErrorMessage();
		std::filesystem::path m_shaderPath;
		std::vector<shaderTexture_t> v_ShaderBindTexture;

	protected:
		Microsoft::WRL::ComPtr <ID3D10Blob> m_shaderBuffer;
		std::string error_message = "";


	};
	class DECL VertexShaders : public Shader
	{
	public:
		bool Init(Microsoft::WRL::ComPtr<ID3D11Device>& device, std::filesystem::path shaderPath) override;
		bool CreateShader() override;

		ID3D11VertexShader* GetShader();
		ID3D11InputLayout* GetInputLayout();

	private:
		Microsoft::WRL::ComPtr <ID3D11VertexShader> VertexShader;
		Microsoft::WRL::ComPtr<ID3D11InputLayout> InputLayout;
	};


	class DECL PixelShaders : public Shader
	{
	public:
		bool Init(Microsoft::WRL::ComPtr<ID3D11Device>& device, std::filesystem::path shaderPath) override;
		bool CreateShader() override;

		ID3D11PixelShader* GetShader();
	private:
		Microsoft::WRL::ComPtr <ID3D11PixelShader> PixelShader;

	};

	class DECL ComputeShader : public Shader
	{
	public:
		bool Init(Microsoft::WRL::ComPtr<ID3D11Device>& device, std::filesystem::path shaderPath) override;
		bool CreateShader() override;
		ID3D11ComputeShader* GetShader();

	private:
		Microsoft::WRL::ComPtr <ID3D11ComputeShader> m_ComputeShader;
	};
}