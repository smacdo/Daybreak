#pragma once
#include "IFileSystem.h"

namespace Daybreak
{
    /** File system abstraction layer implemented using standard C++ API. */
    class DefaultFileSystem : public IFileSystem
    {
    public:
        DefaultFileSystem(const std::string& rootDirectory);
        ~DefaultFileSystem();
        virtual std::future<std::string> loadFileAsText(const std::string& path) override;

    private:
        std::string getFullPath(const std::string& path);

    private:
        std::string m_rootDirectory;
    };
}