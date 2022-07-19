#include "Shaders.h"

bool Armageddon::PixelShaders::Init(Microsoft::WRL::ComPtr<ID3D11Device>& device, std::filesystem::path m_shaderPath)
{
	this->m_shaderPath = m_shaderPath;
	if (m_shaderPath.extension() == ".cso")
	{
		HRESULT hr = D3DReadFileToBlob(m_shaderPath.c_str(), this->m_shaderBuffer.GetAddressOf());
		if (FAILED(hr))
		{
			Armageddon::Log::GetLogger()->error("FAILED READING PIXEL SHADER : {0}", m_shaderPath.string().c_str());
			return false;
		}
		hr = device->CreatePixelShader(this->m_shaderBuffer.Get()->GetBufferPointer(), this->m_shaderBuffer.Get()->GetBufferSize(), NULL, this->PixelShader.GetAddressOf());
		if (FAILED(hr))
		{
			Armageddon::Log::GetLogger()->error("FAILED CREATING PIXEL SHADER : {0}", m_shaderPath.string().c_str());
			return false;
		}
	}
	else if (m_shaderPath.extension() == ".hlsl")
	{
		Armageddon::Log::GetLogger()->info("No Compiled Shader given, trying to compile it");

		Microsoft::WRL::ComPtr <ID3D10Blob> blob;
		HRESULT hr = D3DReadFileToBlob(m_shaderPath.c_str(), blob.GetAddressOf());
		if (FAILED(hr))
		{
			Armageddon::Log::GetLogger()->error("Failed reading file");
			return false;
		}
		ID3DBlob* pError;

		hr = D3DCompile(blob->GetBufferPointer(), blob->GetBufferSize(), NULL, NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "ps_5_0",
			15, 0, m_shaderBuffer.GetAddressOf(), &pError);
		if (pError) {
			Armageddon::Log::GetLogger()->error("FAILED Compiling VertexShader [{0}]", (char*)pError->GetBufferPointer());
			pError->Release();
		}

		if (FAILED(hr))
		{

			Armageddon::Log::GetLogger()->error("FAILED Compiling VertexShader [{0}]", hr);
			return false;

		}

		
	}
    return true;
}

bool Armageddon::PixelShaders::CreateShader()
{
	UINT hr = Armageddon::Interface::GetDevice()->CreatePixelShader(m_shaderBuffer.Get()->GetBufferPointer(), m_shaderBuffer.Get()->GetBufferSize(), NULL, this->PixelShader.GetAddressOf());
	if (FAILED(hr))
	{
		Armageddon::Log::GetLogger()->error("FAILED CREATING PIXEL SHADER");
		return false;
	}

	return true;
}


bool Armageddon::VertexShaders::Init(Microsoft::WRL::ComPtr<ID3D11Device>& device, std::filesystem::path m_shaderPath)
{
    this->m_shaderPath = m_shaderPath;
    if (m_shaderPath.extension() == ".cso")
    {
        HRESULT hr = D3DReadFileToBlob(m_shaderPath.c_str(), m_shaderBuffer.GetAddressOf());
        if (FAILED(hr))
        {

            Armageddon::Log::GetLogger()->error("FAILED READING VERTEX SHADER  [{0}] , {1}", hr, m_shaderPath.string().c_str());
            return false;

        }
		CreateShader();

    }
    else if (m_shaderPath.extension() == ".hlsl")
    {
        Armageddon::Log::GetLogger()->info("No Compiled Shader given, trying to compile it");

		Microsoft::WRL::ComPtr <ID3D10Blob> blob;

		HRESULT hr = D3DReadFileToBlob(m_shaderPath.c_str(), blob.GetAddressOf());

        hr = D3DCompile(blob->GetBufferPointer(), blob->GetBufferSize(), NULL, NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "vs_5_0",
            0, 0, m_shaderBuffer.GetAddressOf(),NULL);

	   if (FAILED(hr))
	   {

		   Armageddon::Log::GetLogger()->error("FAILED Compiling VertexShader [{0}]", hr);
		   return false;

	   }

	   CreateShader();
    }

    return true;
}


