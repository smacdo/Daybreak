#include "stdafx.h"
#include "Graphics/Mesh/VertexAttributeIterator.h"
#include "../../TestHelpers.h"

using namespace Daybreak;

namespace
{
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
}

TEST_CASE("Iterate_Vertex_Position_Attribute", "[graphics][VertexAttributeIterator]")
{
    //VertexAttributeIterator<int> start(&TestData[0], 0, sizeof(test_vertex_t));
    //VertexAttributeIterator<int> end(&TestData[TestDataCount], 0, sizeof(test_vertex_t));
       
    //   Assert::AreEqual()
}

TEST_CASE("Position_Iterator_Index_Of_Returns_Correct_Value", "[graphics][VertexAttributeIterator]")
{
    VertexAttributeIterator<vec3<int>> itr(&TestData[0], 0, sizeof(test_vertex_t));

    REQUIRE(vec3{ 02, 03, 05 } == itr[0]);
    REQUIRE(vec3{ 12, 23, 35 } == itr[1]);
    REQUIRE(vec3{ 22, 43, 65 } == itr[2]);
    REQUIRE(vec3{ 32, 63, 95 } == itr[3]);
}

TEST_CASE("UV_Iterator_Index_Of_Returns_Correct_Value", "[graphics][VertexAttributeIterator]")
{
    VertexAttributeIterator<vec2<float>> itr(
        &TestData[0],
        sizeof(vec3<int>),
        sizeof(test_vertex_t));

    REQUIRE(vec2{ 02.2f, 04.2f } == itr[0]);
    REQUIRE(vec2{ 12.2f, 24.2f } == itr[1]);
    REQUIRE(vec2{ 22.2f, 44.2f } == itr[2]);
    REQUIRE(vec2{ 32.2f, 64.2f } == itr[3]);
}

TEST_CASE("Normal_Iterator_Index_Of_Returns_Correct_Value", "[graphics][VertexAttributeIterator]")
{
    VertexAttributeIterator<vec3<char>> itr(
        &TestData[0],
        sizeof(vec3<int>) + sizeof(vec2<float>),
        sizeof(test_vertex_t));

    REQUIRE(vec3{ 'o', 'n', 'e' } == itr[0]);
    REQUIRE(vec3{ 't', 'w', 'o' } == itr[1]);
    REQUIRE(vec3{ 't', 'h', 'r' } == itr[2]);
    REQUIRE(vec3{ 'f', 'o', 'u' } == itr[3]);
}

TEST_CASE("Position_Const_Iterator_Index_Of_Returns_Correct_Value", "[graphics][VertexAttributeIterator]")
{
    const VertexAttributeIterator<vec3<int>> itr(&TestData[0], 0, sizeof(test_vertex_t));

    REQUIRE(vec3{ 02, 03, 05 } == itr[0]);
    REQUIRE(vec3{ 12, 23, 35 } == itr[1]);
    REQUIRE(vec3{ 22, 43, 65 } == itr[2]);
    REQUIRE(vec3{ 32, 63, 95 } == itr[3]);
}

TEST_CASE("UV_Const_Iterator_Index_Of_Returns_Correct_Value", "[graphics][VertexAttributeIterator]")
{
    const VertexAttributeIterator<vec2<float>> itr(
        &TestData[0],
        sizeof(vec3<int>),
        sizeof(test_vertex_t));

    REQUIRE(vec2{ 02.2f, 04.2f } == itr[0]);
    REQUIRE(vec2{ 12.2f, 24.2f } == itr[1]);
    REQUIRE(vec2{ 22.2f, 44.2f } == itr[2]);
    REQUIRE(vec2{ 32.2f, 64.2f } == itr[3]);
}

