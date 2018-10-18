#pragma once
#include "IFileSystem.h"

namespace Daybreak
{
    /** File system abstraction layer implemented using standard C++ API. */
    class StandardFileSystem : public IFileSystem
    {
    public:
        StandardFileSystem(const std::string& rootDirectory);
        ~StandardFileSystem();
        virtual std::string loadFileAsText(const std::string& path) override;

    private:
        std::string getFullPath(const std::string& path);

    private:
        std::string m_rootDirectory;
    };
}