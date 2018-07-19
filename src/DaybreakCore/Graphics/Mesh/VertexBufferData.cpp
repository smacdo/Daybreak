#include "stdafx.h"
#include "VertexBufferData.h"
#include "Graphics/Mesh/VertexFormat.h"
#include "Common/Error.h"

using namespace Daybreak;

//---------------------------------------------------------------------------------------------------------------------
VertexBufferData::VertexBufferData(
    _In_ std::unique_ptr<uint8_t[]> rawData,
    _In_ std::shared_ptr<VertexFormat> elementType,
    _In_ size_t elementCount)
    : BufferData(elementCount, std::move(rawData)),
      m_elementType(elementType)
{
    CHECK_NOT_NULL(rawData);
    CHECK_NOT_NULL(elementType);
    CHECK_NOT_ZERO(elementCount);
}

//---------------------------------------------------------------------------------------------------------------------
VertexBufferData::VertexBufferData(_In_ std::shared_ptr<VertexFormat> elementType)
    : BufferData(),
      m_elementType(elementType)
{
    CHECK_NOT_NULL(elementType);
}

//---------------------------------------------------------------------------------------------------------------------
VertexBufferData::~VertexBufferData() = default;
