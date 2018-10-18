#pragma once
#include <string>
#include <future>

namespace Daybreak
{
    /** File system abstraction interface. */
    class IFileSystem
    { 
    public:
        virtual ~IFileSystem() = default;
        virtual std::future<std::string> loadFileAsText(const std::string& path) = 0;
    };
}