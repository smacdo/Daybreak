#include "stdafx.h"
#include "BufferData.h"
#include "Common/Error.h"

using namespace Daybreak;

//---------------------------------------------------------------------------------------------------------------------
BufferData::BufferData()
    : m_bytes(nullptr, defaultDelete),
      m_byteCount(0)
{
}

//---------------------------------------------------------------------------------------------------------------------
BufferData::BufferData(
    _In_ size_t byteCount,
    _In_ std::unique_ptr<uint8_t[]> bytes)
    : m_bytes(bytes.release(), defaultDelete),
      m_byteCount(byteCount)
{
}

//---------------------------------------------------------------------------------------------------------------------
BufferData::~BufferData() = default;

//---------------------------------------------------------------------------------------------------------------------
void BufferData::setUnownedDataPtr(_In_ size_t byteCount, _In_ void * bytes) noexcept
{
    m_bytes = std::unique_ptr<uint8_t[], void(*)(uint8_t*)>(reinterpret_cast<uint8_t*>(bytes), noDelete);
    m_byteCount = byteCount;
}

//---------------------------------------------------------------------------------------------------------------------
void BufferData::defaultDelete(uint8_t * data) noexcept
{
    delete[] data;
}

//---------------------------------------------------------------------------------------------------------------------
void BufferData::noDelete(uint8_t *) noexcept
{
    // Empty.
}
