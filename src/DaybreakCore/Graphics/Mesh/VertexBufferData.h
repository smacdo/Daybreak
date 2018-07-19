#pragma once
#include "Graphics/Mesh/BufferData.h"

#include <memory>
#include <vector>
#include <cstdint>

namespace Daybreak
{
    class InputLayout;

    /// Vertex buffer data that can be accessed via attribute streams and uploaded to the GPU.
    class VertexBufferData : public BufferData
    {
    public:
        // Constructor.
        VertexBufferData(
            _In_ std::unique_ptr<uint8_t[]> rawData,
            _In_ std::shared_ptr<InputLayout> inputLayout,
            _In_ size_t elemnetCount);

        // Destructor.
        virtual ~VertexBufferData();

        // Get reference to input layout description.
        const InputLayout& inputLayoutRef() const noexcept { return *m_inputLayout.get(); }

    protected:
        // Internal constructor that does not set a pointer. Derived class must make sure to set it.
        // TODO: Remove when TVertexBufferData class is removed.
        VertexBufferData(_In_ std::shared_ptr<InputLayout> elementType);

    protected:
        // Vertex element type definition.
        std::shared_ptr<InputLayout> m_inputLayout;
    };
    
    /// Typed software vertex buffer.
    template<typename TVertex>
    class TVertexBufferData : public VertexBufferData // TODO: Use static_assert to validate size matches
    {
    public:
        // Constructor.
        TVertexBufferData(_In_ const std::vector<TVertex>&& vertices)
            : VertexBufferData(TVertex::inputLayout),
              m_vertices(vertices)
        {
           setUnownedDataPtr(vertices.size(), m_vertices.data());
        }

    public:
        // Get an unowned pointer to the raw bytes of the vertex buffer.
        const TVertex * typedDataPtr() const noexcept { return m_vertices.size(); }

    private:
        std::vector<TVertex> m_vertices;
    };
}
