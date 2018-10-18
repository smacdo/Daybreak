#include "stdafx.h"
#include "DefaultFileSystem.h"

#include <fstream>

using namespace Daybreak;

//---------------------------------------------------------------------------------------------------------------------
DefaultFileSystem::DefaultFileSystem(const std::string& rootDirectory)
    : m_rootDirectory(rootDirectory)
{
}

//---------------------------------------------------------------------------------------------------------------------
DefaultFileSystem::~DefaultFileSystem() = default;

//---------------------------------------------------------------------------------------------------------------------
std::future<std::string> DefaultFileSystem::loadFileAsText(const std::string& path)
{
    auto fullPath = getFullPath(path);

    return std::async(
        std::launch::async | std::launch::deferred,
        [fullPath]() {
            std::ifstream stream;

            stream.exceptions(std::ifstream::failbit | std::ifstream::badbit);  // Enable exceptions on stream.
            stream.open(fullPath);

            return std::string(std::istreambuf_iterator<char>(stream), {});
    });
}

//---------------------------------------------------------------------------------------------------------------------
std::string DefaultFileSystem::getFullPath(const std::string& path)
{
    if (m_rootDirectory.empty())
    {
        return path;
    }
    else
    {
        return m_rootDirectory + "\\" + path;
    }
}
