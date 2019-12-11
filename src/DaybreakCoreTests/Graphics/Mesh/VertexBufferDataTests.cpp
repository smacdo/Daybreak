#include "stdafx.h"
#include "Graphics/InputLayoutDescription.h"
#include "Graphics/Mesh/VertexBufferData.h"

#include <vector>

#include "../../TestHelpers.h"

using namespace Daybreak;

namespace
{
    struct ivec3
    {
        int x, y, z;
    };

    struct test_vertex_t
    {
        int x, y, z;
    };
}

TEST_CASE("Constructor_Takes_Ownership_Of_Buffer", "[graphics][VertexBufferData]")
{
    // Byte buffer with test data (no real vertex).
    std::unique_ptr<uint8_t[]> bytes(new uint8_t[4]{ 2, 5, 10, 12 });

    // Simple input layout where each vertex is 4 bytes.
    auto layout = std::make_shared<InputLayoutDescription>();
    layout->addAttribute(InputAttribute::SemanticName::None, 0, InputAttribute::StorageType::Int, 1);

    // Construct vertex buffer and ensure the 4 bytes passed are unchanged.
    VertexBufferData vb(4, std::move(bytes), layout);

    REQUIRE(2 == (int)*((uint8_t*)vb.bytes() + 0));
    REQUIRE(5 == (int)*((uint8_t*)vb.bytes() + 1));
    REQUIRE(10 == (int)*((uint8_t*)vb.bytes() + 2));
    REQUIRE(12 == (int)*((uint8_t*)vb.bytes() + 3));

    // Check the input layout was copied.
    REQUIRE(layout.get() == vb.inputLayout().get());
}

TEST_CASE("Constructor_Throws_Exception_If_Given_Empty_Input_Layout", "[graphics][VertexBufferData]")
{
    auto e = []() {
        std::unique_ptr<uint8_t[]> bytes(new uint8_t[3]{ 2, 5, 10 });
        VertexBufferData vb(3, std::move(bytes), std::make_shared<InputLayoutDescription>());
    };

    REQUIRE_THROWS_MATCHES(
        e(),
        DaybreakDataException,
        Catch::Message("Vertex buffer input layout description cannot be empty"));
}

TEST_CASE("Constructor_Throws_Exception_If_Not_Multiple_Of_Vertex_Size", "[graphics][VertexBufferData]")
{
    auto e = []() {
        std::unique_ptr<uint8_t[]> bytes(new uint8_t[3]{ 2, 5, 10 });

        auto layout = std::make_shared<InputLayoutDescription>();
        layout->addAttribute(InputAttribute::SemanticName::None, 0, InputAttribute::StorageType::Int, 1);

        VertexBufferData vb(3, std::move(bytes), layout);
    };

    REQUIRE_THROWS_MATCHES(
        e(),
        DaybreakDataException,
        Catch::Message("Vertex buffer size must be multiple of vertex size"));
}

TEST_CASE("Vertex_Buffer_Typed_Constructor_Copies_Input_Vertices", "[graphics][VertexBufferData]")
{
    std::unique_ptr<test_vertex_t[]> vertices(new test_vertex_t[2]);
    vertices[0] = { 2, 4, 5 };
    vertices[1] = { 10, 12, 14 };

    auto layout = std::make_shared<InputLayoutDescription>();
    layout->addAttribute(InputAttribute::SemanticName::Position, 0, InputAttribute::StorageType::Float, 3);

    // Construct vertex buffer with typed vertices and ensure two vertices copied.
    VertexBufferData vb(2, std::move(vertices), layout);

    REQUIRE(2 == (((test_vertex_t*)vb.bytes()) + 0)->x);
    REQUIRE(4 == (((test_vertex_t*)vb.bytes()) + 0)->y);
    REQUIRE(5 == (((test_vertex_t*)vb.bytes()) + 0)->z);

    REQUIRE(10 == (((test_vertex_t*)vb.bytes()) + 1)->x);
    REQUIRE(12 == (((test_vertex_t*)vb.bytes()) + 1)->y);
    REQUIRE(14 == (((test_vertex_t*)vb.bytes()) + 1)->z);

    // Check the input layout was copied.
    REQUIRE(layout.get() == vb.inputLayout().get());
}

TEST_CASE("Vertex_Buffer_Typed_Constructor_Throws_Exception_If_Given_Empty_Input_Layout", "[graphics][VertexBufferData]")
{
    auto e = []() {
        std::unique_ptr<test_vertex_t[]> vertices(new test_vertex_t[2]);
        vertices[0] = { 2, 4, 5 };
        vertices[1] = { 10, 12, 14 };

        VertexBufferData vb(2, std::move(vertices), std::make_shared<InputLayoutDescription>());
    };

    REQUIRE_THROWS_MATCHES(
        e(),
        DaybreakDataException,
        Catch::Message("Vertex buffer input layout description cannot be empty"));
}

TEST_CASE("Vertex_Buffer_Typed_Constructor_Throws_Exception_Zero_Vertices", "[graphics][VertexBufferData]")
{
    auto layout = std::make_shared<InputLayoutDescription>();
    layout->addAttribute(InputAttribute::SemanticName::Position, 0, InputAttribute::StorageType::Float, 3);

    auto e = [&layout]() {
        std::unique_ptr<test_vertex_t[]> vertices(new test_vertex_t[2]);
        vertices[0] = { 2, 4, 5 };

        VertexBufferData vb(0, std::move(vertices), layout);
    };

    REQUIRE_THROWS_MATCHES(
        e(),
        DaybreakDataException,
        Catch::Message("Vertex buffer size cannot be zero"));
}

TEST_CASE("Single_Attribute_Stream_Iterator", "[graphics][VertexBufferData]")
{
    std::unique_ptr<test_vertex_t[]> vertices(new test_vertex_t[2]);
    vertices[0] = { 2, 4, 5 };
    vertices[1] = { 10, 12, 14 };

    auto layout = std::make_shared<InputLayoutDescription>();
    layout->addAttribute(InputAttribute::SemanticName::Position, 0, InputAttribute::StorageType::Float, 3);

    // Construct vertex buffer and try to read both elements in the position attribute stream.
    VertexBufferData vb(2, std::move(vertices), layout);
    auto positionItr = vb.attributeBegin<ivec3>(InputAttribute::SemanticName::Position);

    REQUIRE(2 == positionItr->x);
    REQUIRE(4 == positionItr->y);
    REQUIRE(5 == positionItr->z);

    positionItr++;

    REQUIRE(10 == positionItr->x);
    REQUIRE(12 == positionItr->y);
    REQUIRE(14 == positionItr->z);
}
