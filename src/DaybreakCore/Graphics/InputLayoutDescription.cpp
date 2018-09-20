#include "stdafx.h"
#include "InputLayoutDescription.h"

#include "Common/Error.h"

using namespace Daybreak;

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
    CHECK_NOT_ZERO(attribute.count());

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
InputAttribute InputLayoutDescription::getAttributeByIndex(size_t index) const noexcept
{
    return m_attributes[index];
}

//---------------------------------------------------------------------------------------------------------------------
size_t InputLayoutDescription::getAttributeIndexByName(
    InputAttribute::SemanticName name,
    unsigned int index) const
{
    for (size_t i = 0; i < m_attributes.size(); ++i)
    {
        const auto& attribute = m_attributes[i];

        if (attribute.semanticName() == name && attribute.semanticIndex() == index)
        {
            return i;
        }
    }

    throw DaybreakDataException("Input layout attribute was not found when searching by semantic name and index");
}

//---------------------------------------------------------------------------------------------------------------------
InputAttribute InputLayoutDescription::getAttributeByName(InputAttribute::SemanticName name, unsigned int index) const
{
    return m_attributes[getAttributeIndexByName(name, index)];
}

//---------------------------------------------------------------------------------------------------------------------
size_t InputLayoutDescription::elementSizeInBytes() const noexcept
{
    size_t size = 0;

    for (const auto& attribute : m_attributes)
    {
        size += attribute.sizeInBytes();
    }

    return size;
}

//---------------------------------------------------------------------------------------------------------------------
uintptr_t InputLayoutDescription::attributeOffsetByIndex(size_t index) const noexcept
{
    uintptr_t offset = 0;

    for (size_t i = 0; i < index; ++i)
    {
        offset += m_attributes[i].sizeInBytes();
    }

    return offset;
}
