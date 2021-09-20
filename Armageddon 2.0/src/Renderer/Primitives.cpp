#include "Primitives.h"

Mesh Armageddon::Primitives::GenerateCube()
{
    std::vector<Vertex> vertices;
    vertices.resize(24);
   
    //Front FACE  --------------------------------------
    vertices[0].Position = { -1.0f, -1.0f, -1.0f };
    vertices[0].TexCoord = { 0.0f, 1.0f };
    vertices[0].Normal   = { -1.0f, -1.0f, -1.0f };

    vertices[1].Position = { -1.0f,  1.0f, -1.0f };
    vertices[1].TexCoord = { 0.0f, 0.0f };
    vertices[1].Normal   =  { -1.0f, 1.0f, -1.0f };

    vertices[2].Position = { 1.0f,  1.0f, -1.0f };
    vertices[2].TexCoord = { 1.0f, 0.0f };
    vertices[2].Normal   = {  1.0f, 1.0f, -1.0f };

    vertices[3].Position = { 1.0f, -1.0f, -1.0f };
    vertices[3].TexCoord = { 1.0f, 1.0f };
    vertices[3].Normal   = {  1.0f, -1.0f, -1.0f };
    //Other face --------------------------------------

    vertices[4].Position = { -1.0f, -1.0f, 1.0f };
    vertices[4].TexCoord = { 1.0f, 1.0f };
    vertices[4].Normal   = { -1.0f, -1.0f, 1.0f };

    vertices[5].Position = { 1.0f, -1.0f, 1.0f };
    vertices[5].TexCoord = { 0.0f, 1.0f };
    vertices[5].Normal   = { 1.0f, -1.0f, 1.0f };

    vertices[6].Position = { 1.0f,  1.0f, 1.0f };
    vertices[6].TexCoord = { 0.0f, 0.0f };
    vertices[6].Normal   = {  1.0f, 1.0f, 1.0f };

    vertices[7].Position = { -1.0f,  1.0f, 1.0f };
    vertices[7].TexCoord = { 1.0f, 0.0f};
    vertices[7].Normal   = {  -1.0f, 1.0f, 1.0f };

	//Other face  --------------------------------------

    vertices[8].Position = { -1.0f, 1.0f, -1.0f };
    vertices[8].TexCoord = { 0.0f, 1.0f };
    vertices[8].Normal   = {  -1.0f, 1.0f, -1.0f };

    vertices[9].Position = { -1.0f, 1.0f,  1.0f };
    vertices[9].TexCoord = { 0.0f, 0.0f };
    vertices[9].Normal   = { -1.0f, 1.0f, 1.0f };

    vertices[10].Position = { 1.0f, 1.0f,  1.0f };
    vertices[10].TexCoord = { 1.0f, 0.0f };
    vertices[10].Normal   = {  1.0f, 1.0f, 1.0f };

    vertices[11].Position = { 1.0f, 1.0f, -1.0f };
    vertices[11].TexCoord = { 1.0f, 1.0f };
    vertices[11].Normal   = {  1.0f, 1.0f, -1.0f };
	//Other face  --------------------------------------

    vertices[12].Position = { -1.0f, -1.0f, -1.0f };
    vertices[12].TexCoord = { 1.0f, 1.0f};
    vertices[12].Normal   = { -1.0f, -1.0f, -1.0f };

    vertices[13].Position = { 1.0f, -1.0f, -1.0f };
    vertices[13].TexCoord = { 0.0f, 1.0f };
    vertices[13].Normal = {  1.0f, -1.0f, -1.0f };

    vertices[14].Position = { 1.0f, -1.0f,  1.0f };
    vertices[14].TexCoord = { 0.0f, 0.0f };
    vertices[14].Normal   = {  1.0f, -1.0f, 1.0f };

    vertices[15].Position = { -1.0f, -1.0f,  1.0f };
    vertices[15].TexCoord = { 1.0f, 0.0f };
    vertices[15].Normal   = {-1.0f, -1.0f,  1.0f };
	//Other face  --------------------------------------

    vertices[16].Position = { -1.0f, -1.0f,  1.0f };
    vertices[16].TexCoord = { 0.0f, 1.0f };
    vertices[16].Normal = {-1.0f, -1.0f,  1.0f };

    vertices[17].Position = { -1.0f,  1.0f,  1.0f };
    vertices[17].TexCoord = { 0.0f, 0.0f };
    vertices[17].Normal = {-1.0f,  1.0f,  1.0f };

    vertices[18].Position = { -1.0f,  1.0f, -1.0f };
    vertices[18].TexCoord = { 1.0f, 0.0f };
    vertices[18].Normal = { -1.0f,  1.0f, -1.0f };

    vertices[19].Position = { -1.0f, -1.0f, -1.0f };
    vertices[19].TexCoord = { 1.0f, 1.0f };
    vertices[19].Normal = {-1.0f, -1.0f, -1.0f };

	//Other face  --------------------------------------

    vertices[20].Position = { 1.0f, -1.0f, -1.0f };
    vertices[20].TexCoord = { 0.0f, 1.0f };
    vertices[20].Normal   = { 1.0f, -1.0f, -1.0f };

    vertices[21].Position = { 1.0f,  1.0f, -1.0f };
    vertices[21].TexCoord = { 0.0f, 0.0f};
    vertices[21].Normal   = {  1.0f,  1.0f, -1.0f };

    vertices[22].Position = { 1.0f,  1.0f,  1.0f };
    vertices[22].TexCoord = { 1.0f, 0.0f, };
    vertices[22].Normal   = { 1.0f,  1.0f,  1.0f };

    vertices[23].Position = { 1.0f, -1.0f,  1.0f };
    vertices[23].TexCoord = { 1.0f, 1.0f };
    vertices[23].Normal   = {1.0f, -1.0f,  1.0f };

    std::vector<DWORD> indices = {

        // Front Face
         0,  1,  2,
         0,  2,  3,

         // Back Face
         4,  5,  6,
         4,  6,  7,

         // Top Face
         8,  9, 10,
         8, 10, 11,

         // Bottom Face
         12, 13, 14,
         12, 14, 15,

         // Left Face
         16, 17, 18,
         16, 18, 19,

         // Right Face
         20, 21, 22,
         20, 22, 23
    };

  return Mesh(vertices, indices);
    
}

