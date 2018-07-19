#pragma once
#include <vector>
#include <memory>

namespace Daybreak
{
    class InputLayout;

    // Standard lit vertex (position, texture 0, normal).
    //  TODO: Move to another file.
    struct vertex_ptn_t
    {
    public:
        vertex_ptn_t()
            : vertex_ptn_t(0, 0, 0, 0, 0, 0, 0, 0)
        {
        }

        vertex_ptn_t(float px, float py, float pz, float u, float v, float nx, float ny, float nz)
        {
            elements[0] = px; elements[1] = py; elements[2] = pz;
            elements[3] = u; elements[4] = v;
            elements[5] = nx; elements[6] = ny; elements[7] = nz;
        }

        float elements[3 + 2 + 3];

    public:
        static std::shared_ptr<InputLayout> inputLayout;
    };
}