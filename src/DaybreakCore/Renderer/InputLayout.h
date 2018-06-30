#pragma once
#include <vector>

namespace Daybreak
{
    // Abstract base class for input layout.
    class IInputLayout
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
        IInputLayout() = default;

        // Copy constructor (deleted).
        IInputLayout(const IInputLayout&) = delete;

        // Destructor.
        virtual ~IInputLayout() = default;

        // Assignment operator (deleted).
        IInputLayout& operator =(const IInputLayout&) = delete;

        // Get number of attributes.
        virtual size_t attributeCount() const noexcept = 0;

        // Get an attribute by index.
        virtual attribute_t getAttributeByIndex(unsigned int index) const noexcept = 0;
    };
}