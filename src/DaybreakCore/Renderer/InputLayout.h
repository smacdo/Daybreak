#pragma once
#include "BindableResource.h"

namespace Daybreak
{
    // Abstract base class for input layout.
    class IInputLayout : public IBindableResource
    {
    public:
        // Destructor.
        virtual ~IInputLayout() = default;
    };
}