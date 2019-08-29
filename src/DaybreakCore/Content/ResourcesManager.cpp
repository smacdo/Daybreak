#include "stdafx.h"
#include "ResourcesManager.h"
#include "Content/IFileSystem.h"
#include "Common/Error.h"

#include "Content/ObjModel/ObjResourceLoader.h"
#include "Content\Models\ModelData.h"

#include "Content/Images/ImageResourceLoader.h"
#include "Graphics/Image.h"

using namespace Daybreak;

//---------------------------------------------------------------------------------------------------------------------
ResourcesManager::ResourcesManager(std::shared_ptr<IFileSystem> fileSystem)
    : m_fileSystem(fileSystem)
{
    CHECK_NOT_NULL(fileSystem);
}

//---------------------------------------------------------------------------------------------------------------------
std::future<std::unique_ptr<ModelData>> ResourcesManager::loadModel(const std::string& path)
{
    ObjResourceLoader l;
    return l.load(path, *this);
}

//---------------------------------------------------------------------------------------------------------------------
std::future<std::unique_ptr<Image>> ResourcesManager::loadImage(const std::string& path)
{
    ImageResourceLoader l;
    return l.load(path, *this);
}

//---------------------------------------------------------------------------------------------------------------------
std::future<std::string> ResourcesManager::loadTextFile(const std::string& path)
{
    return m_fileSystem->loadFileAsText(path);
}
