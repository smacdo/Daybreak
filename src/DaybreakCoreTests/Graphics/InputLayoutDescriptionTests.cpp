#include "stdafx.h"

#include "Common/Error.h"
#include "Graphics/InputLayoutDescription.h"

#include "../TestHelpers.h"

using namespace Daybreak;

TEST_CASE("Input_Layout_Description_Default_Constructor_Creates_Empty_Description", "[graphics][inputlayout]")
{
    InputLayoutDescription d;
    REQUIRE(0 == (int)d.attributeCount());
}

TEST_CASE("Input_Layout_Constructor_Takes_Attribute_Array", "[graphics][inputlayout]")
{
    auto attributes = std::vector<InputAttribute>
    {
        { InputAttribute::SemanticName::Position, 0, InputAttribute::StorageType::Float, 3 },
        { InputAttribute::SemanticName::Texture, 0, InputAttribute::StorageType::Float, 2 },
        { InputAttribute::SemanticName::Normal, 0, InputAttribute::StorageType::Float, 3 }
    };

    InputLayoutDescription d(attributes);
    REQUIRE(3 == (int)d.attributeCount());

    REQUIRE(attributes[0] == d.getAttributeByIndex(0));
    REQUIRE(attributes[1] == d.getAttributeByIndex(1));
    REQUIRE(attributes[2] == d.getAttributeByIndex(2));
}

TEST_CASE("Input_Layout_Keeps_Count_Of_Attributes_Added", "[graphics][inputlayout]")
{
    InputLayoutDescription d;
    REQUIRE(0 == (int)d.attributeCount());

    d.addAttribute(InputAttribute::SemanticName::None, 0, InputAttribute::StorageType::Double, 2);
    REQUIRE(1 == (int)d.attributeCount());

    d.addAttribute(InputAttribute::SemanticName::None, 0, InputAttribute::StorageType::HalfFloat, 3);
    REQUIRE(2 == (int)d.attributeCount());

    d.addAttribute(InputAttribute::SemanticName::None, 0, InputAttribute::StorageType::UnsignedInt, 2);
    REQUIRE(3 == (int)d.attributeCount());
}

TEST_CASE("Input_Layout_Calculates_Size_Of_Element_Using_Attributes", "[graphics][inputlayout]")
{
    InputLayoutDescription d;
    REQUIRE(0 == (int)d.elementSizeInBytes());

    d.addAttribute(InputAttribute::SemanticName::None, 0, InputAttribute::StorageType::Float, 2);
    REQUIRE(8 == (int)d.elementSizeInBytes());

    d.addAttribute(InputAttribute::SemanticName::None, 0, InputAttribute::StorageType::Short, 1);
    REQUIRE(10 == (int)d.elementSizeInBytes());

    d.addAttribute(InputAttribute::SemanticName::None, 0, InputAttribute::StorageType::UnsignedByte, 3);
    REQUIRE(13 == (int)d.elementSizeInBytes());
}

TEST_CASE("Input_Layout_Calculates_Offset_To_Each_Element", "[graphics][inputlayout]")
{
    InputLayoutDescription d(std::vector<InputAttribute>
    {
        { InputAttribute::SemanticName::Position, 0, InputAttribute::StorageType::Short, 3 },
        { InputAttribute::SemanticName::Texture, 0, InputAttribute::StorageType::UnsignedByte, 3 },
        { InputAttribute::SemanticName::Normal, 0, InputAttribute::StorageType::Float, 3 }
    });

    REQUIRE(0 == (int)d.attributeOffsetByIndex(0));
    REQUIRE(6 == (int)d.attributeOffsetByIndex(1));
    REQUIRE(9 == (int)d.attributeOffsetByIndex(2));
}

