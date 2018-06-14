#pragma once
#include "BindableResource.h"

namespace Daybreak
{
    // Graphics API independent index buffer.
    class IIndexBuffer : public IBindableResource
    {
    public:
        // Destructor.
        virtual ~IIndexBuffer() = default;
    };
}