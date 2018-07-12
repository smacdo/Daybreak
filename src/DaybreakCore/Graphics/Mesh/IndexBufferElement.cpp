#include "stdafx.h"
#include "IndexBufferElement.h"
#include "Common/Error.h"

using namespace Daybreak;

//---------------------------------------------------------------------------------------------------------------------
IndexElementType index_element_ui8::elementType = IndexElementType::UnsignedByte;
IndexElementType index_element_ui16::elementType = IndexElementType::UnsignedShort;
IndexElementType index_element_ui32::elementType = IndexElementType::UnsignedInt;

//---------------------------------------------------------------------------------------------------------------------
size_t Daybreak::indexElementSizeInBytes(IndexElementType elementType)
{
    switch (elementType)
    {
    case IndexElementType::UnsignedByte:
        return 1;
    case IndexElementType::UnsignedShort:
        return 2;
    case IndexElementType::UnsignedInt:
        return 4;
    default:
        THROW_ENUM_SWITCH_NOT_HANDLED(IndexElementType, elementType);
    }
}

//---------------------------------------------------------------------------------------------------------------------
IndexElementType Daybreak::indexElementStorageSizeToType(size_t sizeInBytes)
{
    switch (sizeInBytes)
    {
    case 1:
        return IndexElementType::UnsignedByte;
    case 2:
        return IndexElementType::UnsignedShort;
    case 4:
        return IndexElementType::UnsignedInt;
    default:
        throw std::runtime_error("Unknown index element size");
    }
}
