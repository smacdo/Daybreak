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

            /** Check if material assigned. */
            bool hasMaterial() const noexcept { return m_material != nullptr; }

            /** Get reference to the material. */
            const MaterialData& materialRef() const;

            /** Get a reference to the material. */
            MaterialData& materialRef();

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

        /** Get the number of groups in this model. */
        size_t groupCount() const noexcept;

        /** Get a const reference to a group in this model. */
        const Group& group(size_t groupIndex) const;

        /** Get reference to a group in this model. */
        Group& group(size_t groupIndex);

        /** Get a const reference to the list of groups in this model. */
        const std::vector<Group>& groups() const noexcept;

        /** Get a reference to the list of groups in this model. */
        std::vector<Group>& groups() noexcept;

    private:
        std::unique_ptr<MeshData> m_mesh;
        std::vector<Group> m_groups;
    };
}
