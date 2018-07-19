#pragma once
#include "Graphics/Mesh/IndexBufferElement.h"
#include <memory>

namespace Daybreak
{
    class IndexBufferData;
    class VertexBufferData;
    class InputLayout;

    // [NEED BLURB]
    class MeshData
    {
    public:
        MeshData(
            _In_ std::unique_ptr<IndexBufferData> indexBuffer,
            _In_ std::unique_ptr<VertexBufferData> vertexBuffer);

        virtual ~MeshData();

    public:
        const void * rawIndexBufferData() const noexcept;

        size_t indexCount() const noexcept;

        size_t indexElementSizeInBytes() const noexcept;

        IndexElementType indexElementType() const noexcept;

    public:

        const void * rawVertexBufferData() const noexcept;

        size_t vertexCount() const noexcept;

        size_t vertexElementSizeInBytes() const noexcept;

        const InputLayout& vertexElementTypeRef() const noexcept;       // TODO: Rename inputLayoutRef

    protected:
        std::unique_ptr<IndexBufferData> m_indexBuffer;
        std::unique_ptr<VertexBufferData> m_vertexBuffer;
    };
}