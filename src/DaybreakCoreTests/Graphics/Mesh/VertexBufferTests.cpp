#include "stdafx.h"
#include "CppUnitTest.h"

#include "Common/Error.h"
#include "Graphics/InputLayoutDescription.h"
#include "Graphics/Mesh/VertexBufferData.h"

#include <vector>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Daybreak;

TEST_CLASS(VertexBufferTests)
{
public:
    struct ivec3
    {
        int x, y, z;
    };

    struct test_vertex_t
    {
        int x, y, z;
    };

public:
    TEST_METHOD(Constructor_Takes_Ownership_Of_Buffer)
    {
        // Byte buffer with test data (no real vertex).
        std::unique_ptr<uint8_t[]> bytes(new uint8_t[4]{ 2, 5, 10, 12 });

        // Simple input layout where each vertex is 4 bytes.
        auto layout = std::make_shared<InputLayoutDescription>();
        layout->addAttribute(InputAttribute::SemanticName::None, 0, InputAttribute::StorageType::Int, 1);

        // Construct vertex buffer and ensure the 4 bytes passed are unchanged.
        VertexBufferData vb(4, std::move(bytes), layout);

        Assert::AreEqual(2, (int)*((uint8_t*)vb.bytes() + 0));
        Assert::AreEqual(5, (int)*((uint8_t*)vb.bytes() + 1));
        Assert::AreEqual(10, (int)*((uint8_t*)vb.bytes() + 2));
        Assert::AreEqual(12, (int)*((uint8_t*)vb.bytes() + 3));

        // Check the input layout was copied.
        Assert::IsTrue(layout.get() == vb.inputLayout().get());
    }

    TEST_METHOD(Constructor_Throws_Exception_If_Given_Empty_Input_Layout)
    {
        Assert::ExpectException<DaybreakDataException>([]() {
            std::unique_ptr<uint8_t[]> bytes(new uint8_t[3]{ 2, 5, 10 });
            VertexBufferData vb(3, std::move(bytes), std::make_shared<InputLayoutDescription>());
        });
    }

    TEST_METHOD(Constructor_Throws_Exception_If_Not_Multiple_Of_Vertex_Size)
    {
        std::unique_ptr<uint8_t[]> bytes(new uint8_t[3]{ 2, 5, 10 });

        auto layout = std::make_shared<InputLayoutDescription>();
        layout->addAttribute(InputAttribute::SemanticName::None, 0, InputAttribute::StorageType::Int, 1);

        Assert::ExpectException<DaybreakDataException>([&bytes, &layout]() {
            VertexBufferData vb(3, std::move(bytes), layout);
        });
    }

    TEST_METHOD(Vertex_Buffer_Typed_Constructor_Copies_Input_Vertices)
    {
        std::unique_ptr<test_vertex_t[]> vertices(new test_vertex_t[2]);
        vertices[0] = { 2, 4, 5 };
        vertices[1] = { 10, 12, 14 };

        auto layout = std::make_shared<InputLayoutDescription>();
        layout->addAttribute(InputAttribute::SemanticName::Position, 0, InputAttribute::StorageType::Float, 3);

        // Construct vertex buffer with typed vertices and ensure two vertices copied.
        VertexBufferData vb(2, std::move(vertices), layout);

        Assert::AreEqual(2, (((test_vertex_t*)vb.bytes()) + 0)->x);
        Assert::AreEqual(4, (((test_vertex_t*)vb.bytes()) + 0)->y);
        Assert::AreEqual(5, (((test_vertex_t*)vb.bytes()) + 0)->z);

        Assert::AreEqual(10, (((test_vertex_t*)vb.bytes()) + 1)->x);
        Assert::AreEqual(12, (((test_vertex_t*)vb.bytes()) + 1)->y);
        Assert::AreEqual(14, (((test_vertex_t*)vb.bytes()) + 1)->z);

        // Check the input layout was copied.
        Assert::IsTrue(layout.get() == vb.inputLayout().get());
    }

    TEST_METHOD(Vertex_Buffer_Typed_Constructor_Throws_Exception_If_Given_Empty_Input_Layout)
    {
        Assert::ExpectException<DaybreakDataException>([]() {
            std::unique_ptr<test_vertex_t[]> vertices(new test_vertex_t[2]);
            vertices[0] = { 2, 4, 5 };
            vertices[1] = { 10, 12, 14 };

            VertexBufferData vb(2, std::move(vertices), std::make_shared<InputLayoutDescription>());
        });
    }

    TEST_METHOD(Vertex_Buffer_Typed_Constructor_Throws_Exception_Zero_Vertices)
    {
        auto layout = std::make_shared<InputLayoutDescription>();
        layout->addAttribute(InputAttribute::SemanticName::Position, 0, InputAttribute::StorageType::Float, 3);

        Assert::ExpectException<DaybreakDataException>([&layout]() {
            std::unique_ptr<test_vertex_t[]> vertices(new test_vertex_t[2]);
            vertices[0] = { 2, 4, 5 };

            VertexBufferData vb(0, std::move(vertices), layout);
        });
    }

    TEST_METHOD(Single_Attribute_Stream_Iterator)
    {
        std::unique_ptr<test_vertex_t[]> vertices(new test_vertex_t[2]);
        vertices[0] = { 2, 4, 5 };
        vertices[1] = { 10, 12, 14 };

        auto layout = std::make_shared<InputLayoutDescription>();
        layout->addAttribute(InputAttribute::SemanticName::Position, 0, InputAttribute::StorageType::Float, 3);

        // Construct vertex buffer and try to read both elements in the position attribute stream.
        VertexBufferData vb(2, std::move(vertices), layout);
        auto positionItr = vb.attributeBegin<ivec3>(InputAttribute::SemanticName::Position);

        Assert::AreEqual(2, positionItr->x);
        Assert::AreEqual(4, positionItr->y);
        Assert::AreEqual(5, positionItr->z);

        positionItr++;

        Assert::AreEqual(10, positionItr->x);
        Assert::AreEqual(12, positionItr->y);
        Assert::AreEqual(14, positionItr->z);
    }
};