bool Armageddon::VertexShaders::CreateShader()
{
	HRESULT hr = Armageddon::Interface::GetDevice()->CreateVertexShader(m_shaderBuffer->GetBufferPointer(), m_shaderBuffer->GetBufferSize(), NULL, this->VertexShader.GetAddressOf());
	if (FAILED(hr))
	{
		Armageddon::Log::GetLogger()->error("FAILED  CREATING VERTEX SHADER  [{0}] ", hr);
		return false;

	}
	ID3D11ShaderReflection* ShaderReflexion = nullptr;



	hr = D3DReflect(m_shaderBuffer->GetBufferPointer(), m_shaderBuffer->GetBufferSize(), IID_ID3D11ShaderReflection, (void**)&ShaderReflexion);
	if (FAILED(hr))
	{
		Armageddon::Log::GetLogger()->error("FAILED  REFLECTING SHADERS [{0}]", hr);

	}
	D3D11_SHADER_DESC ShaderDescription;
	ZeroMemory(&ShaderDescription, sizeof(D3D11_SHADER_DESC));

	ShaderReflexion->GetDesc(&ShaderDescription);


	std::vector< D3D11_INPUT_ELEMENT_DESC> v_ElementDesc;
	for (UINT i = 0; i < ShaderDescription.InputParameters; i++)
	{
		D3D11_SIGNATURE_PARAMETER_DESC SignatureDesc;
		ShaderReflexion->GetInputParameterDesc(i, &SignatureDesc);
		D3D11_INPUT_ELEMENT_DESC InputDesc;
		InputDesc.SemanticName = SignatureDesc.SemanticName;
		InputDesc.SemanticIndex = SignatureDesc.SemanticIndex;
		InputDesc.InputSlot = 0;
		InputDesc.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
		InputDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		InputDesc.InstanceDataStepRate = 0;

		if (SignatureDesc.Mask == 1)
		{
			if (SignatureDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) InputDesc.Format = DXGI_FORMAT_R32_UINT;
			if (SignatureDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) InputDesc.Format = DXGI_FORMAT_R32_FLOAT;
			if (SignatureDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) InputDesc.Format = DXGI_FORMAT_R32_SINT;
		}
		else if (SignatureDesc.Mask <= 3)
		{
			if (SignatureDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) InputDesc.Format = DXGI_FORMAT_R32G32_UINT;
			if (SignatureDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) InputDesc.Format = DXGI_FORMAT_R32G32_FLOAT;
			if (SignatureDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) InputDesc.Format = DXGI_FORMAT_R32G32_SINT;
		}
		else if (SignatureDesc.Mask <= 7)
		{
			if (SignatureDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) InputDesc.Format = DXGI_FORMAT_R32G32B32_UINT;
			if (SignatureDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) InputDesc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
			if (SignatureDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) InputDesc.Format = DXGI_FORMAT_R32G32B32_SINT;
		}
		else if (SignatureDesc.Mask <= 15)
		{
			if (SignatureDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) InputDesc.Format = DXGI_FORMAT_R32G32B32A32_UINT;
			if (SignatureDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) InputDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
			if (SignatureDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) InputDesc.Format = DXGI_FORMAT_R32G32B32A32_SINT;
		}
		v_ElementDesc.push_back(InputDesc);
		//Armageddon::Log::GetLogger()->trace("Semantics: {0}", SignatureDesc.SemanticName);

	}

	hr = Armageddon::Interface::GetDevice()->CreateInputLayout(&v_ElementDesc[0], v_ElementDesc.size(), m_shaderBuffer->GetBufferPointer(), m_shaderBuffer->GetBufferSize(), this->InputLayout.GetAddressOf());
	if (FAILED(hr))
	{
		Armageddon::Log::GetLogger()->error("FAILED  CREATING INPUT LAYOUT : {0}", hr);

	}
	//Armageddon::Log::GetLogger()->trace("FINISHED");
}

ID3D11PixelShader* Armageddon::PixelShaders::GetShader()
{
    return this->PixelShader.Get();
}


ID3D11VertexShader* Armageddon::VertexShaders::GetShader()
{
    return this->VertexShader.Get();
}

ID3D11InputLayout* Armageddon::VertexShaders::GetInputLayout()
{
    return this->InputLayout.Get();
}

bool Armageddon::ComputeShader::Init(Microsoft::WRL::ComPtr<ID3D11Device>& device, std::filesystem::path m_shaderPath)
{
	this->m_shaderPath = m_shaderPath;
	if (m_shaderPath.extension() == ".cso")
	{
		HRESULT hr = D3DReadFileToBlob(m_shaderPath.c_str(), m_shaderBuffer.GetAddressOf());
		if (FAILED(hr))
		{

			Armageddon::Log::GetLogger()->error("FAILED INIT ComputeShader SHADER  [{0}]", hr);
			return false;

		}
		CreateShader();

	}
	else if (m_shaderPath.extension() == ".hlsl")
	{
		Armageddon::Log::GetLogger()->info("No Compiled Shader given, trying to compile it");

		Microsoft::WRL::ComPtr <ID3D10Blob> blob;

		HRESULT hr = D3DReadFileToBlob(m_shaderPath.c_str(), blob.GetAddressOf());

		hr = D3DCompile(blob->GetBufferPointer(), blob->GetBufferSize(), NULL, NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE, "CSmain", "cs_5_0",
			0, 0, m_shaderBuffer.GetAddressOf(), NULL);

		if (FAILED(hr))
		{

			Armageddon::Log::GetLogger()->error("FAILED Compiling ComputeShader [{0}]", hr);
			return false;
		}
		CreateShader();
	}
}

bool Armageddon::ComputeShader::CreateShader()
{
	HRESULT hr = Armageddon::Interface::GetDevice()->CreateComputeShader(m_shaderBuffer->GetBufferPointer(), m_shaderBuffer.Get()->GetBufferSize(), NULL, this->m_ComputeShader.GetAddressOf());
	if (FAILED(hr))
	{
		Armageddon::Log::GetLogger()->error("FAILED CREATING Compute SHADER");
		return false;
	}
	return true;
}

ID3D10Blob* Armageddon::Shader::GetBuffer()
{
	return m_shaderBuffer.Get();
}

bool Armageddon::Shader::ReloadShader(const char* entry_point, const char* type)
{
	//check if the current shader is a compiled shader
	if (m_shaderPath.extension() == ".cso")
	{
		return true;
	}
	//check if the modified shader is compilable ?!?
	Microsoft::WRL::ComPtr <ID3D10Blob> blob;
	Microsoft::WRL::ComPtr <ID3D10Blob> tmp_shaderBuffer;

	HRESULT hr = D3DReadFileToBlob(m_shaderPath.c_str(), blob.GetAddressOf());
	ID3DBlob* pError;

	hr = D3DCompile(blob->GetBufferPointer(), blob->GetBufferSize(), NULL, NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE, entry_point, type,
		0, 0, tmp_shaderBuffer.GetAddressOf(), &pError);
	if (FAILED(hr))
	{
		Armageddon::Log::GetLogger()->error("Failed compiling {0} : {1}",m_shaderPath, (char*)pError->GetBufferPointer());
		return false;
	}
	m_shaderBuffer = tmp_shaderBuffer;
	if (!CreateShader())
	{
		return false;
	}

	return true;
	}
