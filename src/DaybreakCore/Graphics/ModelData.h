#pragma once

namespace Daybreak
{
    class MeshData;

    /** 3d model that combines a mesh with a material. */
    class ModelData
    {
    public:
        /** Constructor. */
        ModelData(std::unique_ptr<MeshData> mesh);

        /** Destructor. */
        ~ModelData();

        /** Get unowned reference to mesh data. */
        const MeshData& mesh() const noexcept;

    private:
        std::unique_ptr<MeshData> m_mesh;
    };
}
