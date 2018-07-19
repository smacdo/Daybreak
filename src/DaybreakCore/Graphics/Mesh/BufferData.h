#pragma once
#include <memory>

namespace Daybreak
{
    /// Contains untyped data to be uploaded to a GPU buffer.
    class BufferData
    {
    public:
        /// Default constructor.
        BufferData();

        /// Constructor.
        BufferData(
            _In_ size_t byteCount,
            _In_ std::unique_ptr<uint8_t[]> bytes);

        /// Copy constructor (deleted, data buffers cannot be copied).
        BufferData(const BufferData&) = delete;

        /// Destructor.
        virtual ~BufferData();

        /// Assignment operator (deleted, data buffers cannot be copied).
        BufferData& operator =(const BufferData&) = delete;

    public:
        /// Get an unowned pointer to the raw bytes of the data buffer.
        _Ret_maybenull_ const void * bytes() const noexcept { return m_bytes.get(); }

        /// Get the size (in bytes) of the byte buffer.
        size_t byteCount() const noexcept { return m_byteCount; }

        /// Set a pointer to a byte buffer and the size (in bytes) of the buffer.
        void setUnownedDataPtr(_In_ size_t byteCount, _In_ void * bytes) noexcept;

    private:
        static void defaultDelete(uint8_t * data) noexcept;
        static void noDelete(uint8_t * data) noexcept;

    protected:
        // Untyped byte buffer pointer.
        std::unique_ptr<uint8_t[], void(*)(uint8_t*)> m_bytes;

        // Number of bytes in buffer.
        size_t m_byteCount = 0;
    };
}
