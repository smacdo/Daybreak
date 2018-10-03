#include "stdafx.h"
#include "CppUnitTest.h"

#include "Utility/TextParser.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Daybreak;

TEST_CLASS(TextParserTests)
{
public:
    TEST_METHOD(Read_Words_No_Text_Provided)
    {
        TextParser p("");
        std::string_view s;

        Assert::IsFalse(p.tryReadNextWord(s));
        Assert::IsTrue(s.empty());
    }

    TEST_METHOD(Read_Words_Only_Whitespace)
    {
        TextParser p("   ");
        std::string_view s;

        Assert::IsFalse(p.tryReadNextWord(s));
        Assert::IsTrue(s.empty());
    }

    // TODO: Test exception thrown.

    TEST_METHOD(Read_Words_Are_Separated_By_Whitespace)
    {
        TextParser p("First  Second\t \t  third\nFourth\r\nFifth");
        std::string_view s;

        Assert::IsTrue(p.tryReadNextWord(s));
        Assert::AreEqual(std::string_view("First"), s);

        Assert::IsTrue(p.tryReadNextWord(s));
        Assert::AreEqual(std::string_view("Second"), s);

        Assert::IsTrue(p.tryReadNextWord(s));
        Assert::AreEqual(std::string_view("third"), s);

        Assert::IsTrue(p.tryReadNextWord(s));
        Assert::AreEqual(std::string_view("Fourth"), s);

        Assert::IsTrue(p.tryReadNextWord(s));
        Assert::AreEqual(std::string_view("Fifth"), s);
    }

    TEST_METHOD(Is_Newline)
    {
        Assert::IsTrue(TextParser::isNewlineChar("\n", 0));
        Assert::IsTrue(TextParser::isNewlineChar("\n\n", 1));
        Assert::IsTrue(TextParser::isNewlineChar("\r\n", 0));
    }
};
