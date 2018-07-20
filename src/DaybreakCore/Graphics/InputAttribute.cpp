#include "stdafx.h"
#include "InputAttribute.h"
#include "Common/Error.h"

using namespace Daybreak;

//---------------------------------------------------------------------------------------------------------------------
InputAttribute::InputAttribute() = default;

//---------------------------------------------------------------------------------------------------------------------
InputAttribute::InputAttribute(
    _In_ SemanticName name,
    _In_ unsigned int index,
    _In_ StorageType type,
    _In_ unsigned int count)
    : m_name(name), m_index(index), m_type(type), m_count(count)
{
}

//---------------------------------------------------------------------------------------------------------------------
size_t InputAttribute::sizeInBytes() const
{
    return typeSizeInBytes(m_type) * m_count;
}

//---------------------------------------------------------------------------------------------------------------------
size_t InputAttribute::typeSizeInBytes(StorageType type)
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

//---------------------------------------------------------------------------------------------------------------------
bool Daybreak::operator ==(const InputAttribute& lhs, const InputAttribute& rhs) noexcept
{
    return
        lhs.semanticName() == rhs.semanticName() &&
        lhs.semanticIndex() == rhs.semanticIndex() &&
        lhs.type() == rhs.type() &&
        lhs.count() == rhs.count();
}
