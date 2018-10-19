#include "stdafx.h"
#include "TextParsingUtils.h"

#include <charconv>

using namespace Daybreak;

//---------------------------------------------------------------------------------------------------------------------
float Daybreak::readExpectedFloat(Daybreak::TextUtils::StringSplitter& arguments)
{
    if (arguments.hasNextToken())
    {
        return parseFloat(arguments.readNextToken());
    }
    else
    {
        throw std::runtime_error("Expected next token to be a float but no token was found");
    }
}

//---------------------------------------------------------------------------------------------------------------------
float Daybreak::parseFloat(const std::string_view& token)
{
    float value = 0.0f;
    auto result = std::from_chars(
        token.data(),
        token.data() + token.size(),
        value);

    if (result.ec == std::errc::invalid_argument || result.ec == std::errc::result_out_of_range)
    {
        throw std::runtime_error("Could not parse string as float");
    }

    return value;
}

//---------------------------------------------------------------------------------------------------------------------
int Daybreak::readExpectedInt(Daybreak::TextUtils::StringSplitter& arguments)
{
    if (arguments.hasNextToken())
    {
        return parseInt(arguments.readNextToken());
    }
    else
    {
        throw std::runtime_error("Expected next token to be an int but no token was found");
    }
}

//---------------------------------------------------------------------------------------------------------------------
int Daybreak::parseInt(const std::string_view& token)
{
    int value = 0;
    auto result = std::from_chars(
        token.data(),
        token.data() + token.size(),
        value);

    if (result.ec == std::errc::invalid_argument || result.ec == std::errc::result_out_of_range)
    {
        throw std::runtime_error("Could not parse string as int");
    }

    return value;
}

//---------------------------------------------------------------------------------------------------------------------
std::string Daybreak::readExpectedString(Daybreak::TextUtils::StringSplitter& arguments)
{
    if (arguments.hasNextToken())
    {
        auto view = arguments.readNextToken();
        return std::string(view.data(), view.size());
    }
    else
    {
        throw std::runtime_error("Expected next token to be a string but no token was found");
    }
}
