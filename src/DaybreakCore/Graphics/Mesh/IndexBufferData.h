#pragma once
#include "Graphics/Mesh/BufferData.h"
#include "Graphics/Mesh/IndexBufferElement.h"

#include <memory>
#include <vector>

namespace Daybreak
{
    /// Software index buffer.
    class IndexBufferData : public BufferData
    {
    public:
        // Constructor.
        IndexBufferData(
            _In_ std::unique_ptr<void *> rawData,
            _In_ IndexElementType elementType,
            _In_ size_t elemnetCount);

        // Destructor.
        virtual ~IndexBufferData();

        // Get the type of elments in the buffer.
        IndexElementType elementType() const noexcept { return m_elementType; }

    protected:
        // Internal constructor that does not set a pointer. Derived class must make sure to set it.
        IndexBufferData(
            _In_ IndexElementType elementType,
            _In_ size_t elemnetCount);

    protected:
        // Index element type definition.
        IndexElementType m_elementType;
    };

    /// Typed software index buffer.
    template<typename TIndex>
    class TIndexBufferData : public IndexBufferData // TODO: Use static_assert to validate size matches
    {
    public:
        // Constructor.
        TIndexBufferData(_In_ const std::vector<TIndex>&& indices)
            : IndexBufferData(
                TIndex::elementType,
                indices.size()),
              m_indices(indices)
        {
            setUnownedDataPtr(m_indices.data());
        }

    public:
        // Get an unowned pointer to the raw bytes of the index buffer.
        const TIndex * typedDataPtr() const noexcept { return m_indices.size(); }

    private:
        std::vector<TIndex> m_indices;
    };
}
