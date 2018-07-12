#include "stdafx.h"
#include "IndexBufferData.h"
#include "Common/Error.h"

using namespace Daybreak;

//---------------------------------------------------------------------------------------------------------------------
IndexBufferData::IndexBufferData(
    _In_ size_t elementCount,
    _In_ std::unique_ptr<uint8_t[]> rawData,
    _In_ IndexElementType elementType)
    : BufferData(std::move(rawData), elementCount),
      m_elementType(elementType)
{
    CHECK_NOT_ZERO(elementCount);
    CHECK_NOT_NULL(m_data);
}

//---------------------------------------------------------------------------------------------------------------------
IndexBufferData::IndexBufferData(
    _In_ size_t elementCount,
    _In_ std::unique_ptr<uint16_t[]> rawData)
    : IndexBufferData(
        elementCount,
        std::unique_ptr<uint8_t[]>(reinterpret_cast<uint8_t*>(rawData.release())),
        IndexElementType::UnsignedShort)
{
}

//---------------------------------------------------------------------------------------------------------------------
IndexBufferData::IndexBufferData(
    _In_ size_t elementCount,
    _In_ std::unique_ptr<uint32_t[]> rawData)
    : IndexBufferData(
        elementCount,
        std::unique_ptr<uint8_t[]>(reinterpret_cast<uint8_t*>(rawData.release())),
        IndexElementType::UnsignedInt)
{
}

//---------------------------------------------------------------------------------------------------------------------
IndexBufferData::~IndexBufferData() = default;
