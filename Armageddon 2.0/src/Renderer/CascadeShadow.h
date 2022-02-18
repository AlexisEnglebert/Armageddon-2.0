#pragma once
#include "../Macros.h"
#include "Camera/Camera.h"
#include "Interface.h"
#include "OffScreenRenderTargetView.h"
#include "../Scene/Scene.h"
#include "../Scene/Entity.h"
#define NUM_CASCADE 4

namespace Armageddon
{

	class DECL CascadeShadow
	{
	public:
		CascadeShadow() = default;
		CascadeShadow(Camera* m_cam, Scene& m_scene);
		void CalculateMatrices();
		void Update();
		OffScreenRenderTarget m_CascadeLightTex;
		DirectX::XMMATRIX LightView = DirectX::XMMatrixIdentity();
		DirectX::XMMATRIX LightProjection = DirectX::XMMatrixIdentity();
		Armageddon::PixelShaders  px ;
		Armageddon::VertexShaders vx ;
		
	private:
		Scene& m_scene;
		Camera* m_pCamera;
		int NUM_CASCADES = 3;
		float m_CascadePartition[3];
		
	};
}