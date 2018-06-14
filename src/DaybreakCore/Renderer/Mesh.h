#pragma once
#include <memory>

namespace Daybreak
{
    class IIndexBuffer;
    class IVertexBuffer;
    class PhongMaterial;

    // Graphics API indepnedent renderable mesh object.
    class Mesh
    {
    public:
        // Constructor.
        Mesh(
            std::shared_ptr<IVertexBuffer> vertexBuffer,
            std::shared_ptr<IIndexBuffer> indexBuffer,
            std::shared_ptr<PhongMaterial> material);

        // Destructor.
        virtual ~Mesh();

    public:
        // Get the vertex buffer.
        std::shared_ptr<IVertexBuffer> vertexBuffer() const { return m_vertexBuffer; }

        // Get reference to vertex buffer.
        const IVertexBuffer& vertexBufferRef() const { return *m_vertexBuffer.get(); }

        // Set the vertex buffer.
        void setVertexBuffer(std::shared_ptr<IVertexBuffer> vertexBuffer) { m_vertexBuffer = vertexBuffer; }

        // Get the index buffer.
        std::shared_ptr<IIndexBuffer> indexBuffer() const { return m_indexBuffer; }

        // Get reference to index buffer.
        const IIndexBuffer& indexBufferRef() const { return *m_indexBuffer.get(); }

        // Set the index buffer.
        void setIndexBuffer(std::shared_ptr<IIndexBuffer> indexBuffer) { m_indexBuffer = indexBuffer; }

        // Get the material.
        std::shared_ptr<PhongMaterial> material() const { return m_material; }

        // Get reference to material.
        const PhongMaterial& materialRef() const { return *m_material.get(); }

        // Set the material.
        void setIndexBuffer(std::shared_ptr<PhongMaterial> material) { m_material = material; }

    private:
        std::shared_ptr<IVertexBuffer> m_vertexBuffer;
        std::shared_ptr<IIndexBuffer> m_indexBuffer;
        std::shared_ptr<PhongMaterial> m_material;
    };
}