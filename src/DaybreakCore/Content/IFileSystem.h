#pragma once
#include <string>

namespace Daybreak
{
    /** File system abstraction interface. */
    class IFileSystem
    { 
    public:
        virtual ~IFileSystem() = default;
        virtual std::string loadFileAsText(const std::string& path) = 0;
    };
}