#include "stdafx.h"
#include "CppUnitTest.h"

#include "Common/Error.h"
#include "Graphics/Mesh/VertexAttributeIterator.h"

#include <sstream>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Daybreak;

template<typename T> struct vec2
{
    vec2(T x, T y)
        : x(x), y(y)
    {
    }

    bool operator ==(const vec2& v) const
    {
        return x == v.x && y == v.y;
    }

    T x;
    T y;
};

template<typename T> struct vec3
{
    vec3(T x, T y, T z)
        : x(x), y(y), z(z)
    {
    }

    bool operator ==(const vec3& v) const
    {
        return x == v.x && y == v.y && z == v.z;
    }

    T x;
    T y;
    T z;
};

namespace Microsoft {
    namespace VisualStudio {
        namespace CppUnitTestFramework {
            template<>
            std::wstring ToString<>(const vec2<float>& v)
            {
                std::wstringstream ss;
                ss << v.x << ", " << v.y;
                return ss.str();
            }

            template<>
            std::wstring ToString<>(const vec3<int>& v)
            {
                std::wstringstream ss;
                ss << v.x << ", " << v.y << ", " << v.z;
                return ss.str();
            }

            template<>
            std::wstring ToString<>(const vec3<char>& v)
            {
                std::wstringstream ss;
                ss << v.x << ", " << v.y<< ", " << v.z;
                return ss.str();
            }
        }
    }
}

