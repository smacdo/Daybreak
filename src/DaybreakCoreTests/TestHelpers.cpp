#include "stdafx.h"
#include "Content/ObjModel/ObjModelParser.h"

#include <ostream>
#include <string>

// Include this last to make sure we get the #undef CHECK macro in the right order.
#include "TestHelpers.h"

using namespace Daybreak;

std::ostream& operator <<(std::ostream& os, const glm::vec2& v)
{
    os << L"(" << v.x << L", " << v.y << L")";
    return os;
}

std::ostream& operator <<(std::ostream& os, const glm::vec3& v)
{
    os << L"(" << v.x << L", " << v.y << L", " << v.z << L")";
    return os;;
}

std::ostream& operator <<(std::ostream& os, const obj_face_t& v)
{
    os << L"{"
        << " {" << v.position[0] << L", " << v.position[1] << L", " << v.position[2] << L"}, "
        << " {" << v.uv[0] << L", " << v.uv[1] << L", " << v.uv[2] << L"}, "
        << " {" << v.normal[0] << L", " << v.normal[1] << L", " << v.normal[2] << L"} "
        << "}";
    return os;
}

std::ostream& operator <<(std::ostream& os, const obj_face_vertex_t& v)
{
    os << L"{" << v.p << L", " << v.t << L", " << v.n << L"}";
    return os;
}
