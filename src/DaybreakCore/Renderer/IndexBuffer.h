#pragma once
#include "GpuBuffer.h"

namespace Daybreak
{
    /// Type of element stored in an index buffer.
    enum class IndexElementType
    {
        UnsignedByte,       ///< 1 byte index value.
        UnsignedShort,      ///< 2 byte index value.
        UnsignedInt         ///< 4 byte index value.
    };

    /// GPU index buffer.
    class IndexBuffer : public GpuBuffer
    {
    public:
        // Get the number of elements in the buffer.
        virtual size_t elementCount() const noexcept = 0;

        // Get the type of elments in the buffer.
        virtual IndexElementType elementType() const noexcept = 0;

        // Get the size in bytes of an index element type.
        static size_t elementSizeInBytes(IndexElementType elementType);
    };
}