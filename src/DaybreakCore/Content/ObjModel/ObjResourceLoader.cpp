#include "stdafx.h"
#include "ObjResourceLoader.h"
#include "app/support/hash.h"
#include "Content/ResourcesManager.h"
#include "Content/ObjModel/ObjModelParser.h"
#include "Content\Models\ModelData.h"
#include "Content\Materials\MaterialData.h"
#include "Content\ObjModel\MtlMaterialParser.h"
#include "Graphics/Mesh/IndexBufferData.h"
#include "Graphics/Mesh/VertexBufferData.h"
#include "Graphics/Mesh/MeshData.h"
#include "Graphics/Mesh/VertexFormat.h"

#include <functional>
#include <unordered_map>

using namespace Daybreak;

//---------------------------------------------------------------------------------------------------------------------
namespace
{
    struct obj_face_vertex_hasher_t
    {
    public:
        std::size_t operator()(const obj_face_vertex_t& f) const noexcept
        {
            std::hash<decltype(f.p)> hasher;
            auto h = hasher(f.p);

            if (f.hasUV())
            {
                h = combine_hash(h, f.t);
            }

            if (f.hasNormals())
            {
                h = combine_hash(h, f.n);
            }

            return h;
        }
    };
}

//---------------------------------------------------------------------------------------------------------------------
std::unique_ptr<ModelData> ObjResourceLoader::load(
    const std::string& resourcePath,
    ResourcesManager& resources)
{
    // TODO: Improve this code because it was written quickly to get minimal obj support working.
    //       In particular it does not use async I/O.
    ObjModelParser parser;

    auto fileText = resources.loadTextFile(resourcePath);
    auto objData = parser.parse(fileText, resourcePath);

    auto materials = loadMaterials(*(objData.get()), resources);

    return std::move(convert(std::move(objData), materials));
}

//---------------------------------------------------------------------------------------------------------------------
ObjResourceLoader::material_lut_t ObjResourceLoader::loadMaterials(
    const obj_model_t& objModel,
    ResourcesManager& resources)
{
    material_lut_t lut;

    // Load all the referenced material libraries in the obj file.
    for (const auto& materialLibrary : objModel.materialLibraries)
    {
        // TODO: Only add materials that are actually referenced by the model.
        // TODO: Warn about duplicate material names.
        auto materials = loadMtl(materialLibrary, resources);

        for (size_t i = 0; i < materials.size(); ++i)
        {
            lut.emplace(materials[i]->name(), std::move(materials[i]));
        }
    }

    return lut;
}

//---------------------------------------------------------------------------------------------------------------------
std::vector<std::unique_ptr<MaterialData>> ObjResourceLoader::loadMtl(
    const std::string& filepath,
    ResourcesManager& resources)
{
    MtlMaterialParser parser;

    auto fileText = resources.loadTextFile(filepath);
    return parser.parse(fileText, filepath);
}

//---------------------------------------------------------------------------------------------------------------------
std::unique_ptr<ModelData> ObjResourceLoader::convert(
    std::unique_ptr<obj_model_t> objModel,
    const material_lut_t& materials)
{
    // Get a count of the total number of faces in the model across all groups.
    size_t faceCount = 0;  

    for (const auto& group : objModel->groups)
    {
        faceCount += group.faces.size();
    }

    // Allocate space for vertex buffer (use the standard position/texture/normal layout).
    // TODO: Trim space after combined vertex count.
    auto vertexCount = faceCount * 3;
    std::unique_ptr<vertex_ptn_t[]> vertices(new vertex_ptn_t[vertexCount]);

    // Allocate space for the index buffer.
    auto indexCount = faceCount * 3;
    
    std::unique_ptr<uint32_t[]> indices(new uint32_t[indexCount]);

    // Generate vertices for each face group in the obj model.
    using index_t = uint32_t;
    
    std::unordered_map<obj_face_vertex_t, index_t, obj_face_vertex_hasher_t> vertexCache;
    std::vector<ModelData::Group> groups;

    index_t nextVertexIndex = 0;
    size_t nextIndex = 0;

    for (const auto& group : objModel->groups)
    {
        // Record the first index in this group.
        auto firstIndex = nextIndex;

        // Generate vertices for each face in the obj model group.
        for (const auto& face : group.faces)
        {
            for (int j = 0; j < 3; ++j)
            {
                // If the face vertex does not exist in the cache generate it.
                auto objVertex = face.vertex(j);
                auto itr = vertexCache.find(objVertex);

                if (itr == vertexCache.end())
                {
                    // Generate an entry in the vertex buffer for this face vertex.
                    auto position = objModel->positions[static_cast<size_t>(face.position[j]) - 1];
                    vertices[nextVertexIndex].setPosition(position.x, position.y, position.z);

                    if (objModel->hasUV)
                    {
                        auto uv = objModel->uv[static_cast<size_t>(face.uv[j]) - 1];
                        vertices[nextVertexIndex].setUV(uv.x, uv.y);
                    }
                    else
                    {
                        vertices[nextVertexIndex].setUV(0.0f, 0.0f);
                    }

                    if (objModel->hasNormals)
                    {
                        auto normal = objModel->normals[static_cast<size_t>(face.normal[j]) - 1];
                        vertices[nextVertexIndex].setNormal(normal.x, normal.y, normal.z);
                    }
                    else
                    {
                        vertices[nextVertexIndex].setNormal(0.0f, 0.0f, 0.0f);
                    }

                    // Record the index of the face vertex.
                    vertexCache[objVertex] = nextVertexIndex++;
                }

                // Look up the cached vertice's index in the vertex buffer.
                auto vertexIndex = vertexCache[objVertex];

                indices[nextIndex] = vertexIndex;
                nextIndex++;
            }
        }

        // Create a group definition in the model data for this obj group.
        // TODO: Add material sharing.
        // TODO: Warn if material is missing (or error but not exception).
        auto groupIndexCount = nextIndex - firstIndex;
        auto materialItr = materials.find(group.material);

        groups.emplace_back(ModelData::Group(
            group.name,
            (materialItr == materials.end() ? nullptr : materialItr->second),
            firstIndex,
            groupIndexCount));
    }

    // Create the Daybreak model along with index and vertex buffers.
    auto modelData = std::make_unique<ModelData>(
        std::make_unique<MeshData>(
            std::make_unique<IndexBufferData>(
                indexCount,
                std::move(indices)),
            std::make_unique<VertexBufferData>(
                vertexCount,
                std::move(vertices),
                vertex_ptn_t::inputLayout)));

    // Add groups to the Daybreak model.
    modelData->addGroup(std::move(groups));

    // All done!
    return std::move(modelData);
}
