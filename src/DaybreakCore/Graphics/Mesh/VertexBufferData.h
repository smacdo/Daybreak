#pragma once
#include "Graphics/Mesh/BufferData.h"
#include "Graphics/Mesh/VertexAttributeIterator.h"
#include "Graphics/InputLayoutDescription.h"

#include <memory>
#include <vector>
#include <cstdint>

namespace Daybreak
{
    /// Vertex buffer data that can be accessed via attribute streams and uploaded to the GPU.
    class VertexBufferData final : public BufferData
    {
    public:
        // Constructor.
        VertexBufferData(
            _In_ size_t byteCount,
            _In_ std::unique_ptr<uint8_t[]> bytes,
            _In_ std::shared_ptr<const InputLayoutDescription> inputLayout);

        // Typed vertex constructor.
        template<typename TVertex>
        VertexBufferData(
            _In_ size_t vertexCount,
            _In_ std::unique_ptr<TVertex[]> vertices,
            _In_ std::shared_ptr<const InputLayoutDescription> inputLayout)
            : VertexBufferData(
                vertexCount * sizeof(TVertex),
                std::unique_ptr<uint8_t[]>(reinterpret_cast<uint8_t*>(vertices.release())),
                inputLayout)
        {
        }

        // Destructor.
        virtual ~VertexBufferData();

        // Get shared pointer to input layout.
        std::shared_ptr<const InputLayoutDescription> inputLayout() const noexcept { return m_inputLayout; }

        // Get reference to input layout description.
        const InputLayoutDescription& inputLayoutRef() const noexcept { return *m_inputLayout.get(); }

        // Get the number of vertices stored in the vertex buffer.
        size_t vertexCount() const noexcept { return m_vertexCount; }

        // Get an iterator pointing to the start of an attribute stream.
        template<typename T>
        VertexAttributeIterator<T> attributeBegin(InputAttribute::SemanticName name, unsigned int index = 0)
        {
            auto attributeIndex = m_inputLayout->getAttributeIndexByName(name, index);
            auto attributeOffset = m_inputLayout->attributeOffsetByIndex(attributeIndex);
            auto attribute = m_inputLayout->getAttributeByIndex(attributeIndex);

            if (sizeof(T) != attribute.sizeInBytes())
            {
                // TODO: Better exception.
                throw std::runtime_error("Attribute iterator value type does not match input layout");
            }

            return VertexAttributeIterator<T>(
                m_bytes.get(),
                attributeOffset,
                m_inputLayout->elementSizeInBytes());
        }

    protected:
        std::shared_ptr<const InputLayoutDescription> m_inputLayout;
        size_t m_vertexCount = 0;
    };
}
