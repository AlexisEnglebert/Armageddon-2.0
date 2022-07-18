#pragma once
#include "../../Macros.h"
#include "../../Log.h"
#include "../OffScreenRenderTargetView.h"
#include "../Interface.h"
#include "../../Material/AssetManager.h"
#include "../Shaders.h"
#include "../../Material/Texture.h"
#include "../Mesh.h"
#include "../Renderer2D.h"
#include "../ConstantBuffer.h"
#include <DirectXMath.h>
namespace Armageddon
{
	struct BloomConstantBuffer
	{
		DirectX::XMFLOAT2 TexelSize = { 1.0F,1.0F };
	};
#define BLOOM_PASS_COUNT 8
	class DECL Bloom
	{
	public:
		Bloom();
		OffScreenRenderTarget m_BloomTexture;
		Armageddon::PixelShaders  px;
		Armageddon::VertexShaders vx;
		void Render();
		RenderTexture m_bloomUpSample[BLOOM_PASS_COUNT];
		ConstantBuffer<BloomConstantBuffer> m_BloomConstant;
		Armageddon::BloomConstantBuffer BloomPropety;
	private:
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> TextureRessourceView;
		Microsoft::WRL::ComPtr<ID3D11Resource> TextureRessource;
		ID3D11RenderTargetView* RenderTargetView;
		Mesh m_quad = Armageddon::Renderer2D::GeneratePlane();
		ID3D11BlendState* m_BlendState;
		RenderTexture m_bloomDownSample[BLOOM_PASS_COUNT];
		Armageddon::PixelShaders  Bloompx;
		Armageddon::PixelShaders  BloomUpsample;
		Armageddon::VertexShaders Bloomvx;

	};


}