#pragma once
#if __linux__
#else
#include<DirectXMath.h>
#endif

	

class Vertex
{
public:
	Vertex() {}
	Vertex(float x, float y, float z, float u, float v, float nx, float ny, float nz, float tx, float ty, float tz, float btx, float bty, float btz)
		: Position(x,y,z) ,TexCoord(u, v), Normal(nx, ny, nz), Tangent(tx, ty, tz), BiTangent(btx, bty, btz) {}
	Vertex(float x, float y, float z) : Position(x,y,z) {}

	Vertex(float x, float y, float z,float u , float v ) : Position(x,y,z), TexCoord(u,v){}
	Vertex(float x, float y, float z,float u , float v , float nx, float ny, float nz) 
		: Position(x,y,z), TexCoord(u,v) ,Normal(nx,ny,nz){}
	DirectX::XMFLOAT3 Position;
	DirectX::XMFLOAT2 TexCoord;
	DirectX::XMFLOAT3 Normal;
	DirectX::XMFLOAT3 Tangent;
	DirectX::XMFLOAT3 BiTangent;
	UINT boneIDs[200]; //TODO: pour l'instant on accepte 200 bones
	float weights[200];
};