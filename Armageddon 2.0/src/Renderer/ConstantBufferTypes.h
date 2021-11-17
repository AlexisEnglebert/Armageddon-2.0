#pragma once
#include <DirectXMath.h>
#include "Lights.h"
#include "../Macros.h"
/*attention tout doit être alligné sur 16 bytes donc*/

/*
Buffer Registry: 
			
			1- Transform
			2- Light
			3- SkyBox

*/
struct DECL TransFormBuffer
{
	DirectX::XMMATRIX WorldMat;
	DirectX::XMMATRIX ProjectionMat;
	DirectX::XMMATRIX ViewMat;
	DirectX::XMMATRIX MVP;
};

struct DECL LightBuffer
{
	DirectX::XMFLOAT3 CameraPos; //12 
	float Padding0; // 4 
	int PointLightCount = 0; //4
	int DirectionalLightCount = 0; //4
	DirectX::XMFLOAT2 padding1; //8 

	PointLight PointLights[50];
	DirectionalLight DirectionalLights[50];

	DirectX::XMMATRIX LightViewProjection;
};

struct DECL SkyBoxBuffer
{
	float TextureLOD;
	float Intensity;
	DirectX::XMFLOAT2 padding0; //8 ;
};

struct DECL RoughnessBuffer
{
	float Rougness;
};


struct DECL MaterialBuffer
{
	float Roughness;//4
	float Metalic;//8
	bool UseMetalMap;//9
	float _Padding0;//13
	bool UseEmisive;//14
	DirectX::XMFLOAT3 AlbedoTint = {1.0f,1.0f,1.0f}; //26
	//float _Padding1;//30
	//bool _Padding3; //31
	//bool _Padding2; //32
	float EmisiveFactor;//4
	DirectX::XMFLOAT3 EmisiveTint;//12
};

struct DECL WorldBuffer
{
	float Time;

};