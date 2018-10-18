#pragma once
#include <memory>
#include <string>

namespace Daybreak
{
    class IFileSystem;
    class ModelData;
    
    /** Manages the loading, unloading and lifetime of game resources. */
    class ResourcesManager
    {
    public:
        /** Constructor. */
        ResourcesManager(std::shared_ptr<IFileSystem> fileSystem);

        /** Load a 3d model. */
        std::unique_ptr<ModelData> loadModel(const std::string& path);

        /** Load a text file. */
        std::string loadTextFile(const std::string& path);

    private:
        std::shared_ptr<IFileSystem> m_fileSystem;
    };
}
