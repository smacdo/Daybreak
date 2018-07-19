#include "stdafx.h"
#include "CppUnitTest.h"

#include "Graphics/Mesh/IndexBufferData.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Daybreak;

namespace Microsoft::VisualStudio::CppUnitTestFramework
{
    std::wstring ToString(IndexElementType type)
    {
        switch (type)
        {
        case IndexElementType::UnsignedByte:
            return L"IndexElementType::UnsignedByte";
        case IndexElementType::UnsignedShort:
            return L"IndexElementType::UnsignedShort";
        case IndexElementType::UnsignedInt:
            return L"IndexElementType::UnsignedInt";
        default:
            return L"???";
        }
    }
}

TEST_CLASS(IndexBufferDataTests)
{
public:
    TEST_METHOD(Index_Buffer_Takes_Ownership_Of_Unsigned_Byte_Buffer)
    {
        std::unique_ptr<uint8_t[]> indices(new uint8_t[3]{ 2, 5, 10 });
        IndexBufferData indexBuffer(3, std::move(indices));

        Assert::AreEqual((uint8_t)2, *((uint8_t*)indexBuffer.bytes() + 0));
        Assert::AreEqual((uint8_t)5, *((uint8_t*)indexBuffer.bytes() + 1));
        Assert::AreEqual((uint8_t)10, *((uint8_t*)indexBuffer.bytes() + 2));
    }

    TEST_METHOD(Index_Buffer_Unsigned_Byte_Constructor_Assumes_Unsigned_Byte_Index_Element)
    {
        std::unique_ptr<uint8_t[]> indices(new uint8_t[3]{ 2, 5, 10 });
        IndexBufferData indexBuffer(3, std::move(indices));

        Assert::AreEqual(IndexElementType::UnsignedByte, indexBuffer.elementType());
    }

    TEST_METHOD(Constructing_Index_Buffer_Copies_Element_Type)
    {
        std::unique_ptr<unsigned char[]> indices1(new unsigned char[5]{ 2, 5, 10, 11, 12 });
        IndexBufferData indexBuffer1(5, std::move(indices1), IndexElementType::UnsignedShort);

        Assert::AreEqual(IndexElementType::UnsignedShort, indexBuffer1.elementType());

        std::unique_ptr<unsigned char[]> indices2(new unsigned char[4]{ 2, 5, 10, 11 });
        IndexBufferData indexBuffer2(4, std::move(indices2), IndexElementType::UnsignedInt);

        Assert::AreEqual(IndexElementType::UnsignedInt, indexBuffer2.elementType());
    }

    TEST_METHOD(Constructing_Index_Buffer_Copies_Element_Count)
    {
        std::unique_ptr<unsigned char[]> indices1(new unsigned char[5]{ 2, 5, 10, 11, 12 });
        IndexBufferData indexBuffer1(5, std::move(indices1), IndexElementType::UnsignedByte);

        Assert::AreEqual((size_t)5, indexBuffer1.byteCount());

        std::unique_ptr<unsigned char[]> indices2(new unsigned char[4]{ 2, 5, 10, 11 });
        IndexBufferData indexBuffer2(4, std::move(indices2), IndexElementType::UnsignedByte);

        Assert::AreEqual((size_t)4, indexBuffer2.byteCount());
    }

    TEST_METHOD(Index_Buffer_U16_Constructor_Sets_U16_Type)
    {
        std::unique_ptr<uint16_t[]> indices(new uint16_t[3]{ 1000, 2000, 3000 });
        IndexBufferData indexBuffer(3, std::move(indices));

        Assert::AreEqual(IndexElementType::UnsignedShort, indexBuffer.elementType());
    }

    TEST_METHOD(Index_Buffer_U32_Constructor_Sets_U32_Type)
    {
        std::unique_ptr<uint32_t[]> indices(new uint32_t[3]{ 100000, 200000, 300000 });
        IndexBufferData indexBuffer(3, std::move(indices));

        Assert::AreEqual(IndexElementType::UnsignedInt, indexBuffer.elementType());
    }
};