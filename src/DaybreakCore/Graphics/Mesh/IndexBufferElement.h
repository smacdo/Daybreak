#pragma once
#include <cstdint>

namespace Daybreak
{
    /// Type of element stored in an index buffer.
    enum class IndexElementType
    {
        UnsignedByte,       ///< 1 byte index value.
        UnsignedShort,      ///< 2 byte index value.
        UnsignedInt         ///< 4 byte index value.
    };

    // Get size of index element type in bytes.
    size_t indexElementSizeInBytes(IndexElementType elementType);

    // Get index element type from storage size.
    IndexElementType indexElementStorageSizeToType(size_t sizeInBytes);

    /// Generic index element.
    template<typename T>
    struct index_element_t
    {
        explicit index_element_t(T v)
            : value(v)
        {
        }

        operator T() const { return value; }

        T value;
    };

    /// Standard unsigned int index element.
    struct index_element_ui8 : public index_element_t<uint8_t>
    {
        static IndexElementType elementType;
    };

    struct index_element_ui16 : public index_element_t<uint16_t>
    {
        static IndexElementType elementType;
    };

    struct index_element_ui32 : public index_element_t<uint32_t>
    {
        index_element_ui32(int v)
            : index_element_t(v)
        {
        }

        static IndexElementType elementType;
    };
}