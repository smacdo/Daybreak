#pragma once
#include "Content/IResourceLoader.h"
#include "Content\Models\ModelData.h"

namespace Daybreak
{
    struct obj_model_t;

    class ObjResourceLoader : public IResourceLoader<ModelData>
    {
    public:
        virtual std::future<std::unique_ptr<ModelData>> load(
            const std::string& resourcePath,
            ResourcesManager& resources) override;

        static std::unique_ptr<ModelData> convert(std::unique_ptr<obj_model_t> objModel);
    };
}