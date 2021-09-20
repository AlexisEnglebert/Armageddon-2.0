#include "Renderer2D.h"

Mesh Armageddon::Renderer2D::GeneratePlane()
{
    std::vector<Vertex>vertices=
    {
	  Vertex(-1.0f, -1.0f, 0.0f ,0.0f,  1.0f),
	  Vertex(-1.0f,  1.0f, 0.0f ,0.0f,  0.0f),
	  Vertex(1.0f,   1.0f, 0.0f ,1.0f,  0.0f),
	  Vertex(1.0f,  -1.0f, 0.0f , 1.0f, 1.0f)
    };

	std::vector<DWORD> indices=
	{
		 0,  1,  2,
		 0,  2,  3,
	};


	auto m_quad = Mesh(vertices, indices);
	return m_quad;
}
