#pragma once
#include "glm/glm.hpp"
#include <vector>

namespace Daybreak
{
    // Simple mesh.
    template<typename TVertex, typename TIndex>
    class TMeshData
    {
    public:

    private:
        std::vector<TVertex> m_vertices;
        std::vector<TIndex> m_indices;
    };
}