TEST_CASE("Normal_Const_Iterator_Index_Of_Returns_Correct_Value", "[graphics][VertexAttributeIterator]")
{
    const VertexAttributeIterator<vec3<char>> itr(
        &TestData[0],
        sizeof(vec3<int>) + sizeof(vec2<float>),
        sizeof(test_vertex_t));

    REQUIRE(vec3{ 'o', 'n', 'e' } == itr[0]);
    REQUIRE(vec3{ 't', 'w', 'o' } == itr[1]);
    REQUIRE(vec3{ 't', 'h', 'r' } == itr[2]);
    REQUIRE(vec3{ 'f', 'o', 'u' } == itr[3]);
}

TEST_CASE("Position_Const_Iterator_Increment_And_Dereference", "[graphics][VertexAttributeIterator]")
{
    VertexAttributeIterator<vec3<int>> itr(&TestData[0], 0, sizeof(test_vertex_t));

    REQUIRE(vec3{ 02, 03, 05 } == *itr);

    itr++;
    REQUIRE(vec3{ 12, 23, 35 } == *itr);

    itr++;
    REQUIRE(vec3{ 22, 43, 65 } == *itr);
}

TEST_CASE("UV_Const_Iterator_Dereference_Increment_And_Derefreence", "[graphics][VertexAttributeIterator]")
{
    VertexAttributeIterator<vec2<float>> itr(
        &TestData[0],
        sizeof(vec3<int>),
        sizeof(test_vertex_t));

    REQUIRE(vec2{ 02.2f, 04.2f } == *itr);

    itr++;
    REQUIRE(vec2{ 12.2f, 24.2f } == *itr);

    itr++;
    REQUIRE(vec2{ 22.2f, 44.2f } == *itr);
}

TEST_CASE("Normal_Const_Iterator_Dereference_Increment_And_Derefreence", "[graphics][VertexAttributeIterator]")
{
    VertexAttributeIterator<vec3<char>> itr(
        &TestData[0],
        sizeof(vec3<int>) + sizeof(vec2<float>),
        sizeof(test_vertex_t));

    REQUIRE(vec3{ 'o', 'n', 'e' } == *itr);
        
    itr++;
    REQUIRE(vec3{ 't', 'w', 'o' } == *itr);

    itr++;
    REQUIRE(vec3{ 't', 'h', 'r' } == *itr);
}

TEST_CASE("Position_Iterator_PointerTo", "[graphics][VertexAttributeIterator]")
{
    VertexAttributeIterator<vec3<int>> itr(&TestData[0], 0, sizeof(test_vertex_t));

    REQUIRE(2 == itr->x);
    REQUIRE(3 == itr->y);
    REQUIRE(5 == itr->z);

    itr++;
        
    REQUIRE(12 == itr->x);
    REQUIRE(23 == itr->y);
    REQUIRE(35 == itr->z);
}

TEST_CASE("UV_Iterator_PointerTo", "[graphics][VertexAttributeIterator]")
{
    VertexAttributeIterator<vec2<float>> itr(
        &TestData[0],
        sizeof(vec3<int>),
        sizeof(test_vertex_t));

    REQUIRE(2.2f == itr->x);
    REQUIRE(4.2f == itr->y);

    itr++;

    REQUIRE(12.2f == itr->x);
    REQUIRE(24.2f == itr->y);
}

TEST_CASE("Normal_Iterator_PointerTo", "[graphics][VertexAttributeIterator]")
{
    VertexAttributeIterator<vec3<char>> itr(
        &TestData[0],
        sizeof(vec3<int>) + sizeof(vec2<float>),
        sizeof(test_vertex_t));

    REQUIRE('o' == itr->x);
    REQUIRE('n' == itr->y);
    REQUIRE('e' == itr->z);

    itr++;

    REQUIRE('t' == itr->x);
    REQUIRE('w' == itr->y);
    REQUIRE('o' == itr->z);
}

