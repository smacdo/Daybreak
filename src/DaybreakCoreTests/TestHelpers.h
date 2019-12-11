#pragma once
#undef CHECK

#include <catch2\catch.hpp>
#include <glm/glm.hpp>

namespace Daybreak
{
    struct obj_face_t;
    struct obj_face_vertex_t;
    enum class IndexElementType;

    // TODO: Remove these when we Daybreak adds custom math library.
    template<typename T> struct vec2
    {
        vec2(T x, T y)
            : x(x), y(y)
        {
        }

        bool operator ==(const vec2& v) const
        {
            return x == v.x && y == v.y;
        }

        T x;
        T y;
    };

    template<typename T> struct vec3
    {
        vec3(T x, T y, T z)
            : x(x), y(y), z(z)
        {
        }

        bool operator ==(const vec3& v) const
        {
            return x == v.x && y == v.y && z == v.z;
        }

        T x;
        T y;
        T z;
    };
}

std::ostream& operator <<(std::ostream& os, const glm::vec2& v);            // TODO: Remove when not using GLM.
std::ostream& operator <<(std::ostream& os, const glm::vec3& v);            // TODO: Remove when GLM removed.
std::ostream& operator <<(std::ostream& os, const Daybreak::obj_face_t& v);
std::ostream& operator <<(std::ostream& os, const Daybreak::obj_face_vertex_t& v);
std::ostream& operator <<(std::ostream& os, Daybreak::IndexElementType type);
std::ostream& operator <<(std::ostream& os, const Daybreak::vec2<float>& v);
std::ostream& operator <<(std::ostream& os, const Daybreak::vec3<int>& v);
std::ostream& operator <<(std::ostream& os, const Daybreak::vec3<char>& v);