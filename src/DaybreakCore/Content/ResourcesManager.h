#pragma once
#include <memory>
#include <string>
#include <future>

namespace Daybreak
{
    class IFileSystem;
    class IDeviceContext;
    class ModelData;
    class MaterialData;
    class Image;

    enum class MaterialParameterType : int;
    
    /** Manages the loading, unloading and lifetime of game resources. */
    class ResourcesManager
    {
    public:
        /** Initialize resources manager. */
        ResourcesManager(
            std::shared_ptr<IDeviceContext> deviceContext,          ///< Current device context for creating resources.
            std::shared_ptr<IFileSystem> fileSystem);               ///< Current file system device.

        /** Load a 3d model. */
        std::future<std::unique_ptr<ModelData>> loadModel(const std::string& path);

        /** Load an image. */
        std::future<std::unique_ptr<Image>> loadImage(const std::string& path);

        /** Load a text file. */
        std::future<std::string> loadTextFile(const std::string& path);

    private:
        bool loadMaterialTextureParameterIfMissing(
            MaterialData& material,
            MaterialParameterType paramType);

    private:
        std::shared_ptr<IDeviceContext> m_deviceContext;
        std::shared_ptr<IFileSystem> m_fileSystem;
    };
}
