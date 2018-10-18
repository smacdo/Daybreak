#include "stdafx.h"
#include "ObjResourceLoader.h"
#include "Content/ResourcesManager.h"
#include "Content/ObjModel/ObjModelLoader.h"
#include "Graphics/ModelData.h"
#include "Graphics/Mesh/IndexBufferData.h"
#include "Graphics/Mesh/VertexBufferData.h"
#include "Graphics/Mesh/MeshData.h"
#include "Graphics/Mesh/VertexFormat.h"

using namespace Daybreak;

//---------------------------------------------------------------------------------------------------------------------
std::unique_ptr<ModelData> ObjResourceLoader::load(
    const std::string& resourcePath,
    ResourcesManager& resources)
{
    // TODO: Improve this code because it was written quickly to get minimal obj support working.
    ObjModelParser parser;

    auto fileText = resources.loadTextFile(resourcePath);
    auto objData = parser.parse(fileText, resourcePath);

    size_t faceCount = 0;

    for (const auto& group : objData->groups)
    {
        faceCount += group.faces.size();
    }

    auto vertexCount = faceCount * 3;
    std::unique_ptr<vertex_ptn_t[]> vertices(new vertex_ptn_t[vertexCount]);

    auto indexCount = faceCount * 3;
    std::unique_ptr<uint32_t[]> indices(new uint32_t[indexCount]);

    size_t index = 0;

    for (const auto& group : objData->groups)
    {
        for (const auto& face : group.faces)
        {
            for (int j = 0; j < 3; ++j)
            {
                indices[index] = static_cast<uint32_t>(index);

                auto position = objData->positions[static_cast<size_t>(face.position[j] - 1)];
                vertices[index].setPosition(position.x, position.y, position.z);

                if (objData->hasUV)
                {
                    auto uv = objData->uv[static_cast<size_t>(face.uv[j] - 1)];
                    vertices[index].setUV(uv.x, uv.y);
                }
                else
                {
                    vertices[index].setUV(0.0f, 0.0f);
                }

                if (objData->hasNormals)
                {
                    auto normal = objData->normals[static_cast<size_t>(face.normal[j] - 1)];
                    vertices[index].setNormal(normal.x, normal.y, normal.z);
                }
                else
                {
                    vertices[index].setNormal(0.0f, 0.0f, 0.0f);
                }

                index++;
            }
        }
    }

    return std::make_unique<ModelData>(
        std::make_unique<MeshData>(
            std::make_unique<IndexBufferData>(
                indexCount,
                std::move(indices)),
            std::make_unique<VertexBufferData>(
                vertexCount,
                std::move(vertices),
                vertex_ptn_t::inputLayout)));
}
