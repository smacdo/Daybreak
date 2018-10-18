#include "stdafx.h"
#include "StandardFileSystem.h"

#include <fstream>

using namespace Daybreak;

//---------------------------------------------------------------------------------------------------------------------
StandardFileSystem::StandardFileSystem(const std::string& rootDirectory)
    : m_rootDirectory(rootDirectory)
{
}

//---------------------------------------------------------------------------------------------------------------------
StandardFileSystem::~StandardFileSystem() = default;

//---------------------------------------------------------------------------------------------------------------------
std::string StandardFileSystem::loadFileAsText(const std::string& path)
{
    std::ifstream stream;

    stream.exceptions(std::ifstream::failbit | std::ifstream::badbit);  // Enable exceptions on stream.
    stream.open(getFullPath(path));

    return std::string(std::istreambuf_iterator<char>(stream), {});
}

//---------------------------------------------------------------------------------------------------------------------
std::string StandardFileSystem::getFullPath(const std::string& path)
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
