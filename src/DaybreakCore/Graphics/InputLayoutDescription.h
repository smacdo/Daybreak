#pragma once
#include "Graphics/InputAttribute.h"
#include <vector>

namespace Daybreak
{
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
        InputAttribute getAttributeByIndex(size_t index) const noexcept;
        
        // Get index to an attribute by semantic name and index. Throws exception if not found.
        size_t getAttributeIndexByName(InputAttribute::SemanticName name, unsigned int index = 0) const;

        // Get an attribute by semantic name and index. Throws exception if not found.
        InputAttribute getAttributeByName(InputAttribute::SemanticName name, unsigned int index = 0) const;

        // Get the size (in bytes) of a single input element.
        size_t elementSizeInBytes() const noexcept;

        // Get the offset (in bytes) to an attribute.
        uintptr_t attributeOffsetByIndex(size_t index) const noexcept;

    private:
        std::vector<InputAttribute> m_attributes;
    };
}
