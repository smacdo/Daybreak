#include "stdafx.h"
#include "ImageResourceLoader.h"
#include "Content/ResourcesManager.h"
#include "Graphics/Image.h"

using namespace Daybreak;

//---------------------------------------------------------------------------------------------------------------------
std::future<std::unique_ptr<Image>> ImageResourceLoader::load(
    const std::string& resourcePath,
    ResourcesManager& resources)
{
    // TODO: Do not hard code the content directory here. Instead use the resource manager to load a binary blob
    //       (which will add the content directory) and pass the buffer (instead of a file path) to the stb image
    //       loader.
    auto finalResourcePath = "content\\" + resourcePath;

    return std::async(
        std::launch::async | std::launch::deferred,
        [finalResourcePath]() {
            std::unique_ptr<Image> image(BytePerChannelImage::LoadFromFile(finalResourcePath));
            return std::move(image);
    });
}