TEST_CASE("Iterator_Addition_Increments_By_Stride_Multiple", "[graphics][VertexAttributeIterator]")
{
    VertexAttributeIterator<vec3<int>> itr(&TestData[0], 0, sizeof(test_vertex_t));
    VertexAttributeIterator<vec3<int>> one(&TestData[1], 0, sizeof(test_vertex_t));
    VertexAttributeIterator<vec3<int>> two(&TestData[2], 0, sizeof(test_vertex_t));

    REQUIRE(*(itr + 1) == *one);
    REQUIRE(*(itr + 2) == *two);
}

TEST_CASE("Iterator_Self_Addition_Increment_By_Stride_Multiple", "[graphics][VertexAttributeIterator]")
{
    VertexAttributeIterator<vec2<float>> itr(&TestData[0], sizeof(vec3<int>), sizeof(test_vertex_t));
    VertexAttributeIterator<vec2<float>> one(&TestData[1], sizeof(vec3<int>), sizeof(test_vertex_t));
    VertexAttributeIterator<vec2<float>> three(&TestData[3], sizeof(vec3<int>), sizeof(test_vertex_t));

    itr += 1;
    REQUIRE(*itr == *one);

    itr += 2;
    REQUIRE(*itr == *three);
}

TEST_CASE("Iterator_Subtraction_Decrement_By_Stride_Multiple", "[graphics][VertexAttributeIterator]")
{
    const auto offset = sizeof(vec3<int>) + sizeof(vec2<float>);
    VertexAttributeIterator<vec3<char>> itr(&TestData[3], offset, sizeof(test_vertex_t));
    VertexAttributeIterator<vec3<char>> one(&TestData[1], offset, sizeof(test_vertex_t));
    VertexAttributeIterator<vec3<char>> two(&TestData[2], offset, sizeof(test_vertex_t));

    REQUIRE(*(itr - 2) == *one);
    REQUIRE(*(itr - 1) == *two);
}

TEST_CASE("Iterator_Self_Subtraction_Decrements_By_Stride_Multiple", "[graphics][VertexAttributeIterator]")
{
    VertexAttributeIterator<vec3<int>> itr(&TestData[3], 0, sizeof(test_vertex_t));
    VertexAttributeIterator<vec3<int>> two(&TestData[2], 0, sizeof(test_vertex_t));
    VertexAttributeIterator<vec3<int>> zero(&TestData[0], 0, sizeof(test_vertex_t));

    itr -= 1;
    REQUIRE(*itr == *two);

    itr -= 2;
    REQUIRE(*itr == *zero);
}

TEST_CASE("Difference_Between_Iterators_Is_A_Stride_Multiple", "[graphics][VertexAttributeIterator]")
{
    VertexAttributeIterator<vec2<float>> itr(&TestData[0], sizeof(vec3<int>), sizeof(test_vertex_t));
    VertexAttributeIterator<vec2<float>> one(&TestData[1], sizeof(vec3<int>), sizeof(test_vertex_t));
    VertexAttributeIterator<vec2<float>> two(&TestData[2], sizeof(vec3<int>), sizeof(test_vertex_t));
    VertexAttributeIterator<vec2<float>> three(&TestData[3], sizeof(vec3<int>), sizeof(test_vertex_t));

    REQUIRE(0 == (int)(itr - itr));
    REQUIRE(1 == (int)(one - itr));
    REQUIRE(2 == (int)(two - itr));
    REQUIRE(3 == (int)(three - itr));
}

TEST_CASE("Postfix_Incremnt", "[graphics][VertexAttributeIterator]")
{
    VertexAttributeIterator<vec2<float>> itr(&TestData[0], sizeof(vec3<int>), sizeof(test_vertex_t));
    VertexAttributeIterator<vec2<float>> zero(&TestData[0], sizeof(vec3<int>), sizeof(test_vertex_t));
    VertexAttributeIterator<vec2<float>> one(&TestData[1], sizeof(vec3<int>), sizeof(test_vertex_t));
    VertexAttributeIterator<vec2<float>> two(&TestData[2], sizeof(vec3<int>), sizeof(test_vertex_t));
    VertexAttributeIterator<vec2<float>> three(&TestData[3], sizeof(vec3<int>), sizeof(test_vertex_t));

    auto i = itr++;
    REQUIRE(*zero == *i);

    i = itr++;
    REQUIRE(*one == *i);

    i = itr++;
    REQUIRE(*two == *i);

    i = itr++;
    REQUIRE(*three == *i);
}

