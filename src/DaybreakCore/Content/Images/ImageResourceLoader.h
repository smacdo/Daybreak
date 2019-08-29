#pragma once
#include "Content\IResourceLoader.h"
#include "Content\Images\Image.h"

namespace Daybreak
{
    class ImageResourceLoader : public IResourceLoader<Image>
    {
    public:
        virtual std::future<std::unique_ptr<Image>> load(
            const std::string& resourcePath,
            ResourcesManager& resources) override;
    };
}
