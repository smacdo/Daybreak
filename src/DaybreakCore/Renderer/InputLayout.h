#pragma once
#include <vector>

namespace Daybreak
{
    // Abstract base class for input layout.
    class InputLayout
    {
    public:
        // Data types for input layout vertex attributes.
        enum class ElementType
        {
            Byte,
            UnsignedByte,
            Short,
            UnsignedShort,
            Int,
            UnsignedInt,
            HalfFloat,
            Float,
            Double
        };

        struct attribute_t
        {
            unsigned int slot;
            ElementType type;
            unsigned int count;
            bool shouldNormalize;
        };

    public:
        // Constructor.
        InputLayout() = default;

        // Copy constructor (deleted).
        InputLayout(const InputLayout&) = delete;

        // Destructor.
        virtual ~InputLayout() = default;

        // Assignment operator (deleted).
        InputLayout& operator =(const InputLayout&) = delete;

        // Get number of attributes.
        virtual size_t attributeCount() const noexcept = 0;

        // Get an attribute by index.
        virtual attribute_t getAttributeByIndex(unsigned int index) const noexcept = 0;
    };
}