#include "stdafx.h"
#include "IndexBufferData.h"
#include "Common/Error.h"

using namespace Daybreak;

//---------------------------------------------------------------------------------------------------------------------
IndexBufferData::IndexBufferData(
    _In_ size_t byteCount,
    _In_ std::unique_ptr<uint8_t[]> bytes,
    _In_ IndexElementType elementType)
    : BufferData(byteCount, std::move(bytes)),
      m_elementType(elementType)
{
    CHECK_NOT_ZERO(byteCount);
    CHECK_NOT_NULL(m_bytes);

    EXPECT(byteCount % indexElementSizeInBytes(elementType) == 0, "Buffer size must be multiple of index size");
    m_indexCount = byteCount / indexElementSizeInBytes(elementType);
}

//---------------------------------------------------------------------------------------------------------------------
IndexBufferData::IndexBufferData(
    _In_ size_t indexCount,
    _In_ std::unique_ptr<uint16_t[]> indices)
    : IndexBufferData(
        indexCount * 2,
        std::unique_ptr<uint8_t[]>(reinterpret_cast<uint8_t*>(indices.release())),
        IndexElementType::UnsignedShort)
{
}

//---------------------------------------------------------------------------------------------------------------------
IndexBufferData::IndexBufferData(
    _In_ size_t indexCount,
    _In_ std::unique_ptr<uint32_t[]> indices)
    : IndexBufferData(
        indexCount * 4,
        std::unique_ptr<uint8_t[]>(reinterpret_cast<uint8_t*>(indices.release())),
        IndexElementType::UnsignedInt)
{
}

//---------------------------------------------------------------------------------------------------------------------
IndexBufferData::~IndexBufferData() = default;
