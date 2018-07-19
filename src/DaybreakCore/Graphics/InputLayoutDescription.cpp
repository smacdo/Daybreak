#include "stdafx.h"
#include "InputLayoutDescription.h"

#include "Common/Error.h"

using namespace Daybreak;

//---------------------------------------------------------------------------------------------------------------------
namespace
{
    size_t sizeInBytes(InputAttribute::StorageType type)
    {
        switch (type)
        {
        case InputAttribute::StorageType::Byte:
        case InputAttribute::StorageType::UnsignedByte:
            return 1;
        case InputAttribute::StorageType::Short:
        case InputAttribute::StorageType::UnsignedShort:
            return 2;
        case InputAttribute::StorageType::Int:
        case InputAttribute::StorageType::UnsignedInt:
            return 4;
        case InputAttribute::StorageType::HalfFloat:
            return 2;
        case InputAttribute::StorageType::Float:
            return 4;
        case InputAttribute::StorageType::Double:
            return 8;
        default:
            THROW_ENUM_SWITCH_NOT_HANDLED(InputAttribute::StorageType, type);
        }
    }
}

// TODO: Add validation that vertex format is correct and can be handled by the engine.
//---------------------------------------------------------------------------------------------------------------------
InputLayoutDescription::InputLayoutDescription()
{
    m_attributes.reserve(3);
}

//---------------------------------------------------------------------------------------------------------------------
InputLayoutDescription::InputLayoutDescription(_In_ const std::vector<InputAttribute>& attributes)
    : m_attributes()
{
    EXPECT(attributes.size() > 0, "Input layout must have at least one attribute");
    m_attributes.reserve(attributes.size());

    for (const auto& attribute : attributes)
    {
        addAttribute(attribute);
    }
}

//---------------------------------------------------------------------------------------------------------------------
InputLayoutDescription::~InputLayoutDescription() = default;

//---------------------------------------------------------------------------------------------------------------------
void InputLayoutDescription::addAttribute(_In_ const InputAttribute& attribute)
{
    // TODO: Validation!
    CHECK_NOT_ZERO(attribute.count);

    m_attributes.push_back(attribute);
}

//---------------------------------------------------------------------------------------------------------------------
void InputLayoutDescription::addAttribute(
    _In_ InputAttribute::SemanticName name,
    _In_ unsigned int index,
    _In_ InputAttribute::StorageType type,
    _In_ unsigned int count)
{
    addAttribute({ name, index, type, count });
}

//---------------------------------------------------------------------------------------------------------------------
size_t InputLayoutDescription::attributeCount() const noexcept
{
    return m_attributes.size();
}

//---------------------------------------------------------------------------------------------------------------------
InputAttribute InputLayoutDescription::getAttributeByIndex(unsigned int index) const noexcept
{
    return m_attributes[index];
}

//---------------------------------------------------------------------------------------------------------------------
size_t InputLayoutDescription::elementSizeInBytes() const noexcept
{
    size_t size = 0;

    for (const auto& attribute : m_attributes)
    {
        size += sizeInBytes(attribute.type) * attribute.count;
    }

    return size;
}
