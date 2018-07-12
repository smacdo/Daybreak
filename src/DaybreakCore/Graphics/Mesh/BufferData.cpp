#include "stdafx.h"
#include "BufferData.h"
#include "Common/Error.h"

using namespace Daybreak;

//---------------------------------------------------------------------------------------------------------------------
BufferData::BufferData(
    _In_ std::unique_ptr<uint8_t[]> rawData,
    _In_ size_t elementCount)
    : m_data(std::move(rawData)),
      m_dataUnownedPtr(m_data.get()),
      m_elementCount(elementCount)
{
}

//---------------------------------------------------------------------------------------------------------------------
BufferData::BufferData(_In_ size_t elementCount)
    : m_elementCount(elementCount)
{
}

//---------------------------------------------------------------------------------------------------------------------
BufferData::~BufferData() = default;

//---------------------------------------------------------------------------------------------------------------------
void BufferData::setUnownedDataPtr(_In_ void * unownedDataPtr)
{
    EXPECT(m_data == nullptr, "Cannot set unowned pointer if unique_ptr is being used");
    m_dataUnownedPtr = unownedDataPtr;
}
