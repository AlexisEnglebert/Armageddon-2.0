#include "Bloom.h"

Armageddon::Bloom::Bloom()
{
	
}

void Armageddon::Bloom::Init()
{
	m_BloomTexture.Init(1920, 1080, DXGI_FORMAT_R16G16B16A16_FLOAT);
	px = AssetManager::GetOrCreatePixelShader(L"Assets/Shaders/BloomThresholdPixel.cso");
	vx = AssetManager::GetOrCreateVertexShader(L"Assets/Shaders/BloomThresholdVertex.cso");
	Bloomvx = AssetManager::GetOrCreateVertexShader(L"Assets/Shaders/BloomThresholdVertex.cso");
	Bloompx = AssetManager::GetOrCreatePixelShader(L"Assets/Shaders/BloomDownSamplePixel.cso");
	BloomUpsample = AssetManager::GetOrCreatePixelShader(L"Assets/Shaders/BloomUpsample.cso");
	m_BloomConstant.Create(D3D11_USAGE_DYNAMIC, 4);
	for (UINT i = 0; i < BLOOM_PASS_COUNT; i++)
	{
		float width = 960 * pow(0.5, i);
		float height = 540 * pow(0.5, i);
		m_bloomDownSample[i] = { width,height,DXGI_FORMAT_R16G16B16A16_FLOAT };
		m_bloomUpSample[i] = { width,height,DXGI_FORMAT_R16G16B16A16_FLOAT };
	}
	m_quad = Armageddon::Renderer2D::GeneratePlane();
}
void Armageddon::Bloom::Render()
{
	ID3D11ShaderResourceView* null[] = { nullptr, nullptr , nullptr , nullptr , nullptr , nullptr , nullptr , nullptr , nullptr , nullptr };
	Armageddon::Interface::GetDeviceContext()->PSSetShaderResources(0, 10, null);

	Armageddon::Interface::GetDeviceContext()->PSSetSamplers(0, 1, Armageddon::Interface::GetClampSampler().GetAddressOf());

	Armageddon::Interface::GetDeviceContext()->VSSetShader(Bloomvx.GetShader(), NULL, 0);
	Armageddon::Interface::GetDeviceContext()->PSSetShader(Bloompx.GetShader(), NULL, 0);

	for (UINT i = 0; i < BLOOM_PASS_COUNT; i++)
	{

		float factor = i <= 0 ? i : i - 1;
		float width = 1920.0F * pow(0.5, i);
		float height = 1080.0F * pow(0.5, i);

		D3D11_VIEWPORT viewport;
		ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

		viewport.TopLeftX = 0;
		viewport.TopLeftY = 0;
		viewport.Width = width * 0.5f;
		viewport.Height = height * 0.5f;
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;

		Armageddon::Interface::GetDeviceContext()->RSSetViewports(1, &viewport);

		float BackGroundColor[] = { 0.1f,0.1f,0.1f,1.0f };
		ID3D11ShaderResourceView* null[] = { nullptr, nullptr , nullptr , nullptr , nullptr , nullptr , nullptr , nullptr , nullptr , nullptr };
		Armageddon::Interface::GetDeviceContext()->PSSetShaderResources(0, 10, null);
		ID3D11RenderTargetView* RDTVNULL = nullptr;
		Armageddon::Interface::GetDeviceContext()->OMSetRenderTargets(1, &RDTVNULL, nullptr);

		Armageddon::Interface::GetDeviceContext()->OMSetRenderTargets(1, &m_bloomDownSample[i].RenderTargetView, nullptr);
		Armageddon::Interface::GetDeviceContext()->PSSetShaderResources(0, 10, null);
		
		BloomPropety.TexelSize = DirectX::XMFLOAT2(float(1.0F / width), float(1.0F / height));
		m_BloomConstant.SetDynamicData(&BloomPropety);
		m_BloomConstant.BindPS();
		

		if (i == 0)
		{
			Armageddon::Interface::GetDeviceContext()->PSSetShaderResources(0, 1, m_BloomTexture.GetRessourceViewPtr());
		}
		else
		{
			Armageddon::Interface::GetDeviceContext()->PSSetShaderResources(0, 1, m_bloomDownSample[i - 1].GetRessourceViewPtr());

		}

		for (auto& submesh : m_quad.v_SubMeshes)
		{
			submesh.BindVertexBuffer();
			submesh.BindIndexBuffer();
			submesh.DrawIndexed();
		}
	}
	Armageddon::Interface::GetDeviceContext()->PSSetShader(BloomUpsample.GetShader(), NULL, 0);

	for (int i = BLOOM_PASS_COUNT - 2; i >= 0; i--)
	{
		Armageddon::Interface::GetDeviceContext()->OMSetBlendState(Armageddon::Interface::GetColorBlendState().Get(), {}, UINT32_MAX);

		float width = 1920.0F * pow(0.5, i);
		float height = 1080.0F * pow(0.5, i);

		D3D11_VIEWPORT viewport;
		ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

		viewport.TopLeftX = 0;
		viewport.TopLeftY = 0;
		viewport.Width = width * 0.5f;
		viewport.Height = height * 0.5f;
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;

		Armageddon::Interface::GetDeviceContext()->RSSetViewports(1, &viewport);
		float BackGroundColor[] = { 0.1f,0.1f,0.1f,1.0f };
		ID3D11ShaderResourceView* null[] = { nullptr, nullptr , nullptr , nullptr , nullptr , nullptr , nullptr , nullptr , nullptr , nullptr };
		Armageddon::Interface::GetDeviceContext()->PSSetShaderResources(0, 10, null);
		ID3D11RenderTargetView* RDTVNULL = nullptr;
		Armageddon::Interface::GetDeviceContext()->OMSetRenderTargets(1, &RDTVNULL, nullptr);

		Armageddon::Interface::GetDeviceContext()->OMSetRenderTargets(1, &m_bloomUpSample[i].RenderTargetView, nullptr);
		Armageddon::Interface::GetDeviceContext()->PSSetShaderResources(0, 10, null);
		float TexWidth = (width * 0.5) * 0.5;
		float TexHeight = (height * 0.5) * 0.5;
		BloomPropety.TexelSize = DirectX::XMFLOAT2(float(1.0F / TexWidth), float(1.0F / TexHeight));
		m_BloomConstant.SetDynamicData(&BloomPropety);
		m_BloomConstant.BindPS();

		// +1 celui on est
		if (i == BLOOM_PASS_COUNT-1)
		{
			Armageddon::Interface::GetDeviceContext()->PSSetShaderResources(0, 1, m_bloomDownSample[i + 1].GetRessourceViewPtr());
			Armageddon::Interface::GetDeviceContext()->PSSetShaderResources(1, 1, m_bloomDownSample[i].GetRessourceViewPtr());

		}
		else
		{
			Armageddon::Interface::GetDeviceContext()->PSSetShaderResources(0, 1, m_bloomDownSample[i + 1].GetRessourceViewPtr());
			Armageddon::Interface::GetDeviceContext()->PSSetShaderResources(1, 1, m_bloomUpSample[i + 1].GetRessourceViewPtr());

		}

		for (auto& submesh : m_quad.v_SubMeshes)
		{
			submesh.BindVertexBuffer();
			submesh.BindIndexBuffer();
			submesh.DrawIndexed();
		}
	}


	ID3D11RenderTargetView* RDTVNULL = nullptr;
	Armageddon::Interface::GetDeviceContext()->OMSetRenderTargets(1, &RDTVNULL, nullptr);
} 
