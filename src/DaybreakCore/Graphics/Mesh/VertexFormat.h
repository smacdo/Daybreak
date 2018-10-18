#pragma once
#include <vector>
#include <memory>

namespace Daybreak
{
    class InputLayoutDescription;

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

        void setPosition(float x, float y, float z)
        {
            elements[0] = x;
            elements[1] = y;
            elements[2] = z;
        }

        void setUV(float u, float v)
        {
            elements[3] = u;
            elements[4] = v;
        }

        void setNormal(float x, float y, float z)
        {
            elements[5] = x;
            elements[6] = y;
            elements[7] = z;
        }

        float elements[3 + 2 + 3];

    public:
        static std::shared_ptr<InputLayoutDescription> inputLayout;
    };
}