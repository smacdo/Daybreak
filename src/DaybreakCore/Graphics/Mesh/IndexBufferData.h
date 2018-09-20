#pragma once
#include "Graphics/Mesh/BufferData.h"
#include "Graphics/Mesh/IndexBufferElement.h"

#include <memory>
#include <cstdint>

namespace Daybreak
{
    /// Index buffer with data accessile to the CPU (rather than uploaded to the GPU).
    class IndexBufferData final : public BufferData
    {
    public:
        /// U8 or untyped uchar* data constructor.
        IndexBufferData(
            _In_ size_t byteCount,
            _In_ std::unique_ptr<uint8_t[]> bytes,
            _In_ IndexElementType elementType = IndexElementType::UnsignedByte);

        /// U16 constructor.
        IndexBufferData(
            _In_ size_t indexCount,
            _In_ std::unique_ptr<uint16_t[]> indices);

        /// U32 constructor.
        IndexBufferData(
            _In_ size_t indexCount,
            _In_ std::unique_ptr<uint32_t[]> indices);

        /// Destructor.
        virtual ~IndexBufferData();

        /// Get the type of elments in the buffer.
        IndexElementType elementType() const noexcept { return m_elementType; }

        /// Get the number of indices in the buffer.
        size_t indexCount() const noexcept { return m_indexCount; }

    protected:
        /// Index element type definition.
        IndexElementType m_elementType;

        /// Number of elements in index buffer.
        size_t m_indexCount = 0;
    };
}
