#pragma once
#include "Content/IResourceLoader.h"
#include "Content\Models\ModelData.h"

#include <memory>
#include <unordered_map>

namespace Daybreak
{
    struct obj_model_t;

    class ObjResourceLoader : public IResourceLoader<ModelData>
    {
    public:
        using material_lut_t =  std::unordered_map<std::string, std::shared_ptr<MaterialData>>;

    public:
        virtual std::unique_ptr<ModelData> load(
            const std::string& resourcePath,
            ResourcesManager& resources) override;

        /** Convert a obj model into a Daybreak model. */
        static std::unique_ptr<ModelData> convert(
            std::unique_ptr<obj_model_t> objModel,      // TODO: Pass by const ref.
            const material_lut_t& materials);

        /** Get all referenced materials in an obj model. */
        static material_lut_t loadMaterials(
            const obj_model_t& objModel,            ///< OBJ model to load materials for.
            ResourcesManager& resources);           ///< Reference to active resource manager.

        /** Load an MTL material library from disk. */
        static std::vector<std::unique_ptr<MaterialData>> loadMtl(
            const std::string& filepath,
            ResourcesManager& resources);
    };
}