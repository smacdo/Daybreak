#include "stdafx.h"
#include "Utility/TextUtils.h"

#undef CHECK
#include "catch2\catch.hpp"

using namespace Daybreak::TextUtils;

TEST_CASE("Read_Lines_From_String_With_LF", "[core][textutil]")
{
    StringLineReader reader("apple\norange\nbanana");
    REQUIRE(reader.hasNextLine());
    REQUIRE(std::string_view("apple") == reader.readNextLine());
    REQUIRE(reader.hasNextLine());
    REQUIRE(std::string_view("orange") == reader.readNextLine());
    REQUIRE(reader.hasNextLine());
    REQUIRE(std::string_view("banana") == reader.readNextLine());
    REQUIRE(!reader.hasNextLine());
}

TEST_CASE("Read_Lines_From_String_With_CRLF", "[core][textutil]")
{
    StringLineReader reader("apple\norange\nbanana");
    REQUIRE(reader.hasNextLine());
    REQUIRE(std::string_view("apple") == reader.readNextLine());
    REQUIRE(reader.hasNextLine());
    REQUIRE(std::string_view("orange") == reader.readNextLine());
    REQUIRE(reader.hasNextLine());
    REQUIRE(std::string_view("banana") == reader.readNextLine());
    REQUIRE(!reader.hasNextLine());
}

TEST_CASE("Read_Lines_From_String_With_LF_And_CRLF", "[core][textutil]")
{
    StringLineReader reader("hello\nworld!\r\nblah");
    REQUIRE(reader.hasNextLine());
    REQUIRE(std::string_view("hello") == reader.readNextLine());
    REQUIRE(reader.hasNextLine());
    REQUIRE(std::string_view("world!") == reader.readNextLine());
    REQUIRE(reader.hasNextLine());
    REQUIRE(std::string_view("blah") == reader.readNextLine());
    REQUIRE(!reader.hasNextLine());
}

TEST_CASE("Read_Lines_From_Wide_String_With_LF_And_CRLF", "[core][textutil]")
{
    WStringLineReader reader(L"hello\nworld!\r\nblah");
    REQUIRE(reader.hasNextLine());
    REQUIRE(std::wstring_view(L"hello") == reader.readNextLine());
    REQUIRE(reader.hasNextLine());
    REQUIRE(std::wstring_view(L"world!") == reader.readNextLine());
    REQUIRE(reader.hasNextLine());
    REQUIRE(std::wstring_view(L"blah") == reader.readNextLine());
    REQUIRE(!reader.hasNextLine());
}

TEST_CASE("Read_Lines_With_Empty_Input", "[core][textutil]")
{
    StringLineReader reader("");
    REQUIRE(!reader.hasNextLine());
    REQUIRE(std::string_view("") == reader.readNextLine());
}

TEST_CASE("Read_Lines_No_Newlines_In_Input", "[core][textutil]")
{
    StringLineReader reader("Hello World!");
    REQUIRE(reader.hasNextLine());
    REQUIRE(std::string_view("Hello World!") == reader.readNextLine());
    REQUIRE(!reader.hasNextLine());
}

TEST_CASE("Read_Lines_Multiple_Newlines", "[core][textutil]")
{
    StringLineReader reader("Hello\r\n\nWorld!");
    REQUIRE(reader.hasNextLine());
    REQUIRE(std::string_view("Hello") == reader.readNextLine());
    REQUIRE(reader.hasNextLine());
    REQUIRE(std::string_view("") == reader.readNextLine());
    REQUIRE(reader.hasNextLine());
    REQUIRE(std::string_view("World!") == reader.readNextLine());
    REQUIRE(!reader.hasNextLine());
}

TEST_CASE("Read_Lines_With_Comment", "[core][textutil]")
{
    StringLineReader reader("apple # this is a comment", '#');
    REQUIRE(reader.hasNextLine());
    REQUIRE(std::string_view("apple ") == reader.readNextLine());;
    REQUIRE(!reader.hasNextLine());
}

