#include "stdafx.h"
#include "ModelData.h"
#include "Graphics/Mesh/MeshData.h"
#include "Common/Error.h"

using namespace Daybreak;

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
