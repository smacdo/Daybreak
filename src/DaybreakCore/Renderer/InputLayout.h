#pragma once
#include <memory>

namespace Daybreak
{
    class InputLayoutDescription;

    // Abstract base class for input layout.
    class InputLayout
    {
    public:
        // Constructor.
        InputLayout(_In_ std::shared_ptr<const InputLayoutDescription> description);

        // Copy constructor (deleted).
        InputLayout(const InputLayout&) = delete;

        // Destructor.
        virtual ~InputLayout();

        // Assignment operator (deleted).
        InputLayout& operator =(const InputLayout&) = delete;

        // Get input layout description.
        std::shared_ptr<const InputLayoutDescription> layoutDescription() const noexcept;

        // Get input layout description by reference.
        const InputLayoutDescription& layoutDescriptionRef() const noexcept;

    private:
        std::shared_ptr<const InputLayoutDescription> m_layoutDescription;
    };
}