Mesh Armageddon::Primitives::GenerateSphere(int radius = 10,int stackCount = 10,int sectorCount = 10)
{
 
   
    std::vector<Vertex> vertices;
    std::vector<DWORD> indices;

    UINT n = (stackCount * sectorCount) + 2 ;
    float invRadius = 1/radius;


    vertices.push_back(Vertex(0.0f, (float)+radius, 0.0f,0.0f, 0.0f, 0.0f,(float)radius, 0.0f));

    float phiStep = XM_PI / stackCount;
    float thetaStep = 2.0f * XM_PI / sectorCount;

    for (UINT i = 1; i <= stackCount - 1; ++i)
    {
        float phi = i * phiStep;

        for (UINT j = 0; j <= sectorCount; ++j)
        {
            float theta = j * thetaStep;

         
            vertices.push_back(Vertex(
                radius * sinf(phi) * cosf(theta),
                radius * cosf(phi),
                radius * sinf(phi) * sinf(theta), 
                theta / XM_2PI,
                 phi / XM_PI,
                (radius * sinf(phi) * cosf(theta)) * invRadius,
                radius * cosf(phi) * invRadius,
               (radius * sinf(phi) * sinf(theta), theta / XM_2PI, phi / XM_PI) * invRadius));
        }
    }

    vertices.push_back(Vertex(0.0f, -radius, 0.0f));



    for (UINT i = 1; i <= sectorCount; ++i)
    {
        indices.push_back(0);
        indices.push_back(i + 1);
        indices.push_back(i);
    }

    UINT baseIndex = 1;
    UINT ringVertexCount = sectorCount + 1;
    for (UINT i = 0; i < stackCount - 2; ++i)
    {
        for (UINT j = 0; j < sectorCount; ++j)
        {
            indices.push_back(baseIndex + i * ringVertexCount + j);
            indices.push_back(baseIndex + i * ringVertexCount + j + 1);
            indices.push_back(baseIndex + (i + 1) * ringVertexCount + j);

            indices.push_back(baseIndex + (i + 1) * ringVertexCount + j);
            indices.push_back(baseIndex + i * ringVertexCount + j + 1);
            indices.push_back(baseIndex + (i + 1) * ringVertexCount + j + 1);
        }
    }

    UINT southPoleIndex = n - 1;

    baseIndex = southPoleIndex - ringVertexCount;

    for (UINT i = 0; i < sectorCount; ++i)
    {
        indices.push_back(southPoleIndex);
        indices.push_back(baseIndex + i);
        indices.push_back(baseIndex + i + 1);
    }

    return Mesh(vertices, indices);
}

Mesh Armageddon::Primitives::GenerateTorus(int segmentradius,int mainradius,int semgent, int base)
{
    std::vector<Vertex> vertices;
    std::vector<DWORD> indices;

   

    return Mesh(vertices, indices);

}