TEST_CASE("Read_Multiple_Lines_With_Comment", "[core][textutil]")
{
    StringLineReader reader("apple # this is a comment\nfoo\n#blah\n", '#');
    REQUIRE(reader.hasNextLine());
    REQUIRE(std::string_view("apple ") == reader.readNextLine());;
    REQUIRE(reader.hasNextLine());
    REQUIRE(std::string_view("foo") == reader.readNextLine());;
    REQUIRE(reader.hasNextLine());
    REQUIRE(std::string_view("") == reader.readNextLine());;
    REQUIRE(!reader.hasNextLine());
}

TEST_CASE("Split_String_With_Comma_Separator", "[core][textutil]")
{
    StringSplitter reader("one, two, three", ",");
    REQUIRE(reader.hasNextToken());
    REQUIRE(std::string_view("one") == reader.readNextToken());
    REQUIRE(reader.hasNextToken());
    REQUIRE(std::string_view(" two") == reader.readNextToken());
    REQUIRE(reader.hasNextToken());
    REQUIRE(std::string_view(" three") == reader.readNextToken());
    REQUIRE(!reader.hasNextToken());
}

TEST_CASE("Split_String_With_Comma_And_Period_Separator", "[core][textutil]")
{
    StringSplitter reader("one, two. three", ",.");
    REQUIRE(reader.hasNextToken());
    REQUIRE(std::string_view("one") == reader.readNextToken());
    REQUIRE(reader.hasNextToken());
    REQUIRE(std::string_view(" two") == reader.readNextToken());
    REQUIRE(reader.hasNextToken());
    REQUIRE(std::string_view(" three") == reader.readNextToken());
    REQUIRE(!reader.hasNextToken());
}

TEST_CASE("Split_String_With_Empty_Tokens", "[core][textutil]")
{
    StringSplitter reader("one,,. ,two, three", ",.");
    REQUIRE(reader.hasNextToken());
    REQUIRE(std::string_view("one") == reader.readNextToken());
    REQUIRE(reader.hasNextToken());
    REQUIRE(std::string_view("") == reader.readNextToken());
    REQUIRE(reader.hasNextToken());
    REQUIRE(std::string_view("") == reader.readNextToken());
    REQUIRE(reader.hasNextToken());
    REQUIRE(std::string_view(" ") == reader.readNextToken());
    REQUIRE(reader.hasNextToken());
    REQUIRE(std::string_view("two") == reader.readNextToken());
    REQUIRE(reader.hasNextToken());
    REQUIRE(std::string_view(" three") == reader.readNextToken());
    REQUIRE(!reader.hasNextToken());
}

TEST_CASE("Split_String_Can_Skip_Empty_Tokens", "[core][textutil]")
{
    StringSplitter reader("one,,. ,two, three", ",.", true);
    REQUIRE(reader.skipEmptyTokens());

    REQUIRE(std::string_view("one") == reader.readNextToken());
    REQUIRE(reader.hasNextToken());

    REQUIRE(std::string_view(" ") == reader.readNextToken());
    REQUIRE(reader.hasNextToken());

    REQUIRE(std::string_view("two") == reader.readNextToken());
    REQUIRE(reader.hasNextToken());

    REQUIRE(std::string_view(" three") == reader.readNextToken());
    REQUIRE(!reader.hasNextToken());
}

TEST_CASE("Split_String_Has_Next_False_If_Skip_Empty_And_Only_Empty_Left", "[core][textutil]")
{
    StringSplitter reader("   ", " ", true);
    REQUIRE(!reader.hasNextToken());
}

TEST_CASE("Split_String_Can_Skip_Empty_Tokens_At_End", "[core][textutil]")
{
    StringSplitter reader("one,,", ",", true);
    REQUIRE(reader.skipEmptyTokens());

    REQUIRE(std::string_view("one") == reader.readNextToken());
    REQUIRE(!reader.hasNextToken());
}

TEST_CASE("Split_Empty_String_Throws_Exception", "[core][textutil]")
{
    StringSplitter reader("", ",");
    REQUIRE(!reader.hasNextToken());

    REQUIRE_THROWS_WITH(reader.readNextToken(), Catch::Contains("No more tokens to read"));
}

TEST_CASE("Split_String_With_No_Separators", "[core][textutil]")
{
    StringSplitter reader("Hello World!", ",");
    REQUIRE(reader.hasNextToken());
    REQUIRE(std::string_view("Hello World!") == reader.readNextToken());
    REQUIRE(!reader.hasNextToken());
}

