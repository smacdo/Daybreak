#pragma once

namespace Daybreak
{
    // A single data attribute in an input layout.
    class InputAttribute
    {
    public:
        // Intended use for an attribute.
        enum class SemanticName
        {
            None,
            Position,
            Texture,
            Normal
        };

        // Underlying storage type for an attribute.
        enum class StorageType
        {
            Byte,
            UnsignedByte,
            Short,
            UnsignedShort,
            Int,
            UnsignedInt,
            HalfFloat,
            Float,
            Double
        };

    public:
        /// Default constructor.
        InputAttribute();

        /// Value constructor.
        InputAttribute(
            _In_ SemanticName name,
            _In_ unsigned int index,
            _In_ StorageType type,
            _In_ unsigned int count);

    public:
        /// Get attribute intended use.
        SemanticName semanticName() const noexcept { return m_name; }

        /// Set attribute intended use.
        void setSemanticName(_In_ SemanticName name) noexcept { m_name = name; }

        /// Get attribute index (for when there are multiple identical semantic names).
        unsigned int semanticIndex() const noexcept { return m_index; }

        /// Set attribute index (for when there are multiple identical semantic names).
        void setSemanticIndex(_In_ unsigned int index) noexcept { m_index = index; }

        /// Get attribute type.
        StorageType type() const noexcept { return m_type; }

        /// Set attribute type.
        void setType(_In_ StorageType type) noexcept { m_type = type; }

        /// Get the storage count for this attribute.
        unsigned int count() const noexcept { return m_count; }

        /// Set the storage count for this attribute.
        void setCount(_In_ unsigned int count) { m_count = count; }

        /// Get attribute size in bytes.
        size_t sizeInBytes() const;

    public:
        /// Get attribute type size in bytes.
        static size_t typeSizeInBytes(StorageType type);

    private:
        SemanticName m_name;
        unsigned int m_index;
        StorageType m_type;
        unsigned int m_count;
    };

    /// Input attribute equality operator.
    bool operator ==(const InputAttribute& lhs, const InputAttribute& rhs) noexcept;
}
