#include "stdafx.h"
#include "InputLayout.h"

#include "Common/Error.h"

using namespace Daybreak;

//---------------------------------------------------------------------------------------------------------------------
InputLayout::InputLayout(_In_ const std::vector<attribute_t>& attributes)
    : m_attributes(attributes)
{
    EXPECT(attributes.size() > 0, "Input layout must have at least one attribute");
}

//---------------------------------------------------------------------------------------------------------------------
size_t InputLayout::attributeCount() const noexcept
{
    return m_attributes.size();
}

//---------------------------------------------------------------------------------------------------------------------
InputLayout::attribute_t InputLayout::getAttributeByIndex(unsigned int index) const noexcept
{
    return m_attributes[index];
}

//---------------------------------------------------------------------------------------------------------------------
std::vector<InputLayout::attribute_t> InputLayout::createInputLayoutAttributesFor(
    _In_ const std::vector<vertex_attribute_t>& vertexAttributes)
{
    // TODO: Add validation that vertex format is correct and can be handled by the engine.
    //       - Add this on a method in vertex attribute, and call it here.
    std::vector<InputLayout::attribute_t> attributes;
    attributes.reserve(vertexAttributes.size());

    for (size_t i = 0; i < vertexAttributes.size(); ++i)
    {
        auto attribute = createInputLayoutAttributeFor(vertexAttributes[i]);
        attribute.slot = static_cast<unsigned int>(i);

        attributes.push_back(attribute);
    }
    
    return attributes;
}

//---------------------------------------------------------------------------------------------------------------------
InputLayout::attribute_t InputLayout::createInputLayoutAttributeFor(const vertex_attribute_t& vertexAttribute)
{
    // TODO: Use exceptions with descriptive messages rather than "not handled" for malformed vertex attributes.
    // TODO: This can be simplified since I don't think we need to switch on attribute name. If the format has already
    //       been validated then all we need to do is convert vertex attr storage type to input layout attr type +
    //       count. Maybe normalized needs to be handled specially? (Do we ever use true?)
    switch (vertexAttribute.name)
    {
    case VertexAttributeName::Position:
        EXPECT(vertexAttribute.index == 0, "Can only have one position attribute");
        switch (vertexAttribute.storage)
        {
        case VertexAttributeStorage::Float3:
            return { 0, StorageType::Float, 3, false };

        default:
            THROW_ENUM_SWITCH_NOT_HANDLED(VertexAttributeStorage, vertexAttribute.storage);
        }
        break;

    case VertexAttributeName::Normal:
        EXPECT(vertexAttribute.index == 0, "Can only have one normal attribute");
        switch (vertexAttribute.storage)
        {
        case VertexAttributeStorage::Float3:
            return { 0, StorageType::Float, 3, false };

        default:
            THROW_ENUM_SWITCH_NOT_HANDLED(VertexAttributeStorage, vertexAttribute.storage);
        }
        break;

    case VertexAttributeName::Texture:
        EXPECT(vertexAttribute.index == 0, "Can only have one position attribute");
        switch (vertexAttribute.storage)
        {
        case VertexAttributeStorage::Float2:
            return { 0, StorageType::Float, 2, false };

        default:
            THROW_ENUM_SWITCH_NOT_HANDLED(VertexAttributeStorage, vertexAttribute.storage);
        }
        break;

    default:
        THROW_ENUM_SWITCH_NOT_HANDLED(VertexAttributeName, vertexAttribute.name);
    };
}
