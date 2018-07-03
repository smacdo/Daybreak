#pragma once
#include <vector>
#include "Renderer/IndexBuffer.h"

namespace Daybreak
{
    // Generates meshes for basic geometric shapes like cubes and spheres.
    //  TODO: Pass back meshes with typed vertex/index buffers rather than just array of floats.
    class BasicGeometryGenerator
    {
    public:
        using VertexValueType = float;
        using IndexValueType = unsigned int;

        static std::vector<VertexValueType> MakeCubeVertices();
        static std::vector<IndexValueType> MakeCubeIndices();
    };
}
