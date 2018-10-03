#include "stdafx.h"
#include "CppUnitTest.h"

#include "Utility/TextUtils.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Daybreak::TextUtils;

TEST_CLASS(TextUtilsTests)
{
public:
    TEST_METHOD(Read_Lines_From_String_With_LF)
    {
        StringLineReader reader("apple\norange\nbanana");
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
        StringLineReader reader("apple\norange\nbanana");
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
        StringLineReader reader("hello\nworld!\r\nblah");
        Assert::IsTrue(reader.hasNextLine());
        Assert::AreEqual(std::string_view("hello"), reader.readNextLine());
        Assert::IsTrue(reader.hasNextLine());
        Assert::AreEqual(std::string_view("world!"), reader.readNextLine());
        Assert::IsTrue(reader.hasNextLine());
        Assert::AreEqual(std::string_view("blah"), reader.readNextLine());
        Assert::IsFalse(reader.hasNextLine());
    }

    TEST_METHOD(Read_Lines_From_Wide_String_With_LF_And_CRLF)
    {
        WStringLineReader reader(L"hello\nworld!\r\nblah");
        Assert::IsTrue(reader.hasNextLine());
        Assert::AreEqual(std::wstring_view(L"hello"), reader.readNextLine());
        Assert::IsTrue(reader.hasNextLine());
        Assert::AreEqual(std::wstring_view(L"world!"), reader.readNextLine());
        Assert::IsTrue(reader.hasNextLine());
        Assert::AreEqual(std::wstring_view(L"blah"), reader.readNextLine());
        Assert::IsFalse(reader.hasNextLine());
    }

    TEST_METHOD(Read_Lines_With_Empty_Input)
    {
        StringLineReader reader("");
        Assert::IsFalse(reader.hasNextLine());
        Assert::AreEqual(std::string_view(""), reader.readNextLine());
    }

    TEST_METHOD(Read_Lines_No_Newlines_In_Input)
    {
        StringLineReader reader("Hello World!");
        Assert::IsTrue(reader.hasNextLine());
        Assert::AreEqual(std::string_view("Hello World!"), reader.readNextLine());
        Assert::IsFalse(reader.hasNextLine());
    }

    TEST_METHOD(Read_Lines_Multiple_Newlines)
    {
        StringLineReader reader("Hello\r\n\nWorld!");
        Assert::IsTrue(reader.hasNextLine());
        Assert::AreEqual(std::string_view("Hello"), reader.readNextLine());
        Assert::IsTrue(reader.hasNextLine());
        Assert::AreEqual(std::string_view(""), reader.readNextLine());
        Assert::IsTrue(reader.hasNextLine());
        Assert::AreEqual(std::string_view("World!"), reader.readNextLine());
        Assert::IsFalse(reader.hasNextLine());
    }

    TEST_METHOD(Read_Lines_With_Comment)
    {
        StringLineReader reader("apple # this is a comment", '#');
        Assert::IsTrue(reader.hasNextLine());
        Assert::AreEqual(std::string_view("apple "), reader.readNextLine());;
        Assert::IsFalse(reader.hasNextLine());
    }

    TEST_METHOD(Read_Multiple_Lines_With_Comment)
    {
        StringLineReader reader("apple # this is a comment\nfoo\n#blah\n", '#');
        Assert::IsTrue(reader.hasNextLine());
        Assert::AreEqual(std::string_view("apple "), reader.readNextLine());;
        Assert::IsTrue(reader.hasNextLine());
        Assert::AreEqual(std::string_view("foo"), reader.readNextLine());;
        Assert::IsTrue(reader.hasNextLine());
        Assert::IsTrue(std::string_view("") == reader.readNextLine());;
        Assert::IsFalse(reader.hasNextLine());
    }

    TEST_METHOD(Split_String_With_Comma_Separator)
    {
        StringSplitter reader("one, two, three", ",");
        Assert::IsTrue(reader.hasNextToken());
        Assert::AreEqual(std::string_view("one"), reader.readNextToken());
        Assert::IsTrue(reader.hasNextToken());
        Assert::AreEqual(std::string_view(" two"), reader.readNextToken());
        Assert::IsTrue(reader.hasNextToken());
        Assert::AreEqual(std::string_view(" three"), reader.readNextToken());
        Assert::IsFalse(reader.hasNextToken());
    }

    TEST_METHOD(Split_String_With_Comma_And_Period_Separator)
    {
        StringSplitter reader("one, two. three", ",.");
        Assert::IsTrue(reader.hasNextToken());
        Assert::AreEqual(std::string_view("one"), reader.readNextToken());
        Assert::IsTrue(reader.hasNextToken());
        Assert::AreEqual(std::string_view(" two"), reader.readNextToken());
        Assert::IsTrue(reader.hasNextToken());
        Assert::AreEqual(std::string_view(" three"), reader.readNextToken());
        Assert::IsFalse(reader.hasNextToken());
    }

    TEST_METHOD(Split_String_With_Empty_Tokens)
    {
        StringSplitter reader("one,,. ,two, three", ",.");
        Assert::IsTrue(reader.hasNextToken());
        Assert::AreEqual(std::string_view("one"), reader.readNextToken());
        Assert::IsTrue(reader.hasNextToken());
        Assert::AreEqual(std::string_view(""), reader.readNextToken());
        Assert::IsTrue(reader.hasNextToken());
        Assert::AreEqual(std::string_view(""), reader.readNextToken());
        Assert::IsTrue(reader.hasNextToken());
        Assert::AreEqual(std::string_view(" "), reader.readNextToken());
        Assert::IsTrue(reader.hasNextToken());
        Assert::AreEqual(std::string_view("two"), reader.readNextToken());
        Assert::IsTrue(reader.hasNextToken());
        Assert::AreEqual(std::string_view(" three"), reader.readNextToken());
        Assert::IsFalse(reader.hasNextToken());
    }

    TEST_METHOD(Split_String_Can_Skip_Empty_Tokens)
    {
        StringSplitter reader("one,,. ,two, three", ",.", true);
        Assert::IsTrue(reader.skipEmptyTokens());

        Assert::AreEqual(std::string_view("one"), reader.readNextToken());
        Assert::IsTrue(reader.hasNextToken());

        Assert::AreEqual(std::string_view(" "), reader.readNextToken());
        Assert::IsTrue(reader.hasNextToken());

        Assert::AreEqual(std::string_view("two"), reader.readNextToken());
        Assert::IsTrue(reader.hasNextToken());

        Assert::AreEqual(std::string_view(" three"), reader.readNextToken());
        Assert::IsFalse(reader.hasNextToken());
    }

    TEST_METHOD(Split_String_Can_Skip_Empty_Tokens_At_End)
    {
        StringSplitter reader("one,,", ",", true);
        Assert::IsTrue(reader.skipEmptyTokens());

        Assert::AreEqual(std::string_view("one"), reader.readNextToken());
        Assert::IsTrue(reader.hasNextToken());

        Assert::IsTrue(std::string_view() == reader.readNextToken());
        Assert::IsFalse(reader.hasNextToken());
    }

    TEST_METHOD(Split_Empty_String)
    {
        StringSplitter reader("", ",");
        Assert::IsFalse(reader.hasNextToken());
        Assert::IsTrue(std::string_view() == reader.readNextToken());
    }

    TEST_METHOD(Split_String_With_No_Separators)
    {
        StringSplitter reader("Hello World!", ",");
        Assert::IsTrue(reader.hasNextToken());
        Assert::AreEqual(std::string_view("Hello World!"), reader.readNextToken());
        Assert::IsFalse(reader.hasNextToken());
    }

    TEST_METHOD(Split_Wide_String_With_Comma_Separator)
    {
        WStringSplitter reader(L"one, two, three", L",");
        Assert::IsTrue(reader.hasNextToken());
        Assert::AreEqual(std::wstring_view(L"one"), reader.readNextToken());
        Assert::IsTrue(reader.hasNextToken());
        Assert::AreEqual(std::wstring_view(L" two"), reader.readNextToken());
        Assert::IsTrue(reader.hasNextToken());
        Assert::AreEqual(std::wstring_view(L" three"), reader.readNextToken());
        Assert::IsFalse(reader.hasNextToken());
    }

    TEST_METHOD(Read_Remainng_Unsplit_Text)
    {
        WStringSplitter reader(L"one, two, three", L",");

        Assert::AreEqual(std::wstring_view(L"one"), reader.readNextToken());
        Assert::AreEqual(std::wstring_view(L" two, three"), reader.readRemaining());
        Assert::IsFalse(reader.hasNextToken());
    }

    TEST_METHOD(Left_Trim_String)
    {
        Assert::AreEqual(std::string_view("Hello"), leftTrim<char, std::char_traits<char>>("Hello"));
        Assert::AreEqual(std::string_view("Hello"), leftTrim<char, std::char_traits<char>>(" Hello"));
        Assert::AreEqual(std::string_view("Hello"), leftTrim<char, std::char_traits<char>>("  Hello"));
        Assert::AreEqual(std::string_view("Hello"), leftTrim<char, std::char_traits<char>>(" \t\n Hello"));

        Assert::AreEqual(std::string_view("World\t "), leftTrim<char, std::char_traits<char>>(" World\t "));
        Assert::AreEqual(std::string_view("Wo rld "), leftTrim<char, std::char_traits<char>>(" \rWo rld "));
    }

    TEST_METHOD(Left_Trim_String_Containing_Only_Whitespace)
    {
        Assert::IsTrue(std::string_view() == leftTrim<char, std::char_traits<char>>(""));
        Assert::IsTrue(std::string_view() == leftTrim<char, std::char_traits<char>>(" "));
        Assert::IsTrue(std::string_view() == leftTrim<char, std::char_traits<char>>("   "));
        Assert::IsTrue(std::string_view() == leftTrim<char, std::char_traits<char>>(" \t \r  "));
    }

    TEST_METHOD(Left_Trim_Wide_String)
    {
        Assert::AreEqual(std::wstring_view(L"Hello"), leftTrim<wchar_t, std::char_traits<wchar_t>>(L"Hello"));
        Assert::AreEqual(std::wstring_view(L"Hello"), leftTrim<wchar_t, std::char_traits<wchar_t>>(L" Hello"));
        Assert::AreEqual(std::wstring_view(L"Hello"), leftTrim<wchar_t, std::char_traits<wchar_t>>(L"  Hello"));
        Assert::AreEqual(std::wstring_view(L"Hello"), leftTrim<wchar_t, std::char_traits<wchar_t>>(L" \t\n Hello"));

        Assert::AreEqual(std::wstring_view(L"World\t "), leftTrim<wchar_t, std::char_traits<wchar_t>>(L" World\t "));
        Assert::AreEqual(std::wstring_view(L"Wo rld "), leftTrim<wchar_t, std::char_traits<wchar_t>>(L" \rWo rld "));
    }

    TEST_METHOD(Left_Trim_Wide_String_Containing_Only_Whitespace)
    {
        Assert::IsTrue(std::wstring_view() == leftTrim<wchar_t, std::char_traits<wchar_t>>(L""));
        Assert::IsTrue(std::wstring_view() == leftTrim<wchar_t, std::char_traits<wchar_t>>(L" "));
        Assert::IsTrue(std::wstring_view() == leftTrim<wchar_t, std::char_traits<wchar_t>>(L"   "));
        Assert::IsTrue(std::wstring_view() == leftTrim<wchar_t, std::char_traits<wchar_t>>(L" \t \r  "));
    }

    TEST_METHOD(Right_Trim_String)
    {
        Assert::AreEqual(std::string_view("Hello"), rightTrim<char, std::char_traits<char>>("Hello"));
        Assert::AreEqual(std::string_view("Hello"), rightTrim<char, std::char_traits<char>>("Hello "));
        Assert::AreEqual(std::string_view("Hello"), rightTrim<char, std::char_traits<char>>("Hello  "));
        Assert::AreEqual(std::string_view("Hello"), rightTrim<char, std::char_traits<char>>("Hello \t\n "));

        Assert::AreEqual(std::string_view("\t World"), rightTrim<char, std::char_traits<char>>("\t World"));
        Assert::AreEqual(std::string_view(" Wo rld"), rightTrim<char, std::char_traits<char>>(" Wo rld \r"));
    }

    TEST_METHOD(Right_Trim_String_Containing_Only_Whitespace)
    {
        Assert::IsTrue(std::string_view() == rightTrim<char, std::char_traits<char>>(""));
        Assert::IsTrue(std::string_view() == rightTrim<char, std::char_traits<char>>(" "));
        Assert::IsTrue(std::string_view() == rightTrim<char, std::char_traits<char>>("   "));
        Assert::IsTrue(std::string_view() == rightTrim<char, std::char_traits<char>>(" \t \r  "));
    }

    TEST_METHOD(Right_Trim_Wide_String)
    {
        Assert::AreEqual(std::wstring_view(L"Hello"), rightTrim<wchar_t, std::char_traits<wchar_t>>(L"Hello"));
        Assert::AreEqual(std::wstring_view(L"Hello"), rightTrim<wchar_t, std::char_traits<wchar_t>>(L"Hello "));
        Assert::AreEqual(std::wstring_view(L"Hello"), rightTrim<wchar_t, std::char_traits<wchar_t>>(L"Hello  "));
        Assert::AreEqual(std::wstring_view(L"Hello"), rightTrim<wchar_t, std::char_traits<wchar_t>>(L"Hello \t\n "));

        Assert::AreEqual(std::wstring_view(L"\t World"), rightTrim<wchar_t, std::char_traits<wchar_t>>(L"\t World"));
        Assert::AreEqual(std::wstring_view(L" Wo rld"), rightTrim<wchar_t, std::char_traits<wchar_t>>(L" Wo rld \r"));
    }

    TEST_METHOD(Right_Trim_Wide_String_Containing_Only_Whitespace)
    {
        Assert::IsTrue(std::wstring_view() == rightTrim<wchar_t, std::char_traits<wchar_t>>(L""));
        Assert::IsTrue(std::wstring_view() == rightTrim<wchar_t, std::char_traits<wchar_t>>(L" "));
        Assert::IsTrue(std::wstring_view() == rightTrim<wchar_t, std::char_traits<wchar_t>>(L"   "));
        Assert::IsTrue(std::wstring_view() == rightTrim<wchar_t, std::char_traits<wchar_t>>(L" \t \r  "));
    }

    TEST_METHOD(Trim_String)
    {
        Assert::AreEqual(std::string_view("Hello"), trim<char, std::char_traits<char>>("Hello"));
        Assert::AreEqual(std::string_view("Hello"), trim<char, std::char_traits<char>>("Hello "));
        Assert::AreEqual(std::string_view("Hello"), trim<char, std::char_traits<char>>("Hello  "));
        Assert::AreEqual(std::string_view("Hello"), trim<char, std::char_traits<char>>("Hello \t\n "));

        Assert::AreEqual(std::string_view("World"), trim<char, std::char_traits<char>>("World"));
        Assert::AreEqual(std::string_view("World"), trim<char, std::char_traits<char>>(" World"));
        Assert::AreEqual(std::string_view("World"), trim<char, std::char_traits<char>>(" World "));
        Assert::AreEqual(std::string_view("x"), trim<char, std::char_traits<char>>("\t \n\r x \r\r \n"));
        Assert::AreEqual(std::string_view("a b"), trim<char, std::char_traits<char>>("a b"));
    }

    TEST_METHOD(Trim_String_Containing_Only_Whitespace)
    {
        Assert::IsTrue(std::string_view() == trim<char, std::char_traits<char>>(""));
        Assert::IsTrue(std::string_view() == trim<char, std::char_traits<char>>(" "));
        Assert::IsTrue(std::string_view() == trim<char, std::char_traits<char>>("   "));
        Assert::IsTrue(std::string_view() == trim<char, std::char_traits<char>>(" \t \r  "));
    }
};
