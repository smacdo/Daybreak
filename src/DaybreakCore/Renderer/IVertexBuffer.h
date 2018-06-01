#pragma once
#include "BindableResource.h"

namespace Daybreak
{
    // Graphics API independent vertex buffer.
    class IVertexBuffer : public IBindableResource
    {
    public:
        // Destructor.
        virtual ~IVertexBuffer() = default;
    };
}