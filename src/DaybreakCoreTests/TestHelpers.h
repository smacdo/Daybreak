#pragma once
#undef CHECK

#include <catch2\catch.hpp>
#include <glm/glm.hpp>

namespace Daybreak
{
    struct obj_face_t;
    struct obj_face_vertex_t;
}

// TODO: FIX FOR CATCH
namespace Microsoft::VisualStudio::CppUnitTestFramework
{
    std::wstring ToString(const glm::vec2& v);
    std::wstring ToString(const glm::vec3& v);

    std::wstring ToString(const Daybreak::obj_face_t& v);
    std::wstring ToString(const Daybreak::obj_face_vertex_t& v);
}
