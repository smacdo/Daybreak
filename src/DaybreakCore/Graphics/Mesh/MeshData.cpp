#include "stdafx.h"
#include "MeshData.h"
#include "Graphics/InputLayoutDescription.h"
#include "Graphics/Mesh/IndexBufferData.h"
#include "Graphics/Mesh/VertexBufferData.h"
#include "Graphics/Mesh/VertexFormat.h"

#include "Common/Error.h"

using namespace Daybreak;

//---------------------------------------------------------------------------------------------------------------------
MeshData::MeshData(
    _In_ std::unique_ptr<IndexBufferData> indexBuffer,
    _In_ std::unique_ptr<VertexBufferData> vertexBuffer)
    : m_indexBuffer(std::move(indexBuffer)),
      m_vertexBuffer(std::move(vertexBuffer))
{
    CHECK_NOT_NULL(m_indexBuffer);
    CHECK_NOT_NULL(m_vertexBuffer);
}

//---------------------------------------------------------------------------------------------------------------------
MeshData::~MeshData() = default;

//---------------------------------------------------------------------------------------------------------------------
const void * MeshData::rawIndexBufferData() const noexcept
{
    return m_indexBuffer->bytes();
}

//---------------------------------------------------------------------------------------------------------------------
size_t MeshData::indexCount() const noexcept
{
    return m_indexBuffer->indexCount();
}

//---------------------------------------------------------------------------------------------------------------------
size_t MeshData::indexElementSizeInBytes() const noexcept
{
    return Daybreak::indexElementSizeInBytes(indexElementType());
}

//---------------------------------------------------------------------------------------------------------------------
IndexElementType MeshData::indexElementType() const noexcept
{
    return m_indexBuffer->elementType();
}

//---------------------------------------------------------------------------------------------------------------------
const void * MeshData::rawVertexBufferData() const noexcept
{
    return m_vertexBuffer->bytes();
}

//---------------------------------------------------------------------------------------------------------------------
size_t MeshData::vertexCount() const noexcept
{
    return m_vertexBuffer->vertexCount();
}

//---------------------------------------------------------------------------------------------------------------------
size_t MeshData::vertexElementSizeInBytes() const noexcept
{
    return m_vertexBuffer->inputLayoutRef().elementSizeInBytes();
}

//---------------------------------------------------------------------------------------------------------------------
const InputLayoutDescription& MeshData::vertexElementTypeRef() const noexcept
{
    return m_vertexBuffer->inputLayoutRef();
}
