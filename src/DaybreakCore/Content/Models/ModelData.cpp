#include "stdafx.h"
#include "ModelData.h"
#include "Content\Materials\MaterialData.h"
#include "Graphics/Mesh/MeshData.h"
#include "Common/Error.h"

using namespace Daybreak;

//---------------------------------------------------------------------------------------------------------------------
ModelData::Group::Group(
    const std::string& name,
    std::shared_ptr<MaterialData> material,
    size_t indexOffset,
    size_t indexCount)
    : m_name(name),
      m_material(material),
      m_indexOffset(indexOffset),
      m_indexCount(indexCount)
{
    CHECK_NOT_EMPTY(m_name);
    CHECK_NOT_NULL(m_material);
}

//---------------------------------------------------------------------------------------------------------------------
const MaterialData& ModelData::Group::materialRef() const
{
    CHECK_NOT_NULL(m_material);
    return *(m_material.get());
}

//---------------------------------------------------------------------------------------------------------------------
MaterialData& ModelData::Group::materialRef()
{
    CHECK_NOT_NULL(m_material);
    return *(m_material.get());
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------------------------------------------------------
ModelData::ModelData(std::unique_ptr<MeshData> mesh)
    : m_mesh(std::move(mesh))
{
    CHECK_NOT_NULL(m_mesh);
}

//---------------------------------------------------------------------------------------------------------------------
ModelData::~ModelData() = default;

//---------------------------------------------------------------------------------------------------------------------
const MeshData& ModelData::mesh() const noexcept
{
    return *m_mesh.get();
}

//---------------------------------------------------------------------------------------------------------------------
void ModelData::addGroup(Group&& group)
{
    CHECK(group.indexOffset() <= m_mesh->indexCount());
    CHECK(group.indexOffset() + group.indexCount() <= m_mesh->indexCount());
    m_groups.emplace_back(group);
}

//---------------------------------------------------------------------------------------------------------------------
void ModelData::addGroup(std::vector<Group>&& groups)
{
    for (const auto& group : groups)
    {
        CHECK(group.indexOffset() <= m_mesh->indexCount());
        CHECK(group.indexOffset() + group.indexCount() <= m_mesh->indexCount());
    }
    
    m_groups = std::move(groups);
}

//---------------------------------------------------------------------------------------------------------------------
size_t ModelData::groupCount() const noexcept
{
    return m_groups.size();
}

//---------------------------------------------------------------------------------------------------------------------
const ModelData::Group& ModelData::group(size_t groupIndex) const
{
    CHECK(groupIndex < m_groups.size());
    return m_groups[groupIndex];
}

//---------------------------------------------------------------------------------------------------------------------
ModelData::Group& ModelData::group(size_t groupIndex)
{
    CHECK(groupIndex < m_groups.size());
    return m_groups[groupIndex];
}

//---------------------------------------------------------------------------------------------------------------------
const std::vector<ModelData::Group>& ModelData::groups() const noexcept
{
    return m_groups;
}

//---------------------------------------------------------------------------------------------------------------------
std::vector<ModelData::Group>& ModelData::groups() noexcept
{
    return m_groups;
}
