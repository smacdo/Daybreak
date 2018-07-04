#pragma once
#include "Graphics/Mesh/MeshData.h"
#include "Graphics/Mesh/IndexBufferData.h"
#include "Graphics/Mesh/IndexBufferElement.h"
#include "Graphics/Mesh/VertexBufferData.h"
#include "Graphics/Mesh/VertexFormat.h"

namespace Daybreak
{
    // [BLURB]
    class MeshData_ptn_ui32 : public MeshData
    {
    public:
        MeshData_ptn_ui32(
            std::unique_ptr<TIndexBufferData<index_element_ui32>> indices,
            std::unique_ptr<TVertexBufferData<vertex_ptn_t>> vertices)
            : MeshData(std::move(indices), std::move(vertices))
        {
        }
    };
}
