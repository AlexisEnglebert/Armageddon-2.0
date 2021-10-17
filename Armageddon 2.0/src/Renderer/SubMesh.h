#pragma once
#include "../Macros.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Interface.h"
#include"Vertex.h"

class DECL SubMesh
{
public:
	SubMesh() = default;
	SubMesh(std::vector<Vertex>& Vertices, std::vector<DWORD>& indices);
	SubMesh(std::vector<Vertex>& Vertices, std::vector<DWORD>& indices, int32_t index);
	VertexBuffer<Vertex> m_VertexBuffer;
	IndexBuffer<DWORD>   m_IndexBuffer;
	int32_t m_materialIndex;
	void BindVertexBuffer();
	void BindIndexBuffer();
	void DrawIndexed();
private:
};


