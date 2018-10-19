#pragma once
#include <string>
#include "Utility/TextUtils.h"

namespace Daybreak
{
    /** Consume and return the next token from arguments as a float. Throw an exception if not possible. */
    float readExpectedFloat(Daybreak::TextUtils::StringSplitter& arguments);

    /** Parse the provided token as a float. Throw an exception if not possible. */
    float parseFloat(const std::string_view& token);

    /** Consume and return the next token from arguments as an int. Throw an exception if not possible. */
    int readExpectedInt(Daybreak::TextUtils::StringSplitter& arguments);

    /** Parse the provided token as an int. Throw an exception if not possible. */
    int parseInt(const std::string_view& token);

    /** Consume and return the next token from argument as a string. Throw an exception if not possible. */
    std::string readExpectedString(Daybreak::TextUtils::StringSplitter& arguments);
}