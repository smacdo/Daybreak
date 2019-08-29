#pragma once
#include <string>
#include <vector>
#include <memory>

namespace Daybreak
{
    class MeshData;
    class MaterialData;

    /** 3d model that combines a mesh with a material. */
    class ModelData
    {
    public:
        /** Contains instructions on rendering a group of indices in a model. */
        class Group
        {
        public:
            /** Initialize group. */
            Group(
                const std::string& name,
                std::shared_ptr<MaterialData> material,
                size_t indexOffset,
                size_t indexCount);

            /** Get group name. */
            const std::string& name() const { return m_name; }

            /** Get group material. */
            const std::shared_ptr<MaterialData>& material() const { return m_material; }

            /** Get index buffer offset. */
            size_t indexOffset() const noexcept { return m_indexOffset; }

            /** Get the number of indices that belong to this group. */
            size_t indexCount() const noexcept { return m_indexCount; }

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
        void addGroup(Group&& group);

        /** Add multiple groups to the model. */
        void addGroup(std::vector<Group>&& groups);

    private:
        std::unique_ptr<MeshData> m_mesh;
        std::vector<Group> m_groups;
    };
}
