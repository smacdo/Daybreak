#include "stdafx.h"
#include "ResourcesManager.h"
#include "Content/IFileSystem.h"
#include "Common/Error.h"

#include "Content/ObjModel/ObjResourceLoader.h"
#include "Content\Models\ModelData.h"
#include "Content\Materials\MaterialData.h"
#include "Content\Images\ImageResourceLoader.h"
#include "Content\Images\Image.h"

#include "Renderer\DeviceContext.h"

#include <future>

using namespace Daybreak;

// TODO: Rewrite the futures system with async i/o and threads or something. I'm not sure, all I know is this is
//       horrible and barely better than synchronous loading.

//---------------------------------------------------------------------------------------------------------------------
ResourcesManager::ResourcesManager(
    std::shared_ptr<IDeviceContext> deviceContext,
    std::shared_ptr<IFileSystem> fileSystem)
    : m_deviceContext(deviceContext),
      m_fileSystem(fileSystem)
{
    CHECK_NOT_NULL(m_deviceContext);
    CHECK_NOT_NULL(m_fileSystem);
}

//---------------------------------------------------------------------------------------------------------------------
std::future<std::unique_ptr<ModelData>> ResourcesManager::loadModel(const std::string& path)
{
    return std::async(
        std::launch::async | std::launch::deferred,
        [path, this]() {
            // Read the model from disk and convert it to the Daybreak model format.
            // TODO: Use appropriate loader for the file format.
            ObjResourceLoader l;
            auto model = l.load(path, *this).get();

            // Load all textures associated with materials in the model.
            // TODO: Use a stack vector and wait in parallel.
            for (auto& group : model->groups())
            {
                // Skip groups that don't have a material.
                if (!group.hasMaterial())
                {
                    continue;
                }

                // TODO: Is there a better way to do this?
                auto& material = group.materialRef();
                
                loadMaterialTextureParameterIfMissing(material, MaterialParameterType::DiffuseMap);
                loadMaterialTextureParameterIfMissing(material, MaterialParameterType::SpecularMap);
                loadMaterialTextureParameterIfMissing(material, MaterialParameterType::DisplacementMap);
                loadMaterialTextureParameterIfMissing(material, MaterialParameterType::NormalMap);
                loadMaterialTextureParameterIfMissing(material, MaterialParameterType::DiffuseMap);
            }

            // Return the loaded model.
            return std::move(model);
        });   
}

//---------------------------------------------------------------------------------------------------------------------
bool ResourcesManager::loadMaterialTextureParameterIfMissing(
    MaterialData& material,
    MaterialParameterType paramType)
{
    if (material.isParameterDefined(paramType))
    {
        auto param = material.getTextureParameter(paramType);

        // Load the texture from disk if it is not (yet) present).
        if (param.texture == nullptr)
        {
            // TODO: Warn if the texture could not be loaded.
            auto image = std::shared_ptr<Image>(std::move(loadImage(param.filepath).get()));

            // TODO: Support loading texture in format specified by material.
            // TODO: Support loading textures in non vendor specific API.
            param.texture = m_deviceContext->createTexture2d(
                *(image.get()),
                param.textureParams);

            // Remove the parameter if the texture could not be loaded otherwise store the texture in the material.
            if (param.texture == nullptr)
            {
                material.removeParameter(paramType);
            }
            else
            {
                material.setParameter(paramType, param);
            }
        }

        return true;
    }

    return false;
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