TEST_CASE("Prefix_Increment", "[graphics][VertexAttributeIterator]")
{
    const auto offset = sizeof(vec3<int>) + sizeof(vec2<float>);
    VertexAttributeIterator<vec3<char>> itr(&TestData[0], offset, sizeof(test_vertex_t));
    VertexAttributeIterator<vec3<char>> one(&TestData[1], offset, sizeof(test_vertex_t));
    VertexAttributeIterator<vec3<char>> two(&TestData[2], offset, sizeof(test_vertex_t));
    VertexAttributeIterator<vec3<char>> three(&TestData[3], offset, sizeof(test_vertex_t));

    auto i = ++itr;
    REQUIRE(*one == *i);

    i = ++itr;
    REQUIRE(*two == *i);

    i = ++itr;
    REQUIRE(*three == *i);
}

TEST_CASE("Postfix_Decrement", "[graphics][VertexAttributeIterator]")
{
    VertexAttributeIterator<vec2<float>> itr(&TestData[3], sizeof(vec3<int>), sizeof(test_vertex_t));
    VertexAttributeIterator<vec2<float>> zero(&TestData[0], sizeof(vec3<int>), sizeof(test_vertex_t));
    VertexAttributeIterator<vec2<float>> one(&TestData[1], sizeof(vec3<int>), sizeof(test_vertex_t));
    VertexAttributeIterator<vec2<float>> two(&TestData[2], sizeof(vec3<int>), sizeof(test_vertex_t));
    VertexAttributeIterator<vec2<float>> three(&TestData[3], sizeof(vec3<int>), sizeof(test_vertex_t));

    auto i = itr--;
    REQUIRE(*three == *i);

    i = itr--;
    REQUIRE(*two == *i);

    i = itr--;
    REQUIRE(*one == *i);

    i = itr--;
    REQUIRE(*zero == *i);
}

TEST_CASE("Prefix_Decrement", "[graphics][VertexAttributeIterator]")
{
    const auto offset = sizeof(vec3<int>) + sizeof(vec2<float>);
    VertexAttributeIterator<vec3<char>> itr(&TestData[3], offset, sizeof(test_vertex_t));
    VertexAttributeIterator<vec3<char>> zero(&TestData[0], offset, sizeof(test_vertex_t));
    VertexAttributeIterator<vec3<char>> one(&TestData[1], offset, sizeof(test_vertex_t));
    VertexAttributeIterator<vec3<char>> two(&TestData[2], offset, sizeof(test_vertex_t));

    auto i = --itr;
    REQUIRE(*two == *i);

    i = --itr;
    REQUIRE(*one == *i);

    i = --itr;
    REQUIRE(*zero == *i);
}

TEST_CASE("Iterator_Equality", "[graphics][VertexAttributeIterator]")
{
    VertexAttributeIterator<vec2<float>> one(&TestData[1], sizeof(vec3<int>), sizeof(test_vertex_t));
    VertexAttributeIterator<vec2<float>> two(&TestData[2], sizeof(vec3<int>), sizeof(test_vertex_t));

    REQUIRE(one == one);
    REQUIRE(two == two);
    REQUIRE(!(one == two));
    REQUIRE(!(two == one));
}

TEST_CASE("Iterator_Inequality", "[graphics][VertexAttributeIterator]")
{
    const auto offset = sizeof(vec3<int>) + sizeof(vec2<float>);
    VertexAttributeIterator<vec3<char>> one(&TestData[1], offset, sizeof(test_vertex_t));
    VertexAttributeIterator<vec3<char>> two(&TestData[2], offset, sizeof(test_vertex_t));

    REQUIRE(one != two);
    REQUIRE(two != one);
    REQUIRE(!(one != one));
    REQUIRE(!(two != two));
}

