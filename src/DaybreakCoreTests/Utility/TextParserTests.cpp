#include "stdafx.h"
#include "Utility/TextParser.h"

#include "../TestHelpers.h"

using namespace Daybreak;

TEST_CASE("Read_Words_No_Text_Provided", "[core][textutil]")
{
    TextParser p("");
    std::string_view s;

    REQUIRE(!p.tryReadNextWord(s));
    REQUIRE(s.empty());
}

TEST_CASE("Read_Words_Only_Whitespace", "[core][textutil]")
{
    TextParser p("   ");
    std::string_view s;

    REQUIRE(!p.tryReadNextWord(s));
    REQUIRE(s.empty());
}

// TODO: Test exception thrown.

TEST_CASE("Read_Words_Are_Separated_By_Whitespace", "[core][textutil]")
{
    TextParser p("First  Second\t \t  third\nFourth\r\nFifth");
    std::string_view s;

    REQUIRE(p.tryReadNextWord(s));
    REQUIRE(std::string_view("First") == s);

    REQUIRE(p.tryReadNextWord(s));
    REQUIRE(std::string_view("Second") == s);

    REQUIRE(p.tryReadNextWord(s));
    REQUIRE(std::string_view("third") == s);

    REQUIRE(p.tryReadNextWord(s));
    REQUIRE(std::string_view("Fourth") == s);

    REQUIRE(p.tryReadNextWord(s));
    REQUIRE(std::string_view("Fifth") == s);
}

TEST_CASE("Is_Newline", "[core][textutil]")
{
    REQUIRE(TextParser::isNewlineChar("\n", 0));
    REQUIRE(TextParser::isNewlineChar("\n\n", 1));
    REQUIRE(TextParser::isNewlineChar("\r\n", 0));
}
