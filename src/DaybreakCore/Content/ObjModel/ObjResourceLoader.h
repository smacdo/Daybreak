#pragma once
#include "Content/IResourceLoader.h"
#include "Graphics/ModelData.h"

namespace Daybreak
{
    class ObjResourceLoader : public IResourceLoader<ModelData>
    {
    public:
        virtual std::unique_ptr<ModelData> load(
            const std::string& resourcePath,
            ResourcesManager& resources) override;
    };
}