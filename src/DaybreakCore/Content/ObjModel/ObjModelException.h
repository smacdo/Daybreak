#pragma once
#include "Common/Error.h"
#include <sstream>

namespace Daybreak
{
    /** Exception while reading an obj model file. */
    class ObjModelException : public DaybreakDataException
    {
    public:
        /** Constructor. */
        ObjModelException(
            const std::string& message,
            const std::string& fileName,
            size_t lineNumber,
            const std::string& command,
            const std::string& field)
            : DaybreakDataException(format(message, command, field), fileName, lineNumber),
              m_lineNumber(lineNumber),
              m_command(command),
              m_field(field)
        {
        }

    private:
        static std::string format(
            const std::string& errorMessage,
            const std::string& command,
            const std::string& field)
        {
            return errorMessage + " while reading field '" + field + "' for obj command '" + command + "'";
        }

    private:
        size_t m_lineNumber;
        std::string m_command;
        std::string m_field;
    };
}
