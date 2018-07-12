#pragma once
#include "Graphics/Mesh/BufferData.h"
#include "Graphics/Mesh/IndexBufferElement.h"

#include <memory>
#include <vector>
#include <cstdint>

namespace Daybreak
{
    /// Index buffer with data accessile to the CPU (rather than uploaded to the GPU).
    class IndexBufferData : public BufferData
    {
    public:
        /// U8 or untyped uchar* data constructor.
        IndexBufferData(
            _In_ size_t elementCount,
            _In_ std::unique_ptr<uint8_t[]> rawData,
            _In_ IndexElementType elementType = IndexElementType::UnsignedByte);

        /// U16 constructor.
        IndexBufferData(
            _In_ size_t elementCount,
            _In_ std::unique_ptr<uint16_t[]> rawData);

        /// U32 constructor.
        IndexBufferData(
            _In_ size_t elementCount,
            _In_ std::unique_ptr<uint32_t[]> rawData);

        /// Destructor.
        virtual ~IndexBufferData();

        /// Get the type of elments in the buffer.
        IndexElementType elementType() const noexcept { return m_elementType; }

    protected:
        /// Index element type definition.
        IndexElementType m_elementType;
    };
}
