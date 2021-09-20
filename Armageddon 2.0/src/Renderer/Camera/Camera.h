#pragma once
#include "../../Macros.h"
#include "../../Log.h"
#include <d3d11.h>
#include<DirectXMath.h>

#pragma comment(lib,"d3d11.lib")
using namespace DirectX;
namespace Armageddon
{
	class DECL Camera
	{
	public:
		Camera();
		void SetProjectionValues(float FovDegrees, float AspectRatio, float nearZ, float FarZ);
		const DirectX::XMMATRIX& GetViewMatrix()const;
		const DirectX::XMMATRIX& GetProjectionMatrix()const;
		const DirectX::XMFLOAT3& GetPos() const;
		const DirectX::XMVECTOR& GetVecPos() const;

		const DirectX::XMFLOAT3& GetRotation() const;


		void SetPosition(const DirectX::XMVECTOR& pos);
		void SetPosition(float x, float y, float z);

		void AdjustPosition(const DirectX::XMVECTOR& pos);
		void AdjustPosition(float x, float y, float z);

		void SetRotation(const DirectX::XMVECTOR& pos);
		void SetRotation(float x, float y, float z);

		void AdjustRotation(const DirectX::XMVECTOR& pos);
		void AdjustRotation(float x, float y, float z);

		void SetLookAtPos(DirectX::XMFLOAT3 LookPos);

		void SetZoomLevel(float windowWidth, float windowHeight, float ZoomLevel);

		const DirectX::XMVECTOR& GetForwardVec();
		const DirectX::XMVECTOR& GetBackwardVec();
		const DirectX::XMVECTOR& GetLeftVec();
		const DirectX::XMVECTOR& GetRightVec();
		const DirectX::XMVECTOR& GetUpVec();
		const DirectX::XMVECTOR& GetDownVec();

		 float NearZ;
		 float FarZ;
	private:
		void UpdateViewMatrix();
		DirectX::XMVECTOR RotVector;
		DirectX::XMVECTOR posVector;


		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT3 rotation;

		DirectX::XMMATRIX ProjectionMatrix;
		DirectX::XMMATRIX ViewMatrix;

		const DirectX::XMVECTOR DEFAULT_FORWARD_VECTOR = DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f); // Y AXIS = up
		//const DirectX::XMVECTOR DEFAULT_UP_VECTOR =  DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f); // Y AXIS = up

		const DirectX::XMVECTOR DEFAULT_LEFT_VECTOR = DirectX::XMVectorSet(-1.0f, 0.0f, 0.0f, 0.0f); // Y AXIS = up
		const DirectX::XMVECTOR DEFAULT_RIGHT_VECTOR = DirectX::XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f); // Y AXIS = up
		const DirectX::XMVECTOR DEFAULT_BACKWARD_VECTOR = DirectX::XMVectorSet(0.0f, 0.0f, -1.0f, 0.0f); // Y AXIS = up

		const DirectX::XMVECTOR DEFAULT_UP_VECTOR = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f); // Y AXIS = up
		const DirectX::XMVECTOR DEFAULT_DOWN_VECTOR = DirectX::XMVectorSet(0.0f, -1.0f, 0.0f, 0.0f); // Y AXIS = u


		DirectX::XMVECTOR vec_forward;
		DirectX::XMVECTOR vec_left;
		DirectX::XMVECTOR vec_right;
		DirectX::XMVECTOR vec_backward;
		DirectX::XMVECTOR vec_up;
		DirectX::XMVECTOR vec_down;
	};
}