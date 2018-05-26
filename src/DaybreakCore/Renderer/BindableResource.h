#pragma once
#include "Common/Error.h"

namespace Daybreak
{
    // Interface for render objects that have to be bound (activated) to be used by the renderer.
    class IBindableResource
    {
    public:
        IBindableResource() = default;
        virtual ~IBindableResource() = default;
        virtual void bind() = 0;

    private:
        // Copy and assignment for bindable resources is not allowed.
        IBindableResource(const IBindableResource&) = delete;
        IBindableResource& operator =(const IBindableResource&) = delete;
    };
}