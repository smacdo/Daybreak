#include "stdafx.h"
#include "CppUnitTest.h"

#include "Utility/TextUtils.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Daybreak;

TEST_CLASS(TextUtilsTests)
{
public:
    TEST_METHOD(Read_Lines_From_String_With_LF)
    {
        LineReader reader("apple\norange\nbanana");
        Assert::IsTrue(reader.hasNextLine());
        Assert::AreEqual(std::string_view("apple"), reader.readNextLine());
        Assert::IsTrue(reader.hasNextLine());
        Assert::AreEqual(std::string_view("orange"), reader.readNextLine());
        Assert::IsTrue(reader.hasNextLine());
        Assert::AreEqual(std::string_view("banana"), reader.readNextLine());
        Assert::IsFalse(reader.hasNextLine());
    }

    TEST_METHOD(Read_Lines_From_String_With_CRLF)
    {
        LineReader reader("apple\norange\nbanana");
        Assert::IsTrue(reader.hasNextLine());
        Assert::AreEqual(std::string_view("apple"), reader.readNextLine());
        Assert::IsTrue(reader.hasNextLine());
        Assert::AreEqual(std::string_view("orange"), reader.readNextLine());
        Assert::IsTrue(reader.hasNextLine());
        Assert::AreEqual(std::string_view("banana"), reader.readNextLine());
        Assert::IsFalse(reader.hasNextLine());
    }

    TEST_METHOD(Read_Lines_From_String_With_LF_And_CRLF)
    {
        LineReader reader("hello\nworld!\r\nblah");
        Assert::IsTrue(reader.hasNextLine());
        Assert::AreEqual(std::string_view("hello"), reader.readNextLine());
        Assert::IsTrue(reader.hasNextLine());
        Assert::AreEqual(std::string_view("world!"), reader.readNextLine());
        Assert::IsTrue(reader.hasNextLine());
        Assert::AreEqual(std::string_view("blah"), reader.readNextLine());
        Assert::IsFalse(reader.hasNextLine());
    }

    TEST_METHOD(Read_Lines_With_Empty_Input)
    {
        LineReader reader("");
        Assert::IsFalse(reader.hasNextLine());
        Assert::AreEqual(std::string_view(""), reader.readNextLine());
    }

    TEST_METHOD(Read_Lines_No_Newlines_In_Input)
    {
        LineReader reader("Hello World!");
        Assert::IsTrue(reader.hasNextLine());
        Assert::AreEqual(std::string_view("Hello World!"), reader.readNextLine());
        Assert::IsFalse(reader.hasNextLine());
    }

    TEST_METHOD(Read_Lines_Multiple_Newlines)
    {
        LineReader reader("Hello\r\n\nWorld!");
        Assert::IsTrue(reader.hasNextLine());
        Assert::AreEqual(std::string_view("Hello"), reader.readNextLine());
        Assert::IsTrue(reader.hasNextLine());
        Assert::AreEqual(std::string_view(""), reader.readNextLine());
        Assert::IsTrue(reader.hasNextLine());
        Assert::AreEqual(std::string_view("World!"), reader.readNextLine());
        Assert::IsFalse(reader.hasNextLine());
    }
};
