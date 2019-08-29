#pragma once
#include <string>
#include <vector>

namespace Daybreak
{
    class MeshData;
    class MaterialData;

    /** 3d model that combines a mesh with a material. */
    class ModelData
    {
    public:
        class Group
        {
        public:
            Group(
                const std::string& name,
                std::shared_ptr<MaterialData> material,
                size_t indexOffset,
                size_t indexCount);

        private:
            std::string m_name;
            std::shared_ptr<MaterialData> m_material;
            size_t m_indexOffset;
            size_t m_indexCount;
        };

    public:
        /** Constructor. */
        ModelData(std::unique_ptr<MeshData> mesh);

        /** Destructor. */
        ~ModelData();

        /** Get unowned reference to mesh data. */
        const MeshData& mesh() const noexcept;

        /** Add a group to the model. */

    private:
        std::unique_ptr<MeshData> m_mesh;
        std::vector<Group> m_groups;
    };
}
