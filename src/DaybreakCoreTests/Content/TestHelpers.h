#pragma once
#include "CppUnitTest.h"
#include <glm/glm.hpp>
#include <sstream>

namespace Microsoft::VisualStudio::CppUnitTestFramework
{
    std::wstring ToString(const glm::vec2& v)
    {
        std::wstringstream ss;
        ss << L"(" << v.x << L", " << v.y << L")";
        return ss.str();
    }

    std::wstring ToString(const glm::vec3& v)
    {
        std::wstringstream ss;
        ss << L"(" << v.x << L", " << v.y << L", " << v.z << L")";
        return ss.str();
    }
}
