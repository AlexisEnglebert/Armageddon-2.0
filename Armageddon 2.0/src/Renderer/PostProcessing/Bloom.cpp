#include "Bloom.h"

Armageddon::Bloom::Bloom()
{
	m_BloomTexture.Init(Armageddon::Interface::GetDevice().Get(), Armageddon::Interface::GetSwapChain().Get(), 1920, 1080);
	px = AssetManager::GetOrCreatePixelShader(L"..\\bin\\Debug-x64\\Armageddon 2.0\\BloomThresholdPixel.cso");
	vx = AssetManager::GetOrCreateVertexShader(L"..\\bin\\Debug-x64\\Armageddon 2.0\\BloomThresholdVertex.cso");
	Bloomvx = AssetManager::GetOrCreateVertexShader(L"..\\bin\\Debug-x64\\Armageddon 2.0\\BloomThresholdVertex.cso");
	Bloompx = AssetManager::GetOrCreatePixelShader(L"..\\bin\\Debug-x64\\Armageddon 2.0\\BloomDownSamplePixel.cso");
	BloomUpsample = AssetManager::GetOrCreatePixelShader(L"..\\bin\\Debug-x64\\Armageddon 2.0\\BloomUpsample.cso");
	m_BloomConstant.Create(D3D11_USAGE_DYNAMIC, 4);
	for (UINT i = 0; i < 7; i++)
	{
		float width = 960 * pow(0.5, i);
		float height = 540 * pow(0.5, i);
		m_bloomDownSample[i] = { width,height,DXGI_FORMAT_R11G11B10_FLOAT };
		m_bloomUpSample[i] = { width,height,DXGI_FORMAT_R11G11B10_FLOAT };
	}
}

void Armageddon::Bloom::Render()
{
	ID3D11ShaderResourceView* null[] = { nullptr, nullptr , nullptr , nullptr , nullptr , nullptr , nullptr , nullptr , nullptr , nullptr };
	Armageddon::Interface::GetDeviceContext()->PSSetShaderResources(0, 10, null);

	Armageddon::Interface::GetDeviceContext()->PSSetSamplers(0, 1, Armageddon::Interface::GetClampSampler().GetAddressOf());

	Armageddon::Interface::GetDeviceContext()->VSSetShader(Bloomvx.GetShader(), NULL, 0);
	Armageddon::Interface::GetDeviceContext()->PSSetShader(Bloompx.GetShader(), NULL, 0);

	for (UINT i = 0; i < 5; i++)
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
			Armageddon::Interface::GetDeviceContext()->PSSetShaderResources(0, 1, m_BloomTexture.GetAddressOfShaderRessource());
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

	for (UINT i = 3; i > 0; i--)
	{

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

		BloomPropety.TexelSize = DirectX::XMFLOAT2(float(1.0F / width), float(1.0F / height));
		m_BloomConstant.SetDynamicData(&BloomPropety);
		m_BloomConstant.BindPS();

		// +1 celui on est
		if (i == 3)
		{
			Armageddon::Interface::GetDeviceContext()->PSSetShaderResources(0, 1, m_bloomDownSample[i + 1].GetRessourceViewPtr());
			Armageddon::Interface::GetDeviceContext()->PSSetShaderResources(1, 1, m_bloomDownSample[i ].GetRessourceViewPtr());

		}
		else
		{
			Armageddon::Interface::GetDeviceContext()->PSSetShaderResources(0, 1, m_bloomDownSample[i+1].GetRessourceViewPtr());
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
