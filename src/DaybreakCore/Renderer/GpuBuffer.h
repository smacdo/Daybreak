#pragma once
#include "GpuResource.h"

namespace Daybreak
{
    // GPU data buffer interface (vertex, index, constant etc buffer).
    class GpuBuffer : public GpuResource
    {
    public:
        GpuBuffer() = default;
        virtual ~GpuBuffer() = default;

    private:
        // Copy and assignment for GPU resources is not allowed.
        GpuBuffer(const GpuBuffer&) = delete;
        GpuBuffer& operator =(const GpuBuffer&) = delete;
    };
}
