#include "stdafx.h"
#include "CppUnitTest.h"

#include "Graphics/InputLayoutDescription.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Daybreak;

TEST_CLASS(InputLayoutDescriptionTests)
{
public:
    TEST_METHOD(Input_Layout_Description_Default_Constructor_Creates_Empty_Description)
    {
        InputLayoutDescription d;
        Assert::AreEqual(0, (int)d.attributeCount());
    }

    TEST_METHOD(Input_Layout_Constructor_Takes_Attribute_Array)
    {
        auto attributes = std::vector<InputAttribute>
        {
            { InputAttribute::SemanticName::Position, 0, InputAttribute::StorageType::Float, 3 },
            { InputAttribute::SemanticName::Texture, 0, InputAttribute::StorageType::Float, 2 },
            { InputAttribute::SemanticName::Normal, 0, InputAttribute::StorageType::Float, 3 }
        };

        InputLayoutDescription d(attributes);
        Assert::AreEqual(3, (int)d.attributeCount());

        Assert::IsTrue(attributes[0] == d.getAttributeByIndex(0));
        Assert::IsTrue(attributes[1] == d.getAttributeByIndex(1));
        Assert::IsTrue(attributes[2] == d.getAttributeByIndex(2));
    }

    TEST_METHOD(Input_Layout_Keeps_Count_Of_Attributes_Added)
    {
        InputLayoutDescription d;
        Assert::AreEqual(0, (int)d.attributeCount());

        d.addAttribute(InputAttribute::SemanticName::None, 0, InputAttribute::StorageType::Double, 2);
        Assert::AreEqual(1, (int)d.attributeCount());

        d.addAttribute(InputAttribute::SemanticName::None, 0, InputAttribute::StorageType::HalfFloat, 3);
        Assert::AreEqual(2, (int)d.attributeCount());

        d.addAttribute(InputAttribute::SemanticName::None, 0, InputAttribute::StorageType::UnsignedInt, 2);
        Assert::AreEqual(3, (int)d.attributeCount());
    }

    TEST_METHOD(Input_Layout_Calculates_Size_Of_Element_Using_Attributes)
    {
        InputLayoutDescription d;
        Assert::AreEqual(0, (int)d.elementSizeInBytes());

        d.addAttribute(InputAttribute::SemanticName::None, 0, InputAttribute::StorageType::Float, 2);
        Assert::AreEqual(8, (int)d.elementSizeInBytes());

        d.addAttribute(InputAttribute::SemanticName::None, 0, InputAttribute::StorageType::Short, 1);
        Assert::AreEqual(10, (int)d.elementSizeInBytes());

        d.addAttribute(InputAttribute::SemanticName::None, 0, InputAttribute::StorageType::UnsignedByte, 3);
        Assert::AreEqual(13, (int)d.elementSizeInBytes());
    }

    TEST_METHOD(Input_Layout_Calculates_Offset_To_Each_Element)
    {
        InputLayoutDescription d(std::vector<InputAttribute>
        {
            { InputAttribute::SemanticName::Position, 0, InputAttribute::StorageType::Short, 3 },
            { InputAttribute::SemanticName::Texture, 0, InputAttribute::StorageType::UnsignedByte, 3 },
            { InputAttribute::SemanticName::Normal, 0, InputAttribute::StorageType::Float, 3 }
        });

        Assert::AreEqual(0, (int)d.attributeOffsetByIndex(0));
        Assert::AreEqual(6, (int)d.attributeOffsetByIndex(1));
        Assert::AreEqual(9, (int)d.attributeOffsetByIndex(2));
    }
};
