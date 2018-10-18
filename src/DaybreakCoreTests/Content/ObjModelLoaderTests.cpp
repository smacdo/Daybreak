#include "stdafx.h"
#include "CppUnitTest.h"
#include "Content/ObjModel/ObjModelLoader.h"
#include "TestHelpers.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Daybreak;

namespace Microsoft::VisualStudio::CppUnitTestFramework
{
    std::wstring ToString(const obj_face_t& v)
    {
        std::wstringstream ss;
        ss << L"{"
            << " {" << v.position[0] << L", " << v.position[1] << L", " << v.position[2] << L"}, "
            << " {" << v.uv[0]       << L", " << v.uv[1]       << L", " << v.uv[2]       << L"}, "
            << " {" << v.normal[0]   << L", " << v.normal[1]   << L", " << v.normal[2]   << L"} "
            << "}";
        return ss.str();
    }

    std::wstring ToString(const obj_face_vertex_t& v)
    {
        std::wstringstream ss;
        ss << L"{" << v.p << L", " << v.t << L", " << v.n << L"}";
        return ss.str();
    }
}

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

    TEST_METHOD(F_P_Only_Add_Face)
    {
        ObjModelParser parser;

        const char * ObjData = "v 10 20 30\nv 11 21 31\nv 12 22 32\nf 1 2 3\nf 3 2 1";
        auto model = parser.parse(ObjData);

        Assert::AreEqual(2, (int)model->groups[0].faces.size());
        Assert::AreEqual({ {1, 2, 3}, {0, 0, 0}, {0, 0, 0} }, model->groups[0].faces[0]);
        Assert::AreEqual({ {3, 2, 1}, {0, 0, 0}, {0, 0, 0} }, model->groups[0].faces[1]);
    }

    TEST_METHOD(F_PT_Adds_Face)
    {
        ObjModelParser parser;

        const char * ObjData =
            "v 10 20 30\n vt 0.2 0.4\n vn 0.1 0.2 0.3\n"
            "v 11 21 31\n vt 0.3 0.5\n vn 0.4 0.5 0.6\n"
            "v 12 22 32\n vt 0.4 0.5\n vn 0.7 0.8 0.9\n"
            "f 1/2 2/1 3/3\n f 3/3 2/1 1/2";

        auto model = parser.parse(ObjData);

        Assert::AreEqual(2, (int)model->groups[0].faces.size());
        Assert::AreEqual({ {1, 2, 3}, {2, 1, 3}, {0, 0, 0} }, model->groups[0].faces[0]);
        Assert::AreEqual({ {3, 2, 1}, {3, 1, 2}, {0, 0, 0} }, model->groups[0].faces[1]);
    }

    TEST_METHOD(F_Missing_One_Or_Two_Face_Elements_Throws_Exception)
    {
        ObjModelParser parser;

        const char * ObjData =
            "v 10 20 30\n vt 0.2 0.4\n vn 0.1 0.2 0.3\n"
            "v 11 21 31\n vt 0.3 0.5\n vn 0.4 0.5 0.6\n"
            "v 12 22 32\n vt 0.4 0.5\n vn 0.7 0.8 0.9\n"
            "f 1/2 2/1 3/3\n f 3/3 2/1 1/2";

        auto model = parser.parse(ObjData);

        Assert::AreEqual(2, (int)model->groups[0].faces.size());
        Assert::AreEqual({ {1, 2, 3}, {2, 1, 3}, {0, 0, 0} }, model->groups[0].faces[0]);
        Assert::AreEqual({ {3, 2, 1}, {3, 1, 2}, {0, 0, 0} }, model->groups[0].faces[1]);
    }

    TEST_METHOD(F_PTN_Adds_Face)
    {
        ObjModelParser parser;

        const char * ObjData =
            "v 10 20 30\n vt 0.2 0.4\n vn 0.1 0.2 0.3\n"
            "v 11 21 31\n vt 0.3 0.5\n vn 0.4 0.5 0.6\n"
            "v 12 22 32\n vt 0.4 0.5\n vn 0.7 0.8 0.9\n"
            "f 1/2/3 2/1/1 3/3/2\n f 3/3/1 2/1/3 1/2/2";

        auto model = parser.parse(ObjData);

        Assert::AreEqual(2, (int)model->groups[0].faces.size());
        Assert::AreEqual({ {1, 2, 3}, {2, 1, 3}, {3, 1, 2} }, model->groups[0].faces[0]);
        Assert::AreEqual({ {3, 2, 1}, {3, 1, 2}, {1, 3, 2} }, model->groups[0].faces[1]);
    }

    TEST_METHOD(F_PN_Adds_Face)
    {
        ObjModelParser parser;

        const char * ObjData =
            "v 10 20 30\n vt 0.2 0.4\n vn 0.1 0.2 0.3\n"
            "v 11 21 31\n vt 0.3 0.5\n vn 0.4 0.5 0.6\n"
            "v 12 22 32\n vt 0.4 0.5\n vn 0.7 0.8 0.9\n"
            "f 1//3 2//1 3//2\n f 3//1 2//3 1//2";

        auto model = parser.parse(ObjData);

        Assert::AreEqual(2, (int)model->groups[0].faces.size());
        Assert::AreEqual({ {1, 2, 3}, {0, 0, 0}, {3, 1, 2} }, model->groups[0].faces[0]);
        Assert::AreEqual({ {3, 2, 1}, {0, 0, 0}, {1, 3, 2} }, model->groups[0].faces[1]);
    }

    TEST_METHOD(F_Adds_Faces_To_Current_Group)
    {
        ObjModelParser parser;

        const char * ObjData =
            "v 10 20 30\n vt 0.2 0.4\n vn 0.1 0.2 0.3\n"
            "v 11 21 31\n vt 0.3 0.5\n vn 0.4 0.5 0.6\n"
            "v 12 22 32\n vt 0.4 0.5\n vn 0.7 0.8 0.9\n"
            "f 1//3 2//1 3//2\ng group\n f 3//1 2//3 1//2";

        auto model = parser.parse(ObjData);

        Assert::AreEqual(1, (int)model->groups[0].faces.size());
        Assert::AreEqual({ {1, 2, 3}, {0, 0, 0}, {3, 1, 2} }, model->groups[0].faces[0]);

        Assert::AreEqual(1, (int)model->groups[1].faces.size());
        Assert::AreEqual({ {3, 2, 1}, {0, 0, 0}, {1, 3, 2} }, model->groups[1].faces[0]);
    }

    TEST_METHOD(G_Creates_A_New_Group)
    {
        ObjModelParser parser;

        const char * ObjData =
            "v 10 20 30\n vt 0.2 0.4\n vn 0.1 0.2 0.3\n"
            "v 11 21 31\n vt 0.3 0.5\n vn 0.4 0.5 0.6\n"
            "v 12 22 32\n vt 0.4 0.5\n vn 0.7 0.8 0.9\n"
            "f 1//3 2//1 3//2\ng first\n f 3//1 2//3 1//2\ng second";

        auto model = parser.parse(ObjData);

        Assert::AreEqual(3, (int)model->groups.size());
        Assert::AreEqual(ObjModelParser::DefaultGroupName, model->groups[0].name);
        Assert::AreEqual(std::string("first"), model->groups[1].name);
        Assert::AreEqual(std::string("second"), model->groups[2].name);
    }

    TEST_METHOD(MTLLIB_Adds_Material_Library)
    {
        ObjModelParser parser;

        const char * ObjData = "mtllib foobar.mtl\nmtllib foo.mtl \n mtllib blah.mtl";
        auto model = parser.parse(ObjData);
       
        Assert::AreEqual(3, (int)model->materialLibraries.size());
        Assert::AreEqual("foobar.mtl", model->materialLibraries[0].c_str());
        Assert::AreEqual("foo.mtl", model->materialLibraries[1].c_str());
        Assert::AreEqual("blah.mtl", model->materialLibraries[2].c_str());
    }
};
