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

    // Interface for render objects that have to be bound (activated) with a particular channel or index to be used by
    // the renderer.
    class IBindableSlotResource
    {
    public:
        IBindableSlotResource() = default;
        virtual ~IBindableSlotResource() = default;
        virtual void bind(unsigned int slotIndex) = 0;

    private:
        // Copy and assignment for bindable resources is not allowed.
        IBindableSlotResource(const IBindableSlotResource&) = delete;
        IBindableSlotResource& operator =(const IBindableSlotResource&) = delete;
    };
}