TEST_CASE("Less_Than_Comparison", "[graphics][VertexAttributeIterator]")
{
    VertexAttributeIterator<vec2<float>> zero(&TestData[0], sizeof(vec3<int>), sizeof(test_vertex_t));
    VertexAttributeIterator<vec2<float>> one(&TestData[1], sizeof(vec3<int>), sizeof(test_vertex_t));
    VertexAttributeIterator<vec2<float>> two(&TestData[2], sizeof(vec3<int>), sizeof(test_vertex_t));
    VertexAttributeIterator<vec2<float>> three(&TestData[3], sizeof(vec3<int>), sizeof(test_vertex_t));

    REQUIRE(zero < one);
    REQUIRE(zero < two);
    REQUIRE(one < two);
    REQUIRE(two < three);

    REQUIRE(!(one < one));
    REQUIRE(!(three < two));
}

TEST_CASE("Less_Than_Equal_Comparison", "[graphics][VertexAttributeIterator]")
{
    VertexAttributeIterator<vec2<float>> zero(&TestData[0], sizeof(vec3<int>), sizeof(test_vertex_t));
    VertexAttributeIterator<vec2<float>> one(&TestData[1], sizeof(vec3<int>), sizeof(test_vertex_t));
    VertexAttributeIterator<vec2<float>> two(&TestData[2], sizeof(vec3<int>), sizeof(test_vertex_t));
    VertexAttributeIterator<vec2<float>> three(&TestData[3], sizeof(vec3<int>), sizeof(test_vertex_t));

    REQUIRE(zero <= zero);
    REQUIRE(two <= two);

    REQUIRE(zero <= one);
    REQUIRE(zero <= two);
    REQUIRE(one <= two);
    REQUIRE(two <= three);

    REQUIRE(!(two <= zero));
    REQUIRE(!(three <= two));
}

TEST_CASE("Greater_Than_Comparison", "[graphics][VertexAttributeIterator]")
{
    VertexAttributeIterator<vec2<float>> zero(&TestData[0], sizeof(vec3<int>), sizeof(test_vertex_t));
    VertexAttributeIterator<vec2<float>> one(&TestData[1], sizeof(vec3<int>), sizeof(test_vertex_t));
    VertexAttributeIterator<vec2<float>> two(&TestData[2], sizeof(vec3<int>), sizeof(test_vertex_t));
    VertexAttributeIterator<vec2<float>> three(&TestData[3], sizeof(vec3<int>), sizeof(test_vertex_t));

    REQUIRE(one > zero);
    REQUIRE(three > one);
    REQUIRE(two > one);
    REQUIRE(three > two);

    REQUIRE(!(one > one));
    REQUIRE(!(two > three));
}

TEST_CASE("Greater_Than_Equal_Comparison", "[graphics][VertexAttributeIterator]")
{
    VertexAttributeIterator<vec2<float>> zero(&TestData[0], sizeof(vec3<int>), sizeof(test_vertex_t));
    VertexAttributeIterator<vec2<float>> one(&TestData[1], sizeof(vec3<int>), sizeof(test_vertex_t));
    VertexAttributeIterator<vec2<float>> two(&TestData[2], sizeof(vec3<int>), sizeof(test_vertex_t));
    VertexAttributeIterator<vec2<float>> three(&TestData[3], sizeof(vec3<int>), sizeof(test_vertex_t));

    REQUIRE(zero >= zero);
    REQUIRE(two >= two);

    REQUIRE(one >= zero);
    REQUIRE(two >= zero);
    REQUIRE(two >= one);
    REQUIRE(three >= two);

    REQUIRE(!(zero >= two));
    REQUIRE(!(two >= three));
}