TEST_CASE("Input_Layout_Finds_Attribute_Index_By_Semantic_Name", "[graphics][inputlayout]")
{
    InputLayoutDescription d(std::vector<InputAttribute>
    {
        { InputAttribute::SemanticName::Position, 0, InputAttribute::StorageType::Short, 3 },
        { InputAttribute::SemanticName::Texture, 0, InputAttribute::StorageType::UnsignedByte, 3 },
        { InputAttribute::SemanticName::Normal, 0, InputAttribute::StorageType::Float, 3 }
    });

    REQUIRE(0 == (int)d.getAttributeIndexByName(InputAttribute::SemanticName::Position,  0));
    REQUIRE(1 == (int)d.getAttributeIndexByName(InputAttribute::SemanticName::Texture,  0));
    REQUIRE(2 == (int)d.getAttributeIndexByName(InputAttribute::SemanticName::Normal,  0));
}

TEST_CASE("Input_Layout_Finds_Attribute_Index_By_Semantic_Name_And_Index", "[graphics][inputlayout]")
{
    InputLayoutDescription d(std::vector<InputAttribute>
    {
        { InputAttribute::SemanticName::Position, 1, InputAttribute::StorageType::Short, 3 },
        { InputAttribute::SemanticName::Texture, 0, InputAttribute::StorageType::UnsignedByte, 3 },
        { InputAttribute::SemanticName::Position, 0, InputAttribute::StorageType::Float, 3 },
        { InputAttribute::SemanticName::Texture, 1, InputAttribute::StorageType::UnsignedByte, 3 },
    });

    REQUIRE(2 == (int)d.getAttributeIndexByName(InputAttribute::SemanticName::Position, 0));
    REQUIRE(0 == (int)d.getAttributeIndexByName(InputAttribute::SemanticName::Position, 1));
    REQUIRE(1 == (int)d.getAttributeIndexByName(InputAttribute::SemanticName::Texture, 0));
    REQUIRE(3 == (int)d.getAttributeIndexByName(InputAttribute::SemanticName::Texture, 1));
}

TEST_CASE("Input_Layout_Find_Attribute_Index_Throws_Exception_If_Name_Not_Found", "[graphics][inputlayout]")
{
    InputLayoutDescription d(std::vector<InputAttribute>
    {
        { InputAttribute::SemanticName::Position, 0, InputAttribute::StorageType::Short, 3 },
        { InputAttribute::SemanticName::Texture, 0, InputAttribute::StorageType::UnsignedByte, 3 }
    });

    // Input layout attribute was not found when searching by semantic name and index
    REQUIRE_THROWS_WITH(
        d.getAttributeIndexByName(InputAttribute::SemanticName::Normal, 0),
        Catch::Contains("Input layout attribute was not found when searching by semantic name and index"));
}

TEST_CASE("Input_Layout_Find_Attribute_Index_Throws_Exception_If_Index_Not_Found", "[graphics][inputlayout]")
{
    InputLayoutDescription d(std::vector<InputAttribute>
    {
        { InputAttribute::SemanticName::Position, 0, InputAttribute::StorageType::Short, 3 },
        { InputAttribute::SemanticName::Texture, 0, InputAttribute::StorageType::UnsignedByte, 3 }
    });

    REQUIRE_THROWS_WITH(
        d.getAttributeIndexByName(InputAttribute::SemanticName::Position, 1),
        Catch::Contains("Input layout attribute was not found when searching by semantic name and index"));
}

TEST_CASE("Input_Layout_Finds_Attribute_By_Semantic_Name_And_Index", "[graphics][inputlayout]")
{
    std::vector<InputAttribute> attributes = {
        { InputAttribute::SemanticName::Position, 1, InputAttribute::StorageType::Short, 3 },
        { InputAttribute::SemanticName::Texture, 0, InputAttribute::StorageType::UnsignedByte, 3 },
        { InputAttribute::SemanticName::Position, 0, InputAttribute::StorageType::Float, 3 }
    };

    InputLayoutDescription d(attributes);

    REQUIRE(attributes[2] == d.getAttributeByName(InputAttribute::SemanticName::Position, 0));
    REQUIRE(attributes[0] == d.getAttributeByName(InputAttribute::SemanticName::Position, 1));
    REQUIRE(attributes[1] == d.getAttributeByName(InputAttribute::SemanticName::Texture, 0));
}
