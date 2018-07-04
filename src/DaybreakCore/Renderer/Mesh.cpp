#include "stdafx.h"
#include "Mesh.h"

#include "Common/Error.h"
#include "Renderer/VertexBuffer.h"
#include "Renderer/IndexBuffer.h"
#include "Renderer/Phong/PhongMaterial.h"

using namespace Daybreak;

//---------------------------------------------------------------------------------------------------------------------
Mesh::Mesh(
    _In_ std::shared_ptr<VertexBuffer> vertexBuffer,
    _In_ std::shared_ptr<IndexBuffer> indexBuffer,
    _In_opt_ std::shared_ptr<PhongMaterial> material)
    : m_vertexBuffer(vertexBuffer),
      m_indexBuffer(indexBuffer),
      m_material(material)
{
    CHECK_NOT_NULL(vertexBuffer);
    CHECK_NOT_NULL(indexBuffer);
    // TODO: Maybe enforce material cannot be null and should use a default material for missing.
}

//---------------------------------------------------------------------------------------------------------------------
Mesh::~Mesh() = default;
