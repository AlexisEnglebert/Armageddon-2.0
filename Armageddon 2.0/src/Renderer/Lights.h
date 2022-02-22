#pragma once
#if __linux__
//pas pour l'instant
#else
#include <DirectXMath.h>
#endif
#include "../Macros.h"
struct DECL SkyLight
{
	DirectX::XMFLOAT3 Direction;	//12
	float Intensity;				//4
	DirectX::XMFLOAT3 Color;		//12
	float padding0;					//4

};
struct DECL PointLight
{
	//On alterne pour rester alligner sur 16 bytes
	DirectX::XMFLOAT3 Position; //12
	float Intensity;			//4
	DirectX::XMFLOAT3 Color;	//12
	float Radius;				//4
};

struct DECL SpotLight
{
	DirectX::XMFLOAT3 Position; //12
	float Intensity;			//4
	DirectX::XMFLOAT3 Color;	//12
	float coneAngle;			//4
	DirectX::XMFLOAT3 Direction;//12
	float Padding0;

};

struct DECL DirectionalLight
{
	DirectX::XMFLOAT3 Direction = {1.0f,1.0f,1.0f}; //12
	float Intensity = 0;			//4
	DirectX::XMFLOAT3 Color;	//12
	float padding0;				//4
};