#pragma once
#include <vector>
#include <memory>

namespace Daybreak
{
    // Defines what a vertex attribute is used for.
    enum class VertexAttributeName
    {
        Position,
        Texture,
        Normal
    };

    // Defines how a vertex attribute is stored. Not all storage types are valid for all vertex attribute names.
    enum class VertexAttributeStorage
    {
        Float2,
        Float3,
        Float4
    };

    // Defines one attribute in a vertex format.
    struct vertex_attribute_t
    {
        VertexAttributeName name;
        VertexAttributeStorage storage;
        unsigned int index;       // Used for attributes like texture.
    };

    // Defines the layout and composition of the attributes that make up a vertex.
    class VertexFormat
    {
    public:
        // Default constructor.
        VertexFormat();

        // Constructor.
        VertexFormat(_In_ const std::vector<vertex_attribute_t>& attributes);

        // Add a vertex attribute.
        void addAttribute(VertexAttributeName name, VertexAttributeStorage storage);

        // Add a vertex attribute.
        void addAttribute(VertexAttributeName name, VertexAttributeStorage storage, unsigned int index);

        // Get number of attributes.
        size_t attributeCount() const noexcept;

        // Get an attribute by index.
        vertex_attribute_t getAttributeByIndex(unsigned int index) const noexcept;

        // Get the size of a single vertex.
        size_t vertexSizeInBytes() const noexcept;

    private:
        std::vector<vertex_attribute_t> m_attributes;
    };

    // Standard lit vertex (position, texture 0, normal).
    //  TODO: Move to another file.
    struct vertex_ptn_t
    {
    public:
        vertex_ptn_t(float px, float py, float pz, float u, float v, float nx, float ny, float nz)
        {
            elements[0] = px; elements[1] = py; elements[2] = pz;
            elements[3] = u; elements[4] = v;
            elements[5] = nx; elements[6] = ny; elements[7] = nz;
        }

        float elements[3 + 2 + 3];

    public:
        static std::shared_ptr<VertexFormat> elementType;
    };
}