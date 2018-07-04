#pragma once
#include <memory>

namespace Daybreak
{
    class MeshData;

    // Generates meshes for basic geometric shapes like cubes and spheres.
    //  TODO: Pass back meshes with typed vertex/index buffers rather than just array of floats.
    class BasicGeometryGenerator
    {
    public:
        static std::unique_ptr<MeshData> MakeCube();
    };
}
