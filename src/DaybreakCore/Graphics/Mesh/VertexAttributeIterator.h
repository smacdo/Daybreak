#pragma once
#include <iterator>
#include <functional>
#include <cstdint>

#include "Common/Error.h"

namespace Daybreak
{
    /**
     * Custom iterator for custom vertex types that allows developers to iterate through an array of vertices
     * for a specific attribute (position, texture, normal, etc) that is present at a fixed offset in each vertex.
     *
     * \code
     * struct vertex {
     *   vec3 position;
     *   vec2 uv;
     *   vec2 normal
     * };
     * std::unique_ptr<vertex> vertices(new vertex[1000]);
     * // fill out vertices...
     * VertexAttributeIterator<vertex> position_iterator(0, 0);
     * VertexAttributeIterator<vertex> uv_iterator(0, sizeof(vec3));
     * VertexAttributeIterator<vertex> normal_iterator(0, sizeof(vec3) + sizeof(vec2));
     * \endcode
     */
    template<typename TAttribute>
    class VertexAttributeIterator
    {
    public:
        typedef VertexAttributeIterator self_type;
        typedef TAttribute value_type;
        typedef TAttribute& reference;
        typedef TAttribute* pointer;
        typedef std::random_access_iterator_tag iterator_category;
        typedef ptrdiff_t difference_type;

    public:
        /**
         * Constructor.
         *
         * \param[in]  vertex      Pointer to vertex data.
         * \param attributeOffset  Offset in bytes from start of vertex to the attribute.
         * \param attributeStride  Offset in bytes from the attribute to next instance of the attribute.
         */
        VertexAttributeIterator(
            _In_ void * vertex, 
            size_t attributeOffset,
            size_t attributeStride) noexcept
            : VertexAttributeIterator(
                reinterpret_cast<std::byte*>(vertex) + attributeOffset,
                attributeStride)
        {
        }

    protected:
        /**
         * Constructor.
         *
         * \param[in]  attribute  Pointer to attribute.
         * \param      stride     Offset in bytes from the attribute to next instance of the attribute.
         */
        VertexAttributeIterator(
            _In_ std::byte * attribute,
            size_t stride) noexcept
            : m_pointer(attribute),
              m_stride(stride)
        {
            // Debug checks.
            EXPECT(attribute != nullptr, "Vertex attribute pointer cannot be null");
            EXPECT(stride > 0, "Vertex attribute stride must be larger than zero");
        }

    public:
        /** Copy constructor. */
        VertexAttributeIterator(const VertexAttributeIterator&) = default;

        /** Destructor. */
        ~VertexAttributeIterator() = default;

        /** Assignment operator. */
        VertexAttributeIterator& operator= (const VertexAttributeIterator&) = default;

        /* Iterator index operator. */
        value_type& operator [](const difference_type offset) noexcept
        {
            return *(*this + offset);
        }

        /** Const iterator index operator. */
        const value_type& operator [](const difference_type offset) const noexcept
        {
            return *(*this + offset);
        }

        /** Return attribute pointed to by this iterator as a reference. */
        reference operator *() const noexcept
        {
            return *reinterpret_cast<TAttribute*>(m_pointer);
        }

        /** Return attribute pointed to by this iterator as a pointer. */
        _Ret_notnull_ pointer operator ->() const noexcept
        {
            return reinterpret_cast<TAttribute*>(m_pointer);
        }

        /** Return an iterator incremented by count * stride. */
        self_type operator +(difference_type count) const noexcept
        {
            return self_type(m_pointer + count * m_stride, m_stride);
        }

        /** Advance iterator by count * stride. */
        self_type& operator +=(difference_type count) noexcept
        {
            m_pointer += count * m_stride;
            return *this;
        }

        /** Return an iterator decremented by count * stride. */
        self_type operator -(difference_type count) const noexcept
        {
            return self_type(m_pointer - count * m_stride, m_stride);
        }
        
        /** Decrease iterator by count * stride. */
        self_type& operator -=(difference_type count) noexcept
        {
            m_pointer -= count * m_stride;
            return *this;
        }

        /** Difference between two vertex attribute iterators as a stride multiple. */
        difference_type operator -(const self_type& other) const noexcept
        {
            EXPECT(m_stride == other.m_stride, "Cannot operate on two different types of iterators");

            difference_type distance = m_pointer - other.m_pointer; 
            EXPECT(distance % m_stride == 0, "Distance must be multiple of stride");

            return distance / m_stride;
        }

        /** Increment iterator by stride amount and return value of new iterator. */
        self_type& operator ++() noexcept
        {
            m_pointer += m_stride;
            return *this;
        }

        /** Incremnet iterator by stride amount and return value of previous iterator. */
        self_type operator ++(int) noexcept
        {
            self_type temp = *this;
            m_pointer += m_stride;

            return temp;
        }

        /** Decrement iterator by stride amount and return value of new iterator. */
        self_type& operator --() noexcept
        {
            m_pointer -= m_stride;
            return *this;
        }

        /** Decrement iterator by stride amount and return value of previous iterator .*/
        self_type operator --(int) noexcept
        {
            self_type temp = *this;
            m_pointer -= m_stride;

            return temp;
        }

        /** Equality operator. */
        bool operator == (const self_type& other) const noexcept
        {
            EXPECT(m_stride == other.m_stride, "Cannot compare iterators of different type");
            return m_pointer == other.m_pointer;
        }

        /** Inequality operator. */
        bool operator != (const self_type& other) const noexcept
        {
            EXPECT(m_stride == other.m_stride, "Cannot compare iterators of different type");
            return m_pointer != other.m_pointer;
        }

        /** Less than comparison. */
        bool operator <(const self_type& other) const noexcept
        {
            EXPECT(m_stride == other.m_stride, "Cannot compare iterators of different type");
            return m_pointer < other.m_pointer;
        }

        /** Less than equal comparison. */
        bool operator <=(const self_type& other) const noexcept
        {
            EXPECT(m_stride == other.m_stride, "Cannot compare iterators of different type");
            return m_pointer <= other.m_pointer;
        }

        /** Greater than equal comparison. */
        bool operator >(const self_type& other) const noexcept
        {
            EXPECT(m_stride == other.m_stride, "Cannot compare iterators of different type");
            return m_pointer > other.m_pointer;
        }

        /** Greater than equal comparison. */
        bool operator >=(const self_type& other) const noexcept
        {
            EXPECT(m_stride == other.m_stride, "Cannot compare iterators of different type");
            return m_pointer >= other.m_pointer;
        }

    private:
        std::byte * m_pointer = nullptr;
        size_t m_stride = 0;
    };
}
