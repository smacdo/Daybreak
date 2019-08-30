#pragma once
#include <memory>
#include "Graphics\InputLayoutDescription.h"

namespace Daybreak
{
    // Abstract base class for input layout.
    class InputLayout
    {
    public:
        // Constructor.
        InputLayout(const InputLayoutDescription& layoutDescription);

        // Copy constructor (deleted).
        InputLayout(const InputLayout&) = delete;

        // Destructor.
        virtual ~InputLayout();

        // Assignment operator (deleted).
        InputLayout& operator =(const InputLayout&) = delete;

        // Get input layout description.
        const InputLayoutDescription& layoutDescription() const noexcept;

    private:
        InputLayoutDescription m_layoutDescription;
    };
}
