#include "stdafx.h"
#include "Mesh.h"

#include "Common/Error.h"
#include "Renderer/VertexBuffer.h"
#include "Renderer/IndexBuffer.h"
#include "Renderer/Phong/PhongMaterial.h"

using namespace Daybreak;

//---------------------------------------------------------------------------------------------------------------------
Mesh::Mesh(
    std::shared_ptr<IVertexBuffer> vertexBuffer,
    std::shared_ptr<IIndexBuffer> indexBuffer,
    std::shared_ptr<PhongMaterial> material)
    : m_vertexBuffer(vertexBuffer),
      m_indexBuffer(indexBuffer),
      m_material(material)
{
    CHECK_NOT_NULL(vertexBuffer);
    CHECK_NOT_NULL(indexBuffer);
    CHECK_NOT_NULL(material);
}

//---------------------------------------------------------------------------------------------------------------------
Mesh::~Mesh() = default;
