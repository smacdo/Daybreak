#pragma once
#include "GpuBuffer.h"
#include "Graphics/Mesh/IndexBufferElement.h"
#include <cstdint>

namespace Daybreak
{
    /// GPU index buffer.
    class IndexBuffer : public GpuBuffer
    {
    public:
        // Get the number of elements in the buffer.
        virtual size_t elementCount() const noexcept = 0;

        // Get the type of elments in the buffer.
        virtual IndexElementType elementType() const noexcept = 0;
    };
}