#include "stdafx.h"
#include "VertexBufferData.h"
#include "Graphics/InputLayoutDescription.h"
#include "Common/Error.h"

using namespace Daybreak;

//---------------------------------------------------------------------------------------------------------------------
VertexBufferData::VertexBufferData(
    _In_ size_t byteCount,
    _In_ std::unique_ptr<uint8_t[]> bytes,
    _In_ std::shared_ptr<const InputLayoutDescription> inputLayout)
    : BufferData(byteCount, std::move(bytes)),
      m_inputLayout(inputLayout)
{
    CHECK_NOT_NULL(m_bytes);
    CHECK_NOT_NULL(inputLayout);

    if (byteCount == 0)
    {
        throw DaybreakDataException("Vertex buffer size cannot be zero");
    }

    if (inputLayout->elementSizeInBytes() == 0)
    {
        throw DaybreakDataException("Vertex buffer input layout description cannot be empty");
    }

    if (byteCount % inputLayout->elementSizeInBytes() != 0)
    {
        throw DaybreakDataException("Vertex buffer size must be multiple of vertex size");
    }

    m_vertexCount = byteCount / inputLayout->elementSizeInBytes();
}

//---------------------------------------------------------------------------------------------------------------------
VertexBufferData::~VertexBufferData() = default;
