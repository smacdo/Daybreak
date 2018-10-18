#pragma once
#include <memory>
#include <string>

namespace Daybreak
{
    class ResourcesManager;

    /** Interface for a class that will load a resource. */
    template<typename TResource>
    class IResourceLoader
    {
    public:
        /** 
         * Returns a fully constructed resource given a resource path and the resource manager for loading depenendent
         * resources.
         */
        virtual std::unique_ptr<TResource> load(
            const std::string& resourcePath,
            ResourcesManager& resources) = 0;
    };
}