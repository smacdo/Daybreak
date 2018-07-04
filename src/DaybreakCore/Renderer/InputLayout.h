#pragma once
#include "Graphics/Mesh/VertexFormat.h"

#include <vector>

namespace Daybreak
{
    // Abstract base class for input layout.
    class InputLayout
    {
    public:
        // Underlying storage type for an input attribute.
        enum class StorageType
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
            StorageType type;
            unsigned int elementCount;
            bool shouldNormalize;
        };

    public:
        // Constructor.
        InputLayout(_In_ const std::vector<attribute_t>& attributes);

        // Copy constructor (deleted).
        InputLayout(const InputLayout&) = delete;

        // Destructor.
        virtual ~InputLayout() = default;

        // Assignment operator (deleted).
        InputLayout& operator =(const InputLayout&) = delete;

        // Get number of attributes.
        size_t attributeCount() const noexcept;

        // Get an attribute by index.
        attribute_t getAttributeByIndex(unsigned int index) const noexcept;

    public:
        // Convert a vertex format into an array of input layout attributes.
        static std::vector<attribute_t> createInputLayoutAttributesFor(
            _In_ const std::vector<vertex_attribute_t>& vertexAttributes);

        // Convert a vertex format attribute into an input layout attribute.
        static attribute_t createInputLayoutAttributeFor(_In_ const vertex_attribute_t& vertexAttribute);

    private:
        std::vector<attribute_t> m_attributes;
    };
}