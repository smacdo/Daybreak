#include "stdafx.h"
#include "Graphics/Mesh/IndexBufferData.h"

#include "../../TestHelpers.h"

using namespace Daybreak;

TEST_CASE("Index_Buffer_Takes_Ownership_Of_Unsigned_Byte_Buffer", "[graphics][IndexBufferData]")
{
    std::unique_ptr<uint8_t[]> indices(new uint8_t[3]{ 2, 5, 10 });
    IndexBufferData indexBuffer(3, std::move(indices));

    REQUIRE((uint8_t)2 == *((uint8_t*)indexBuffer.bytes() + 0));
    REQUIRE((uint8_t)5 == *((uint8_t*)indexBuffer.bytes() + 1));
    REQUIRE((uint8_t)10 == *((uint8_t*)indexBuffer.bytes() + 2));
}

TEST_CASE("Index_Buffer_Unsigned_Byte_Constructor_Assumes_Unsigned_Byte_Index_Element", "[graphics][IndexBufferData]")
{
    std::unique_ptr<uint8_t[]> indices(new uint8_t[3]{ 2, 5, 10 });
    IndexBufferData indexBuffer(3, std::move(indices));

    REQUIRE(IndexElementType::UnsignedByte == indexBuffer.elementType());
}

TEST_CASE("Constructing_Index_Buffer_Copies_Element_Type", "[graphics][IndexBufferData]")
{
    std::unique_ptr<unsigned char[]> indices1(new unsigned char[5]{ 2, 5, 10, 11, 12 });
    IndexBufferData indexBuffer1(5 * 2, std::move(indices1), IndexElementType::UnsignedShort);

    REQUIRE(IndexElementType::UnsignedShort == indexBuffer1.elementType());

    std::unique_ptr<unsigned char[]> indices2(new unsigned char[4]{ 2, 5, 10, 11 });
    IndexBufferData indexBuffer2(4 * 4, std::move(indices2), IndexElementType::UnsignedInt);

    REQUIRE(IndexElementType::UnsignedInt == indexBuffer2.elementType());
}

TEST_CASE("Constructing_Index_Buffer_Copies_Element_Count", "[graphics][IndexBufferData]")
{
    std::unique_ptr<unsigned char[]> indices1(new unsigned char[5]{ 2, 5, 10, 11, 12 });
    IndexBufferData indexBuffer1(5, std::move(indices1), IndexElementType::UnsignedByte);

    REQUIRE((size_t)5 == indexBuffer1.byteCount());

    std::unique_ptr<unsigned char[]> indices2(new unsigned char[4]{ 2, 5, 10, 11 });
    IndexBufferData indexBuffer2(4, std::move(indices2), IndexElementType::UnsignedByte);

    REQUIRE((size_t)4 == indexBuffer2.byteCount());
}

TEST_CASE("Index_Buffer_U16_Constructor_Sets_U16_Type", "[graphics][IndexBufferData]")
{
    std::unique_ptr<uint16_t[]> indices(new uint16_t[3]{ 1000, 2000, 3000 });
    IndexBufferData indexBuffer(3, std::move(indices));

    REQUIRE(IndexElementType::UnsignedShort == indexBuffer.elementType());
}

TEST_CASE("Index_Buffer_U16_Constructor_Copies_Number_Of_Indices", "[graphics][IndexBufferData]")
{
    std::unique_ptr<uint16_t[]> indices(new uint16_t[4]{ 1000, 2000, 3000, 4000 });
    IndexBufferData indexBuffer(4, std::move(indices));

    REQUIRE(4 == (int)indexBuffer.indexCount());
}

TEST_CASE("Index_Buffer_U32_Constructor_Sets_U32_Type", "[graphics][IndexBufferData]")
{
    std::unique_ptr<uint32_t[]> indices(new uint32_t[3]{ 100000, 200000, 300000 });
    IndexBufferData indexBuffer(3, std::move(indices));

    REQUIRE(IndexElementType::UnsignedInt == indexBuffer.elementType());
}

TEST_CASE("Index_Buffer_U32_Constructor_Copies_Number_Of_Indices", "[graphics][IndexBufferData]")
{
    std::unique_ptr<uint32_t[]> indices(new uint32_t[5]{ 100000, 200000, 300000, 400000, 500000 });
    IndexBufferData indexBuffer(5, std::move(indices));

    REQUIRE(5 == (int)indexBuffer.indexCount());
}
