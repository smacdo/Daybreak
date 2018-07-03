#include "stdafx.h"
#include "IndexBuffer.h"
#include "Common/Error.h"

using namespace Daybreak;

//---------------------------------------------------------------------------------------------------------------------
size_t IndexBuffer::elementSizeInBytes(IndexElementType elementType)
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
        return 0;
    }
}