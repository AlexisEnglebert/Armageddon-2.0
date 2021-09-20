#pragma once
#include "../Macros.h"
#include "Mesh.h"
#include "Vertex.h"
namespace Armageddon
{

	class DECL Renderer2D
	{
	public:
		Renderer2D() = default;
		static Mesh GeneratePlane();

	private:
	};
}