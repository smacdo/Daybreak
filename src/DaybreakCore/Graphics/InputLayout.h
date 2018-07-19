#pragma once
#include "Graphics/Mesh/VertexFormat.h"

#include <vector>

// TODO: Move to Graphics/

namespace Daybreak
{
    // A single data attribute in an input layout.
    class InputAttribute
    {
    public:
        // Intended use for an attribute.
        enum class SemanticName
        {
            None,
            Position,
            Texture,
            Normal
        };

        // Underlying storage type for an attribute.
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

    public:
        InputAttribute() = default;
        InputAttribute(SemanticName name, unsigned int index, StorageType type, unsigned int count)
            : name(name), index(index), type(type), count(count)
        {
            // TODO: VALIDATION?
        }

    public:
        SemanticName name;
        unsigned int index;
        StorageType type;
        unsigned int count;
    };

    // Defines how input data is laid out for a untyped data buffer.
    class InputLayout
    {
    public:
        // Default constructor.
        InputLayout();

        // Constructor.
        InputLayout(_In_ const std::vector<InputAttribute>& attributes);

        // Destructor.
        virtual ~InputLayout();

    public:
        // Add an input attribute.
        void addAttribute(_In_ const InputAttribute& attribute);

        // Add an input attribute.
        void addAttribute(
            _In_ InputAttribute::SemanticName name,
            _In_ unsigned int index,
            _In_ InputAttribute::StorageType type,
            _In_ unsigned int count);

        // Get number of attributes.
        size_t attributeCount() const noexcept;

        // Get an attribute by index.
        InputAttribute getAttributeByIndex(unsigned int index) const noexcept;

        // Get the size (in bytes) of a single input element.
        size_t elementSizeInBytes() const noexcept;

    private:
        std::vector<InputAttribute> m_attributes;
    };
}