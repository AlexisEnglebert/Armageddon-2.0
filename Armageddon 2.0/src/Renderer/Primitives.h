#pragma once
#include "../Macros.h"
#include "Mesh.h"


namespace Armageddon
{
	class DECL Primitives
	{
	public:
		Primitives();
		static Mesh GenerateCube();
		static Mesh GenerateSphere(int radius , int stackCount, int sectorCount);
		static Mesh GenerateTorus(int segmentradius, int mainradius, int semgent, int base);
		static Mesh Sphere;
	private:
	};
}