TEST_CASE("Split_Wide_String_With_Comma_Separator", "[core][textutil]")
{
    WStringSplitter reader(L"one, two, three", L",");
    REQUIRE(reader.hasNextToken());
    REQUIRE(std::wstring_view(L"one") == reader.readNextToken());
    REQUIRE(reader.hasNextToken());
    REQUIRE(std::wstring_view(L" two") == reader.readNextToken());
    REQUIRE(reader.hasNextToken());
    REQUIRE(std::wstring_view(L" three") == reader.readNextToken());
    REQUIRE(!reader.hasNextToken());
}

TEST_CASE("Read_Remainng_Unsplit_Text", "[core][textutil]")
{
    WStringSplitter reader(L"one, two, three", L",");

    REQUIRE(std::wstring_view(L"one") == reader.readNextToken());
    REQUIRE(std::wstring_view(L" two, three") == reader.readRemaining());
    REQUIRE(!reader.hasNextToken());
}

TEST_CASE("Left_Trim_String", "[core][textutil]")
{
    REQUIRE(std::string_view("Hello") == leftTrim<char, std::char_traits<char>>("Hello"));
    REQUIRE(std::string_view("Hello") == leftTrim<char, std::char_traits<char>>(" Hello"));
    REQUIRE(std::string_view("Hello") == leftTrim<char, std::char_traits<char>>("  Hello"));
    REQUIRE(std::string_view("Hello") == leftTrim<char, std::char_traits<char>>(" \t\n Hello"));

    REQUIRE(std::string_view("World\t ") == leftTrim<char, std::char_traits<char>>(" World\t "));
    REQUIRE(std::string_view("Wo rld ") == leftTrim<char, std::char_traits<char>>(" \rWo rld "));
}

TEST_CASE("Left_Trim_String_Containing_Only_Whitespace", "[core][textutil]")
{
    REQUIRE(std::string_view() == leftTrim<char, std::char_traits<char>>(""));
    REQUIRE(std::string_view() == leftTrim<char, std::char_traits<char>>(" "));
    REQUIRE(std::string_view() == leftTrim<char, std::char_traits<char>>("   "));
    REQUIRE(std::string_view() == leftTrim<char, std::char_traits<char>>(" \t \r  "));
}

TEST_CASE("Left_Trim_Wide_String", "[core][textutil]")
{
    REQUIRE(std::wstring_view(L"Hello") == leftTrim<wchar_t, std::char_traits<wchar_t>>(L"Hello"));
    REQUIRE(std::wstring_view(L"Hello") == leftTrim<wchar_t, std::char_traits<wchar_t>>(L" Hello"));
    REQUIRE(std::wstring_view(L"Hello") == leftTrim<wchar_t, std::char_traits<wchar_t>>(L"  Hello"));
    REQUIRE(std::wstring_view(L"Hello") == leftTrim<wchar_t, std::char_traits<wchar_t>>(L" \t\n Hello"));

    REQUIRE(std::wstring_view(L"World\t ") == leftTrim<wchar_t, std::char_traits<wchar_t>>(L" World\t "));
    REQUIRE(std::wstring_view(L"Wo rld ") == leftTrim<wchar_t, std::char_traits<wchar_t>>(L" \rWo rld "));
}

TEST_CASE("Left_Trim_Wide_String_Containing_Only_Whitespace", "[core][textutil]")
{
    REQUIRE(std::wstring_view() == leftTrim<wchar_t, std::char_traits<wchar_t>>(L""));
    REQUIRE(std::wstring_view() == leftTrim<wchar_t, std::char_traits<wchar_t>>(L" "));
    REQUIRE(std::wstring_view() == leftTrim<wchar_t, std::char_traits<wchar_t>>(L"   "));
    REQUIRE(std::wstring_view() == leftTrim<wchar_t, std::char_traits<wchar_t>>(L" \t \r  "));
}

