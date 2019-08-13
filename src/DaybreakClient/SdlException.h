#pragma once

#include <string>
#include <stdexcept>

/** Error when interacting with SDL API. */
class SdlException : public std::runtime_error
{
public:
    SdlException(const std::string& message);

    std::string Message() const { return m_message; }
    static std::string GetLastSdlError();

private:
    std::string m_message;
};

