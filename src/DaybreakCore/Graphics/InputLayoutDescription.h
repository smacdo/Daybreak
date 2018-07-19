#pragma once
#include <vector>

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
        }

    public:
        SemanticName name;
        unsigned int index;
        StorageType type;
        unsigned int count;
    };

    // Defines how input data is laid out for a untyped data buffer.
    class InputLayoutDescription
    {
    public:
        // Default constructor.
        InputLayoutDescription();

        // Constructor.
        InputLayoutDescription(_In_ const std::vector<InputAttribute>& attributes);

        // Destructor.
        virtual ~InputLayoutDescription();

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
