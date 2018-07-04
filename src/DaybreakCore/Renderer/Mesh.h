#pragma once
#include <memory>

namespace Daybreak
{
    class IndexBuffer;
    class VertexBuffer;
    class PhongMaterial;

    // Drawable mesh defined by a vertex and index buffer with an optional material.
    class Mesh
    {
    public:
        // Constructor.
        Mesh(
            _In_ std::shared_ptr<VertexBuffer> vertexBuffer,
            _In_ std::shared_ptr<IndexBuffer> indexBuffer,
            _In_opt_ std::shared_ptr<PhongMaterial> material);

        // Destructor.
        virtual ~Mesh();

    public:
        // Get the vertex buffer.
        std::shared_ptr<VertexBuffer> vertexBuffer() const { return m_vertexBuffer; }

        // Get reference to vertex buffer.
        const VertexBuffer& vertexBufferRef() const { return *m_vertexBuffer.get(); }

        // Set the vertex buffer.
        void setVertexBuffer(std::shared_ptr<VertexBuffer> vertexBuffer) { m_vertexBuffer = vertexBuffer; }

        // Get the index buffer.
        std::shared_ptr<IndexBuffer> indexBuffer() const { return m_indexBuffer; }

        // Get reference to index buffer.
        const IndexBuffer& indexBufferRef() const { return *m_indexBuffer.get(); }

        // Set the index buffer.
        void setIndexBuffer(std::shared_ptr<IndexBuffer> indexBuffer) { m_indexBuffer = indexBuffer; }

        // Get the material.
        std::shared_ptr<PhongMaterial> material() const { return m_material; }

        // Get reference to material.
        const PhongMaterial& materialRef() const { return *m_material.get(); }

        // Set the material.
        void setIndexBuffer(std::shared_ptr<PhongMaterial> material) { m_material = material; }

    private:
        std::shared_ptr<VertexBuffer> m_vertexBuffer;
        std::shared_ptr<IndexBuffer> m_indexBuffer;
        std::shared_ptr<PhongMaterial> m_material;
    };
}