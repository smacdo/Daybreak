#pragma once
#include <memory>

namespace Daybreak
{
    /// Base for vertex and index buffer data.
    class BufferData
    {
    public:
        // Constructor.
        BufferData(
            _In_ std::unique_ptr<uint8_t[]> rawData,
            _In_ size_t elemnetCount);

        // Copy constructor (deleted, cannot copy unowned pointer).
        BufferData(const BufferData&) = delete;

        // Destructor.
        virtual ~BufferData();

        // Assignment operator (deleted), cannot copy unowned pointer).
        BufferData& operator =(const BufferData&) = delete;

    public:
        // Get an unowned pointer to the raw bytes of the vertex buffer.
        const void * data() const noexcept { return m_dataUnownedPtr; }

        // Get the number of elements in the buffer.
        size_t elementCount() const noexcept { return m_elementCount; }

    protected:
        // Internal constructor that does not set a pointer. Derived class must make sure to set it.
        BufferData(_In_ size_t elemnetCount);

        // Allows derived class to set an unowned pointer that the derived class is responsible for maintaining.
        void setUnownedDataPtr(_In_ void * unownedDataPtr);

    protected:
        // Pointer to raw untyped vertex buffer data.
        std::unique_ptr<uint8_t[]> m_data;

        // Special unowned pointer (lifetime must be maintained by derived classes!).
        void * m_dataUnownedPtr = nullptr;

        // Number of indices in buffer.
        size_t m_elementCount = 0;
    };
}
