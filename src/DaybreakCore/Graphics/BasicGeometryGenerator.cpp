#include "stdafx.h"
#include "BasicGeometryGenerator.h"
#include "Graphics/Mesh/IndexBufferData.h"
#include "Graphics/Mesh/VertexBufferData.h"
#include "Graphics/Mesh/TypedMeshData.h"

using namespace Daybreak;

//---------------------------------------------------------------------------------------------------------------------
std::unique_ptr<MeshData> BasicGeometryGenerator::MakeCube()
{
    const size_t CubeVerticesCount = 36;
    const size_t CubeIndicesCount = 36;

    const vertex_ptn_t CubeVertices[CubeVerticesCount] =
    {
        //  x      y      z       u     v     nx     ny     nz
        { -0.5f, -0.5f, -0.5f,   0.0f, 0.0f,   0.0f,  0.0f, -1.0f },          // Front
        {  0.5f, -0.5f, -0.5f,   1.0f, 0.0f,   0.0f,  0.0f, -1.0f },
        {  0.5f,  0.5f, -0.5f,   1.0f, 1.0f,   0.0f,  0.0f, -1.0f },
        {  0.5f,  0.5f, -0.5f,   1.0f, 1.0f,   0.0f,  0.0f, -1.0f },
        { -0.5f,  0.5f, -0.5f,   0.0f, 1.0f,   0.0f,  0.0f, -1.0f },
        { -0.5f, -0.5f, -0.5f,   0.0f, 0.0f,   0.0f,  0.0f, -1.0f },

        { -0.5f, -0.5f,  0.5f,   0.0f, 0.0f,   0.0f,  0.0f,  1.0f },          // Back
        {  0.5f, -0.5f,  0.5f,   1.0f, 0.0f,   0.0f,  0.0f,  1.0f },
        {  0.5f,  0.5f,  0.5f,   1.0f, 1.0f,   0.0f,  0.0f,  1.0f },
        {  0.5f,  0.5f,  0.5f,   1.0f, 1.0f,   0.0f,  0.0f,  1.0f },
        { -0.5f,  0.5f,  0.5f,   0.0f, 1.0f,   0.0f,  0.0f,  1.0f },
        { -0.5f, -0.5f,  0.5f,   0.0f, 0.0f,   0.0f,  0.0f,  1.0f },

        { -0.5f,  0.5f,  0.5f,   1.0f, 0.0f,  -1.0f,  0.0f,  0.0f },          // Left
        { -0.5f,  0.5f, -0.5f,   1.0f, 1.0f,  -1.0f,  0.0f,  0.0f },
        { -0.5f, -0.5f, -0.5f,   0.0f, 1.0f,  -1.0f,  0.0f,  0.0f },
        { -0.5f, -0.5f, -0.5f,   0.0f, 1.0f,  -1.0f,  0.0f,  0.0f },
        { -0.5f, -0.5f,  0.5f,   0.0f, 0.0f,  -1.0f,  0.0f,  0.0f },
        { -0.5f,  0.5f,  0.5f,   1.0f, 0.0f,  -1.0f,  0.0f,  0.0f },

        { 0.5f,  0.5f,  0.5f,   1.0f, 0.0f,   1.0f,  0.0f,  0.0f },          // Right
        { 0.5f,  0.5f, -0.5f,   1.0f, 1.0f,   1.0f,  0.0f,  0.0f },
        { 0.5f, -0.5f, -0.5f,   0.0f, 1.0f,   1.0f,  0.0f,  0.0f },
        { 0.5f, -0.5f, -0.5f,   0.0f, 1.0f,   1.0f,  0.0f,  0.0f },
        { 0.5f, -0.5f,  0.5f,   0.0f, 0.0f,   1.0f,  0.0f,  0.0f },
        { 0.5f,  0.5f,  0.5f,   1.0f, 0.0f,   1.0f,  0.0f,  0.0f },

        { -0.5f, -0.5f, -0.5f,   0.0f, 1.0f,   0.0f, -1.0f,  0.0f },          // Bottom
        {  0.5f, -0.5f, -0.5f,   1.0f, 1.0f,   0.0f, -1.0f,  0.0f },
        {  0.5f, -0.5f,  0.5f,   1.0f, 0.0f,   0.0f, -1.0f,  0.0f },
        {  0.5f, -0.5f,  0.5f,   1.0f, 0.0f,   0.0f, -1.0f,  0.0f },
        { -0.5f, -0.5f,  0.5f,   0.0f, 0.0f,   0.0f, -1.0f,  0.0f },
        { -0.5f, -0.5f, -0.5f,   0.0f, 1.0f,   0.0f, -1.0f,  0.0f },

        { -0.5f,  0.5f, -0.5f,   0.0f, 1.0f,   0.0f,  1.0f,  0.0f },          // Top
        {  0.5f,  0.5f, -0.5f,   1.0f, 1.0f,   0.0f,  1.0f,  0.0f },
        {  0.5f,  0.5f,  0.5f,   1.0f, 0.0f,   0.0f,  1.0f,  0.0f },
        {  0.5f,  0.5f,  0.5f,   1.0f, 0.0f,   0.0f,  1.0f,  0.0f },
        { -0.5f,  0.5f,  0.5f,   0.0f, 0.0f,   0.0f,  1.0f,  0.0f },
        { -0.5f,  0.5f, -0.5f,   0.0f, 1.0f,   0.0f,  1.0f,  0.0f }
    };

    const index_element_ui32 CubeIndices[CubeIndicesCount] =
    {
        0,  1,  2,  3,  4,  5,
        6,  7,  8,  9, 10, 11,
        12, 13, 14, 15, 16, 17,
        18, 19, 20, 21, 22, 23,
        24, 25, 26, 27, 28, 29,
        30, 31, 32, 33, 34, 35
    };

    // Copy vertex and index data to new mesh.
    std::unique_ptr<vertex_ptn_t[]> vertices(new vertex_ptn_t[CubeVerticesCount]);
    std::copy(std::begin(CubeVertices), std::end(CubeVertices), vertices.get());

    std::unique_ptr<unsigned int[]> indices(new unsigned int[CubeIndicesCount]);
    std::copy(std::begin(CubeIndices), std::end(CubeIndices), indices.get());

    return std::make_unique<MeshData_ptn>(
        std::make_unique<IndexBufferData>(CubeIndicesCount, std::move(indices)),
        std::make_unique<TVertexBufferData<vertex_ptn_t>>(
            std::vector<vertex_ptn_t> { std::begin(CubeVertices), std::end(CubeVertices) })
    );
}
