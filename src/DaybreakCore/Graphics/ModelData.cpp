#include "stdafx.h"
#include "ModelData.h"
#include "Graphics/Mesh/MeshData.h"
#include "Common/Error.h"

using namespace Daybreak;

//---------------------------------------------------------------------------------------------------------------------
ModelData::Group::Group(
    const std::string& name,
    std::shared_ptr<MaterialData> material,
    uint32_t indexOffset,
    uint32_t indexCount)
    : m_name(name),
      m_material(material),
      m_indexOffset(indexOffset),
      m_indexCount(indexCount)
{
    CHECK_NOT_EMPTY(name);
    CHECK_NOT_NULL(material);
}

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
void ModelData::addGroup(Group group)
{
    CHECK(group.indexOffset() <= m_mesh->indexCount());
    CHECK(group.indexOffset() + group.indexCount() <= m_mesh->indexCount());
    m_groups.emplace_back(group);
}
