#include "stdafx.h"
#include "ResourcesManager.h"
#include "Content/IFileSystem.h"
#include "Common/Error.h"

#include "Content/ObjModel/ObjResourceLoader.h"
#include "Graphics/ModelData.h"

using namespace Daybreak;

//---------------------------------------------------------------------------------------------------------------------
ResourcesManager::ResourcesManager(std::shared_ptr<IFileSystem> fileSystem)
    : m_fileSystem(fileSystem)
{
    CHECK_NOT_NULL(fileSystem);
}

//---------------------------------------------------------------------------------------------------------------------
std::unique_ptr<ModelData> ResourcesManager::loadModel(const std::string& path)
{
    ObjResourceLoader l;
    return l.load(path, *this);
}

//---------------------------------------------------------------------------------------------------------------------
std::string ResourcesManager::loadTextFile(const std::string& path)
{
    return m_fileSystem->loadFileAsText(path);
}
