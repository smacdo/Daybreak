#include "stdafx.h"
#include "CppUnitTest.h"
#include "Resources/Obj/ObjModelLoader.h"
#include "TestHelpers.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Daybreak;

TEST_CLASS(ObjModelLoaderTests)
{
public:
    TEST_METHOD(V_Adds_Vertex_Positions)
    {
        ObjModelParser parser;

        const char * ObjData = "v 5 2 3\nv 1.2 2.0 2.5";
        auto model = parser.parse(ObjData);

        Assert::AreEqual(2, (int)model->positions.size());
        Assert::AreEqual(glm::vec3(5, 2, 3), model->positions[0]);
        Assert::AreEqual(glm::vec3(1.2, 2, 2.5), model->positions[1]);
    }

    TEST_METHOD(V_Throws_Exception_If_X_Y_Z_Field_Missing)
    {
        ObjModelParser parser;

        Assert::ExpectException<std::runtime_error>([] { ObjModelParser p; p.parse("v"); });
        Assert::ExpectException<std::runtime_error>([] { ObjModelParser p; p.parse("v 1"); });
        Assert::ExpectException<std::runtime_error>([] { ObjModelParser p; p.parse("v 1 2"); });
    }

    TEST_METHOD(V_Throws_Exception_If_X_Y_Z_Field_Invalid)
    {
        ObjModelParser parser;

        Assert::ExpectException<std::runtime_error>([] { ObjModelParser p; p.parse("v a 2 3"); });
        Assert::ExpectException<std::runtime_error>([] { ObjModelParser p; p.parse("v 1 x 2"); });
        Assert::ExpectException<std::runtime_error>([] { ObjModelParser p; p.parse("v 1 2 z"); });
    }

    TEST_METHOD(VT_Adds_Vertex_UV)
    {
        ObjModelParser parser;

        const char * ObjData = "vt 0.2 0.4\nvt 0.5 0.1";
        auto model = parser.parse(ObjData);

        Assert::AreEqual(2, (int)model->uv.size());
        Assert::AreEqual(glm::vec2(0.2f, 0.4f), model->uv[0]);
        Assert::AreEqual(glm::vec2(0.5f, 0.1f), model->uv[1]);
    }

    TEST_METHOD(VT_Throws_Exception_If_S_T_Field_Missing)
    {
        ObjModelParser parser;

        Assert::ExpectException<std::runtime_error>([] { ObjModelParser p; p.parse("vt"); });
        Assert::ExpectException<std::runtime_error>([] { ObjModelParser p; p.parse("vt 1"); });
    }

    TEST_METHOD(VT_Throws_Exception_If_S_T_Field_Invalid)
    {
        ObjModelParser parser;

        Assert::ExpectException<std::runtime_error>([] { ObjModelParser p; p.parse("vt a 1"); });
        Assert::ExpectException<std::runtime_error>([] { ObjModelParser p; p.parse("vt 0 b"); });
    }

    TEST_METHOD(VN_Adds_Vertex_Normal)
    {
        ObjModelParser parser;

        const char * ObjData = "vn 2 6.2 4\nvn 2.2 5 2.5";
        auto model = parser.parse(ObjData);

        Assert::AreEqual(2, (int)model->normals.size());
        Assert::AreEqual(glm::vec3(2, 6.2, 4), model->normals[0]);
        Assert::AreEqual(glm::vec3(2.2, 5, 2.5), model->normals[1]);
    }

    TEST_METHOD(VN_Throws_Exception_If_X_Y_Z_Field_Missing)
    {
        ObjModelParser parser;

        Assert::ExpectException<std::runtime_error>([] { ObjModelParser p; p.parse("vn"); });
        Assert::ExpectException<std::runtime_error>([] { ObjModelParser p; p.parse("vn 1"); });
        Assert::ExpectException<std::runtime_error>([] { ObjModelParser p; p.parse("vn 1 2"); });
    }

    TEST_METHOD(VN_Throws_Exception_If_X_Y_Z_Field_Invalid)
    {
        ObjModelParser parser;

        Assert::ExpectException<std::runtime_error>([] { ObjModelParser p; p.parse("vn a 2 3"); });
        Assert::ExpectException<std::runtime_error>([] { ObjModelParser p; p.parse("vn 1 x 2"); });
        Assert::ExpectException<std::runtime_error>([] { ObjModelParser p; p.parse("vn 1 2 z"); });
    }
};
