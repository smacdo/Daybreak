#pragma once
#include <memory>
#include <string>
#include <future>

namespace Daybreak
{
    class IFileSystem;
    class ModelData;
    class Image;
    
    /** Manages the loading, unloading and lifetime of game resources. */
    class ResourcesManager
    {
    public:
        /** Constructor. */
        ResourcesManager(std::shared_ptr<IFileSystem> fileSystem);

        /** Load a 3d model. */
        std::future<std::unique_ptr<ModelData>> loadModel(const std::string& path);

        /** Load an image. */
        std::future<std::unique_ptr<Image>> loadImage(const std::string& path);

        /** Load a text file. */
        std::future<std::string> loadTextFile(const std::string& path);

    private:
        std::shared_ptr<IFileSystem> m_fileSystem;
    };
}
