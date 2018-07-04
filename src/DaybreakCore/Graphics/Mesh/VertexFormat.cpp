#include "stdafx.h"
#include "VertexFormat.h"

#include "Common/Error.h"

using namespace Daybreak;

// TODO: Add validation that vertex format is correct and can be handled by the engine.

//---------------------------------------------------------------------------------------------------------------------
namespace
{
    size_t sizeInBytes(VertexAttributeStorage attribute)
    {
        switch (attribute)
        {
        case VertexAttributeStorage::Float2:
            return sizeof(float) * 2;
        case VertexAttributeStorage::Float3:
            return sizeof(float) * 3;
        case VertexAttributeStorage::Float4:
            return sizeof(float) * 4;
        default:
            THROW_ENUM_SWITCH_NOT_HANDLED(VertexAttributeStorage, attribute);
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
VertexFormat::VertexFormat()
{
    m_attributes.reserve(3);
}

//---------------------------------------------------------------------------------------------------------------------
VertexFormat::VertexFormat(_In_ const std::vector<vertex_attribute_t>& attributes)
    : m_attributes()
{
    EXPECT(attributes.size() > 0, "Vertex format must have at least one attribute");
    m_attributes.reserve(attributes.size());

    for (const auto& attribute : attributes)
    {
        addAttribute(attribute.name, attribute.storage, attribute.index);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VertexFormat::addAttribute(VertexAttributeName name, VertexAttributeStorage storage)
{
    addAttribute(name, storage, 0);
}

//---------------------------------------------------------------------------------------------------------------------
void VertexFormat::addAttribute(VertexAttributeName name, VertexAttributeStorage storage, unsigned int index)
{
    // TODO: Validation!
    m_attributes.push_back({ name, storage, index });
}

//---------------------------------------------------------------------------------------------------------------------
size_t VertexFormat::attributeCount() const noexcept
{
    return m_attributes.size();
}

//---------------------------------------------------------------------------------------------------------------------
vertex_attribute_t VertexFormat::getAttributeByIndex(unsigned int index) const noexcept
{
    return m_attributes[index];
}

//---------------------------------------------------------------------------------------------------------------------
size_t VertexFormat::vertexSizeInBytes() const noexcept
{
    size_t size = 0;

    for (const auto& attribute : m_attributes)
    {
        size += sizeInBytes(attribute.storage);
    }

    return size;
}

//---------------------------------------------------------------------------------------------------------------------
std::shared_ptr<VertexFormat> vertex_ptn_t::elementType = std::make_shared<VertexFormat>(
    std::vector<vertex_attribute_t> {
        { VertexAttributeName::Position, VertexAttributeStorage::Float3, 0 },
        { VertexAttributeName::Texture, VertexAttributeStorage::Float2, 0 },
        { VertexAttributeName::Normal, VertexAttributeStorage::Float3, 0 }
});
