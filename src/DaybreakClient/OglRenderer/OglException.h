#pragma once
#include "Common/Error.h"

namespace Daybreak::OpenGlRenderer
{
    // Error when interacting with GLAD API.
    class GladException : public std::runtime_error
    {
    public:
        GladException(const std::string& message);
    };

    // Error when interacting with OpenGL.
    class GlException : public std::runtime_error       // TODO: Derive from another base
    {
    public:
        // Constructor.
        GlException(
            unsigned int error,                 // OpenGL error enum value.
            const char* filename = nullptr,     // Optional name of source code file where error caught.
            int line = -1);                     // Optional line in source code file where error caught.

                                                // Get the error code.
        unsigned int ErrorCode() const { return m_error; }

        // Get a human readable name for the error that was generated.
        const char * ErrorName() const;

        // Get the filename where the error was caught (Optional, might be null).
        const char * FileName() const { return m_filename; }

        // Get the line where the error was caught (Optional, might be -1).
        int Line() const { return m_line; }

        // Get a human readable name for an OpenGL error code.
        static const char * GetErrorName(unsigned int error);

    private:
        unsigned int m_error;
        const char * m_filename;
        int m_line;
    };
}
