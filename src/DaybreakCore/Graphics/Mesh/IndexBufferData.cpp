#include "stdafx.h"
#include "IndexBufferData.h"
#include "Common/Error.h"

using namespace Daybreak;

//---------------------------------------------------------------------------------------------------------------------
IndexBufferData::IndexBufferData(
    _In_ std::unique_ptr<void *> rawData,
    _In_ IndexElementType elementType,
    _In_ size_t elementCount)
    : BufferData(std::move(rawData), elementCount),
      m_elementType(elementType)
{
    CHECK_NOT_NULL(rawData);
    CHECK_NOT_ZERO(elementCount);
}

//---------------------------------------------------------------------------------------------------------------------
IndexBufferData::IndexBufferData(
    _In_ IndexElementType elementType,
    _In_ size_t elementCount)
    : BufferData(elementCount),
      m_elementType(elementType)
{
    CHECK_NOT_ZERO(elementCount);
}

//---------------------------------------------------------------------------------------------------------------------
IndexBufferData::~IndexBufferData() = default;
