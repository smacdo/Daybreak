#include "stdafx.h"
#include "Graphics/Mesh/BufferData.h"

#include "../../TestHelpers.h"

using namespace Daybreak;

TEST_CASE("Data_Buffer_Default_Constructor_Sets_Null_Data_And_Zero_Count", "[graphics][BufferData]")
{
    BufferData buffer;
    REQUIRE(0 == (int)buffer.byteCount());
    REQUIRE((const void*) nullptr == buffer.bytes());
}

TEST_CASE("Data_Buffer_Takes_Buffer_Pointer_And_Size", "[graphics][BufferData]")
{
    std::unique_ptr<uint8_t[]> data(new uint8_t[3]{ 1, 3, 4 });
    auto rawPointer = data.get();

    BufferData buffer(3, std::move(data));

    // Check if size was copied.
    REQUIRE(3 == (int)buffer.byteCount());

    // Check that the pointer was taken (not copied to a new address).
    REQUIRE(rawPointer == (uint8_t*)buffer.bytes());

    // Check data was not changed.
    REQUIRE((uint8_t)1 == *((uint8_t*)buffer.bytes() + 0));
    REQUIRE((uint8_t)3 == *((uint8_t*)buffer.bytes() + 1));
    REQUIRE((uint8_t)4 == *((uint8_t*)buffer.bytes() + 2));
}

TEST_CASE("Data_Buffer_Set_Unowned_Data_Pointer_Copies_Pointer_And_Size", "[graphics][BufferData]")
{
    // Construct with null buffer.
    BufferData buffer;

    // Set unowned buffer pointer and size.
    std::unique_ptr<uint8_t[]> data(new uint8_t[3]{ 1, 3, 4 });
    auto rawPointer = data.get();

    buffer.setUnownedDataPtr(3, rawPointer);

    // Check if size was copied.
    REQUIRE(3 == (int)buffer.byteCount());

    // Check that the pointer was taken (not copied to a new address).
    REQUIRE(rawPointer == (uint8_t*)buffer.bytes());

    // Check data was not changed.
    REQUIRE((uint8_t)1 == *((uint8_t*)buffer.bytes() + 0));
    REQUIRE((uint8_t)3 == *((uint8_t*)buffer.bytes() + 1));
    REQUIRE((uint8_t)4 == *((uint8_t*)buffer.bytes() + 2));
}

TEST_CASE("Data_Buffer_Set_Unowned_Data_Pointer_Does_Not_Own_Pointer", "[graphics][BufferData]")
{
    // Create a data pointer.
    uint8_t * data = new uint8_t[3]{ 1, 3, 4 };

    {
        BufferData buffer;
        buffer.setUnownedDataPtr(3, data);
    }

    // If this does not trigger a double free its reasonable to assume BufferData did not take ownership.
    delete[] data;
}

TEST_CASE("Data_Buffer_Accepts_Zero_Size_Buffer", "[graphics][BufferData]")
{
    BufferData buffer(0, nullptr);
}