TEST_CLASS(VertexAttributeIteratorTests)
{
public:
    TEST_METHOD(Iterate_Vertex_Position_Attribute)
    {
        //VertexAttributeIterator<int> start(&TestData[0], 0, sizeof(test_vertex_t));
        //VertexAttributeIterator<int> end(&TestData[TestDataCount], 0, sizeof(test_vertex_t));
       
     //   Assert::AreEqual()
    }

    TEST_METHOD(Position_Iterator_Index_Of_Returns_Correct_Value)
    {
        VertexAttributeIterator<vec3<int>> itr(&TestData[0], 0, sizeof(test_vertex_t));

        Assert::AreEqual({ 02, 03, 05 }, itr[0]);
        Assert::AreEqual({ 12, 23, 35 }, itr[1]);
        Assert::AreEqual({ 22, 43, 65 }, itr[2]);
        Assert::AreEqual({ 32, 63, 95 }, itr[3]);
    }

    TEST_METHOD(UV_Iterator_Index_Of_Returns_Correct_Value)
    {
        VertexAttributeIterator<vec2<float>> itr(
            &TestData[0],
            sizeof(vec3<int>),
            sizeof(test_vertex_t));

        Assert::AreEqual({ 02.2f, 04.2f }, itr[0]);
        Assert::AreEqual({ 12.2f, 24.2f }, itr[1]);
        Assert::AreEqual({ 22.2f, 44.2f }, itr[2]);
        Assert::AreEqual({ 32.2f, 64.2f }, itr[3]);
    }

    TEST_METHOD(Normal_Iterator_Index_Of_Returns_Correct_Value)
    {
        VertexAttributeIterator<vec3<char>> itr(
            &TestData[0],
            sizeof(vec3<int>) + sizeof(vec2<float>),
            sizeof(test_vertex_t));

        Assert::AreEqual({ 'o', 'n', 'e' }, itr[0]);
        Assert::AreEqual({ 't', 'w', 'o' }, itr[1]);
        Assert::AreEqual({ 't', 'h', 'r' }, itr[2]);
        Assert::AreEqual({ 'f', 'o', 'u' }, itr[3]);
    }

    TEST_METHOD(Position_Const_Iterator_Index_Of_Returns_Correct_Value)
    {
        const VertexAttributeIterator<vec3<int>> itr(&TestData[0], 0, sizeof(test_vertex_t));

        Assert::AreEqual({ 02, 03, 05 }, itr[0]);
        Assert::AreEqual({ 12, 23, 35 }, itr[1]);
        Assert::AreEqual({ 22, 43, 65 }, itr[2]);
        Assert::AreEqual({ 32, 63, 95 }, itr[3]);
    }

    TEST_METHOD(UV_Const_Iterator_Index_Of_Returns_Correct_Value)
    {
        const VertexAttributeIterator<vec2<float>> itr(
            &TestData[0],
            sizeof(vec3<int>),
            sizeof(test_vertex_t));

        Assert::AreEqual({ 02.2f, 04.2f }, itr[0]);
        Assert::AreEqual({ 12.2f, 24.2f }, itr[1]);
        Assert::AreEqual({ 22.2f, 44.2f }, itr[2]);
        Assert::AreEqual({ 32.2f, 64.2f }, itr[3]);
    }

    TEST_METHOD(Normal_Const_Iterator_Index_Of_Returns_Correct_Value)
    {
        const VertexAttributeIterator<vec3<char>> itr(
            &TestData[0],
            sizeof(vec3<int>) + sizeof(vec2<float>),
            sizeof(test_vertex_t));

        Assert::AreEqual({ 'o', 'n', 'e' }, itr[0]);
        Assert::AreEqual({ 't', 'w', 'o' }, itr[1]);
        Assert::AreEqual({ 't', 'h', 'r' }, itr[2]);
        Assert::AreEqual({ 'f', 'o', 'u' }, itr[3]);
    }

    TEST_METHOD(Position_Const_Iterator_Increment_And_Dereference)
    {
        VertexAttributeIterator<vec3<int>> itr(&TestData[0], 0, sizeof(test_vertex_t));

        Assert::AreEqual({ 02, 03, 05 }, *itr);

        itr++;
        Assert::AreEqual({ 12, 23, 35 }, *itr);

        itr++;
        Assert::AreEqual({ 22, 43, 65 }, *itr);
    }

    TEST_METHOD(UV_Const_Iterator_Dereference_Increment_And_Derefreence)
    {
        VertexAttributeIterator<vec2<float>> itr(
            &TestData[0],
            sizeof(vec3<int>),
            sizeof(test_vertex_t));

        Assert::AreEqual({ 02.2f, 04.2f }, *itr);

        itr++;
        Assert::AreEqual({ 12.2f, 24.2f }, *itr);

        itr++;
        Assert::AreEqual({ 22.2f, 44.2f }, *itr);
    }

    TEST_METHOD(Normal_Const_Iterator_Dereference_Increment_And_Derefreence)
    {
        VertexAttributeIterator<vec3<char>> itr(
            &TestData[0],
            sizeof(vec3<int>) + sizeof(vec2<float>),
            sizeof(test_vertex_t));

        Assert::AreEqual({ 'o', 'n', 'e' }, *itr);
        
        itr++;
        Assert::AreEqual({ 't', 'w', 'o' }, *itr);

        itr++;
        Assert::AreEqual({ 't', 'h', 'r' }, *itr);
    }

    TEST_METHOD(Position_Iterator_PointerTo)
    {
        VertexAttributeIterator<vec3<int>> itr(&TestData[0], 0, sizeof(test_vertex_t));

        Assert::AreEqual(2, itr->x);
        Assert::AreEqual(3, itr->y);
        Assert::AreEqual(5, itr->z);

        itr++;
        
        Assert::AreEqual(12, itr->x);
        Assert::AreEqual(23, itr->y);
        Assert::AreEqual(35, itr->z);
    }

    TEST_METHOD(UV_Iterator_PointerTo)
    {
        VertexAttributeIterator<vec2<float>> itr(
            &TestData[0],
            sizeof(vec3<int>),
            sizeof(test_vertex_t));

        Assert::AreEqual(2.2f, itr->x);
        Assert::AreEqual(4.2f, itr->y);

        itr++;

        Assert::AreEqual(12.2f, itr->x);
        Assert::AreEqual(24.2f, itr->y);
    }

    TEST_METHOD(Normal_Iterator_PointerTo)
    {
        VertexAttributeIterator<vec3<char>> itr(
            &TestData[0],
            sizeof(vec3<int>) + sizeof(vec2<float>),
            sizeof(test_vertex_t));

        Assert::AreEqual('o', itr->x);
        Assert::AreEqual('n', itr->y);
        Assert::AreEqual('e', itr->z);

        itr++;

        Assert::AreEqual('t', itr->x);
        Assert::AreEqual('w', itr->y);
        Assert::AreEqual('o', itr->z);
    }

    TEST_METHOD(Iterator_Addition_Increments_By_Stride_Multiple)
    {
        VertexAttributeIterator<vec3<int>> itr(&TestData[0], 0, sizeof(test_vertex_t));
        VertexAttributeIterator<vec3<int>> one(&TestData[1], 0, sizeof(test_vertex_t));
        VertexAttributeIterator<vec3<int>> two(&TestData[2], 0, sizeof(test_vertex_t));

        Assert::AreEqual(*(itr + 1), *one);
        Assert::AreEqual(*(itr + 2), *two);
    }

    TEST_METHOD(Iterator_Self_Addition_Increment_By_Stride_Multiple)
    {
        VertexAttributeIterator<vec2<float>> itr(&TestData[0], sizeof(vec3<int>), sizeof(test_vertex_t));
        VertexAttributeIterator<vec2<float>> one(&TestData[1], sizeof(vec3<int>), sizeof(test_vertex_t));
        VertexAttributeIterator<vec2<float>> three(&TestData[3], sizeof(vec3<int>), sizeof(test_vertex_t));

        itr += 1;
        Assert::AreEqual(*itr, *one);

        itr += 2;
        Assert::AreEqual(*itr, *three);
    }

    TEST_METHOD(Iterator_Subtraction_Decrement_By_Stride_Multiple)
    {
        const auto offset = sizeof(vec3<int>) + sizeof(vec2<float>);
        VertexAttributeIterator<vec3<char>> itr(&TestData[3], offset, sizeof(test_vertex_t));
        VertexAttributeIterator<vec3<char>> one(&TestData[1], offset, sizeof(test_vertex_t));
        VertexAttributeIterator<vec3<char>> two(&TestData[2], offset, sizeof(test_vertex_t));

        Assert::AreEqual(*(itr - 2), *one);
        Assert::AreEqual(*(itr - 1), *two);
    }

    TEST_METHOD(Iterator_Self_Subtraction_Decrements_By_Stride_Multiple)
    {
        VertexAttributeIterator<vec3<int>> itr(&TestData[3], 0, sizeof(test_vertex_t));
        VertexAttributeIterator<vec3<int>> two(&TestData[2], 0, sizeof(test_vertex_t));
        VertexAttributeIterator<vec3<int>> zero(&TestData[0], 0, sizeof(test_vertex_t));

        itr -= 1;
        Assert::AreEqual(*itr, *two);

        itr -= 2;
        Assert::AreEqual(*itr, *zero);
    }

    TEST_METHOD(Difference_Between_Iterators_Is_A_Stride_Multiple)
    {
        VertexAttributeIterator<vec2<float>> itr(&TestData[0], sizeof(vec3<int>), sizeof(test_vertex_t));
        VertexAttributeIterator<vec2<float>> one(&TestData[1], sizeof(vec3<int>), sizeof(test_vertex_t));
        VertexAttributeIterator<vec2<float>> two(&TestData[2], sizeof(vec3<int>), sizeof(test_vertex_t));
        VertexAttributeIterator<vec2<float>> three(&TestData[3], sizeof(vec3<int>), sizeof(test_vertex_t));

        Assert::AreEqual(0, (int)(itr - itr));
        Assert::AreEqual(1, (int)(one - itr));
        Assert::AreEqual(2, (int)(two - itr));
        Assert::AreEqual(3, (int)(three - itr));
    }

    TEST_METHOD(Postfix_Incremnt)
    {
        VertexAttributeIterator<vec2<float>> itr(&TestData[0], sizeof(vec3<int>), sizeof(test_vertex_t));
        VertexAttributeIterator<vec2<float>> zero(&TestData[0], sizeof(vec3<int>), sizeof(test_vertex_t));
        VertexAttributeIterator<vec2<float>> one(&TestData[1], sizeof(vec3<int>), sizeof(test_vertex_t));
        VertexAttributeIterator<vec2<float>> two(&TestData[2], sizeof(vec3<int>), sizeof(test_vertex_t));
        VertexAttributeIterator<vec2<float>> three(&TestData[3], sizeof(vec3<int>), sizeof(test_vertex_t));

        auto i = itr++;
        Assert::AreEqual(*zero, *i);

        i = itr++;
        Assert::AreEqual(*one, *i);

        i = itr++;
        Assert::AreEqual(*two, *i);

        i = itr++;
        Assert::AreEqual(*three, *i);
    }

    TEST_METHOD(Prefix_Increment)
    {
        const auto offset = sizeof(vec3<int>) + sizeof(vec2<float>);
        VertexAttributeIterator<vec3<char>> itr(&TestData[0], offset, sizeof(test_vertex_t));
        VertexAttributeIterator<vec3<char>> one(&TestData[1], offset, sizeof(test_vertex_t));
        VertexAttributeIterator<vec3<char>> two(&TestData[2], offset, sizeof(test_vertex_t));
        VertexAttributeIterator<vec3<char>> three(&TestData[3], offset, sizeof(test_vertex_t));

        auto i = ++itr;
        Assert::AreEqual(*one, *i);

        i = ++itr;
        Assert::AreEqual(*two, *i);

        i = ++itr;
        Assert::AreEqual(*three, *i);
    }

    TEST_METHOD(Postfix_Decrement)
    {
        VertexAttributeIterator<vec2<float>> itr(&TestData[3], sizeof(vec3<int>), sizeof(test_vertex_t));
        VertexAttributeIterator<vec2<float>> zero(&TestData[0], sizeof(vec3<int>), sizeof(test_vertex_t));
        VertexAttributeIterator<vec2<float>> one(&TestData[1], sizeof(vec3<int>), sizeof(test_vertex_t));
        VertexAttributeIterator<vec2<float>> two(&TestData[2], sizeof(vec3<int>), sizeof(test_vertex_t));
        VertexAttributeIterator<vec2<float>> three(&TestData[3], sizeof(vec3<int>), sizeof(test_vertex_t));

        auto i = itr--;
        Assert::AreEqual(*three, *i);

        i = itr--;
        Assert::AreEqual(*two, *i);

        i = itr--;
        Assert::AreEqual(*one, *i);

        i = itr--;
        Assert::AreEqual(*zero, *i);
    }

    TEST_METHOD(Prefix_Decrement)
    {
        const auto offset = sizeof(vec3<int>) + sizeof(vec2<float>);
        VertexAttributeIterator<vec3<char>> itr(&TestData[3], offset, sizeof(test_vertex_t));
        VertexAttributeIterator<vec3<char>> zero(&TestData[0], offset, sizeof(test_vertex_t));
        VertexAttributeIterator<vec3<char>> one(&TestData[1], offset, sizeof(test_vertex_t));
        VertexAttributeIterator<vec3<char>> two(&TestData[2], offset, sizeof(test_vertex_t));

        auto i = --itr;
        Assert::AreEqual(*two, *i);

        i = --itr;
        Assert::AreEqual(*one, *i);

        i = --itr;
        Assert::AreEqual(*zero, *i);
    }

    TEST_METHOD(Iterator_Equality)
    {
        VertexAttributeIterator<vec2<float>> one(&TestData[1], sizeof(vec3<int>), sizeof(test_vertex_t));
        VertexAttributeIterator<vec2<float>> two(&TestData[2], sizeof(vec3<int>), sizeof(test_vertex_t));

        Assert::IsTrue(one == one);
        Assert::IsTrue(two == two);
        Assert::IsFalse(one == two);
        Assert::IsFalse(two == one);
    }

    TEST_METHOD(Iterator_Inequality)
    {
        const auto offset = sizeof(vec3<int>) + sizeof(vec2<float>);
        VertexAttributeIterator<vec3<char>> one(&TestData[1], offset, sizeof(test_vertex_t));
        VertexAttributeIterator<vec3<char>> two(&TestData[2], offset, sizeof(test_vertex_t));

        Assert::IsTrue(one != two);
        Assert::IsTrue(two != one);
        Assert::IsFalse(one != one);
        Assert::IsFalse(two != two);
    }

    TEST_METHOD(Less_Than_Comparison)
    {
        VertexAttributeIterator<vec2<float>> zero(&TestData[0], sizeof(vec3<int>), sizeof(test_vertex_t));
        VertexAttributeIterator<vec2<float>> one(&TestData[1], sizeof(vec3<int>), sizeof(test_vertex_t));
        VertexAttributeIterator<vec2<float>> two(&TestData[2], sizeof(vec3<int>), sizeof(test_vertex_t));
        VertexAttributeIterator<vec2<float>> three(&TestData[3], sizeof(vec3<int>), sizeof(test_vertex_t));

        Assert::IsTrue(zero < one);
        Assert::IsTrue(zero < two);
        Assert::IsTrue(one < two);
        Assert::IsTrue(two < three);

        Assert::IsFalse(one < one);
        Assert::IsFalse(three < two);
    }

    TEST_METHOD(Less_Than_Equal_Comparison)
    {
        VertexAttributeIterator<vec2<float>> zero(&TestData[0], sizeof(vec3<int>), sizeof(test_vertex_t));
        VertexAttributeIterator<vec2<float>> one(&TestData[1], sizeof(vec3<int>), sizeof(test_vertex_t));
        VertexAttributeIterator<vec2<float>> two(&TestData[2], sizeof(vec3<int>), sizeof(test_vertex_t));
        VertexAttributeIterator<vec2<float>> three(&TestData[3], sizeof(vec3<int>), sizeof(test_vertex_t));

        Assert::IsTrue(zero <= zero);
        Assert::IsTrue(two <= two);

        Assert::IsTrue(zero <= one);
        Assert::IsTrue(zero <= two);
        Assert::IsTrue(one <= two);
        Assert::IsTrue(two <= three);

        Assert::IsFalse(two <= zero);
        Assert::IsFalse(three <= two);
    }

    TEST_METHOD(Greater_Than_Comparison)
    {
        VertexAttributeIterator<vec2<float>> zero(&TestData[0], sizeof(vec3<int>), sizeof(test_vertex_t));
        VertexAttributeIterator<vec2<float>> one(&TestData[1], sizeof(vec3<int>), sizeof(test_vertex_t));
        VertexAttributeIterator<vec2<float>> two(&TestData[2], sizeof(vec3<int>), sizeof(test_vertex_t));
        VertexAttributeIterator<vec2<float>> three(&TestData[3], sizeof(vec3<int>), sizeof(test_vertex_t));

        Assert::IsTrue(one > zero);
        Assert::IsTrue(three > one);
        Assert::IsTrue(two > one);
        Assert::IsTrue(three > two);

        Assert::IsFalse(one > one);
        Assert::IsFalse(two > three);
    }

    TEST_METHOD(Greater_Than_Equal_Comparison)
    {
        VertexAttributeIterator<vec2<float>> zero(&TestData[0], sizeof(vec3<int>), sizeof(test_vertex_t));
        VertexAttributeIterator<vec2<float>> one(&TestData[1], sizeof(vec3<int>), sizeof(test_vertex_t));
        VertexAttributeIterator<vec2<float>> two(&TestData[2], sizeof(vec3<int>), sizeof(test_vertex_t));
        VertexAttributeIterator<vec2<float>> three(&TestData[3], sizeof(vec3<int>), sizeof(test_vertex_t));

        Assert::IsTrue(zero >= zero);
        Assert::IsTrue(two >= two);

        Assert::IsTrue(one >= zero);
        Assert::IsTrue(two >= zero);
        Assert::IsTrue(two >= one);
        Assert::IsTrue(three >= two);

        Assert::IsFalse(zero >= two);
        Assert::IsFalse(two >= three);
    }

    struct test_vertex_t
    {
        vec3<int> pos;
        vec2<float> uv;
        vec3<char> normal;
    };

    const size_t TestDataCount = 4;
    /*const*/ test_vertex_t TestData[4] =
    {
        { {  2,  3,  5 }, { 02.2f, 04.2f }, { 'o', 'n', 'e' } },
        { { 12, 23, 35 }, { 12.2f, 24.2f }, { 't', 'w', 'o' } },
        { { 22, 43, 65 }, { 22.2f, 44.2f }, { 't', 'h', 'r' } },
        { { 32, 63, 95 }, { 32.2f, 64.2f }, { 'f', 'o', 'u' } },
    };
};