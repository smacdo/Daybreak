#pragma once
#include "GpuBuffer.h"

namespace Daybreak
{
    // Graphics API independent vertex buffer.
    class VertexBuffer : public GpuBuffer
    {
    public:
        // Destructor.
        virtual ~VertexBuffer() = default;

        // Get the number of elements in the buffer.
        virtual size_t elementCount() const noexcept = 0;
    };
}
