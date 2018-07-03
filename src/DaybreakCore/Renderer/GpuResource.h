#pragma once

namespace Daybreak
{
    // A non-copyable resource that resides on the GPU.
    class GpuResource
    {
    public:
        GpuResource() = default;
        virtual ~GpuResource() = default;

    private:
        // Copy and assignment for GPU resources is not allowed.
        GpuResource(const GpuResource&) = delete;
        GpuResource& operator =(const GpuResource&) = delete;
    };
}