TEST_CASE("Right_Trim_String", "[core][textutil]")
{
    REQUIRE(std::string_view("Hello") == rightTrim<char, std::char_traits<char>>("Hello"));
    REQUIRE(std::string_view("Hello") == rightTrim<char, std::char_traits<char>>("Hello "));
    REQUIRE(std::string_view("Hello") == rightTrim<char, std::char_traits<char>>("Hello  "));
    REQUIRE(std::string_view("Hello") == rightTrim<char, std::char_traits<char>>("Hello \t\n "));

    REQUIRE(std::string_view("\t World") == rightTrim<char, std::char_traits<char>>("\t World"));
    REQUIRE(std::string_view(" Wo rld") == rightTrim<char, std::char_traits<char>>(" Wo rld \r"));
}

TEST_CASE("Right_Trim_String_Containing_Only_Whitespace", "[core][textutil]")
{
    REQUIRE(std::string_view() == rightTrim<char, std::char_traits<char>>(""));
    REQUIRE(std::string_view() == rightTrim<char, std::char_traits<char>>(" "));
    REQUIRE(std::string_view() == rightTrim<char, std::char_traits<char>>("   "));
    REQUIRE(std::string_view() == rightTrim<char, std::char_traits<char>>(" \t \r  "));
}

TEST_CASE("Right_Trim_Wide_String", "[core][textutil]")
{
    REQUIRE(std::wstring_view(L"Hello") == rightTrim<wchar_t, std::char_traits<wchar_t>>(L"Hello"));
    REQUIRE(std::wstring_view(L"Hello") == rightTrim<wchar_t, std::char_traits<wchar_t>>(L"Hello "));
    REQUIRE(std::wstring_view(L"Hello") == rightTrim<wchar_t, std::char_traits<wchar_t>>(L"Hello  "));
    REQUIRE(std::wstring_view(L"Hello") == rightTrim<wchar_t, std::char_traits<wchar_t>>(L"Hello \t\n "));

    REQUIRE(std::wstring_view(L"\t World") == rightTrim<wchar_t, std::char_traits<wchar_t>>(L"\t World"));
    REQUIRE(std::wstring_view(L" Wo rld") == rightTrim<wchar_t, std::char_traits<wchar_t>>(L" Wo rld \r"));
}

TEST_CASE("Right_Trim_Wide_String_Containing_Only_Whitespace", "[core][textutil]")
{
    REQUIRE(std::wstring_view() == rightTrim<wchar_t, std::char_traits<wchar_t>>(L""));
    REQUIRE(std::wstring_view() == rightTrim<wchar_t, std::char_traits<wchar_t>>(L" "));
    REQUIRE(std::wstring_view() == rightTrim<wchar_t, std::char_traits<wchar_t>>(L"   "));
    REQUIRE(std::wstring_view() == rightTrim<wchar_t, std::char_traits<wchar_t>>(L" \t \r  "));
}

TEST_CASE("Trim_String", "[core][textutil]")
{
    REQUIRE(std::string_view("Hello") == trim<char, std::char_traits<char>>("Hello"));
    REQUIRE(std::string_view("Hello") == trim<char, std::char_traits<char>>("Hello "));
    REQUIRE(std::string_view("Hello") == trim<char, std::char_traits<char>>("Hello  "));
    REQUIRE(std::string_view("Hello") == trim<char, std::char_traits<char>>("Hello \t\n "));

    REQUIRE(std::string_view("World") == trim<char, std::char_traits<char>>("World"));
    REQUIRE(std::string_view("World") == trim<char, std::char_traits<char>>(" World"));
    REQUIRE(std::string_view("World") == trim<char, std::char_traits<char>>(" World "));
    REQUIRE(std::string_view("x") == trim<char, std::char_traits<char>>("\t \n\r x \r\r \n"));
    REQUIRE(std::string_view("a b") == trim<char, std::char_traits<char>>("a b"));
}

TEST_CASE("Trim_String_Containing_Only_Whitespace", "[core][textutil]")
{
    REQUIRE(std::string_view() == trim<char, std::char_traits<char>>(""));
    REQUIRE(std::string_view() == trim<char, std::char_traits<char>>(" "));
    REQUIRE(std::string_view() == trim<char, std::char_traits<char>>("   "));
    REQUIRE(std::string_view() == trim<char, std::char_traits<char>>(" \t \r  "));
}
