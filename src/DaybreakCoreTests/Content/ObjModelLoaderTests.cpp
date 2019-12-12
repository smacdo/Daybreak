#include "stdafx.h"
#include "Content/ObjModel/ObjModelParser.h"
#include "Content/ObjModel/ObjModelException.h"

#include "../TestHelpers.h"

using namespace Daybreak;

TEST_CASE("V_Adds_Vertex_Positions", "[content][ObjMaterialParser]")
{
    ObjModelParser parser;

    const char * ObjData = "v 5 2 3\nv 1.2 2.0 2.5";
    auto model = parser.parse(ObjData);

    REQUIRE(2 == (int)model->positions.size());
    REQUIRE(glm::vec3(5, 2, 3) == model->positions[0]);
    REQUIRE(glm::vec3(1.2, 2, 2.5) == model->positions[1]);
}

TEST_CASE("V_Throws_Exception_If_X_Y_Z_Field_Missing", "[content][ObjMaterialParser]")
{
    // TODO: Test when there are more than three floats.
    REQUIRE_THROWS_MATCHES(
        [] { ObjModelParser p; p.parse("v"); }(),
        std::runtime_error,
        ContainsExceptionMessage<std::runtime_error>("Expected next token to be a float but no token was found"));

    REQUIRE_THROWS_MATCHES(
        [] { ObjModelParser p; p.parse("v 1"); }(),
        std::runtime_error,
        ContainsExceptionMessage<std::runtime_error>("Expected next token to be a float but no token was found"));

    REQUIRE_THROWS_MATCHES(
        [] { ObjModelParser p; p.parse("v 3 2"); }(),
        std::runtime_error,
        ContainsExceptionMessage<std::runtime_error>("Expected next token to be a float but no token was found"));
}

TEST_CASE("V_Throws_Exception_If_X_Y_Z_Field_Invalid", "[content][ObjMaterialParser]")
{
    REQUIRE_THROWS_MATCHES(
        [] { ObjModelParser p; p.parse("v a"); }(),
        std::runtime_error,
        ContainsExceptionMessage<std::runtime_error>("Could not parse string as float"));

    REQUIRE_THROWS_MATCHES(
        [] { ObjModelParser p; p.parse("v 1 x"); }(),
        std::runtime_error,
        ContainsExceptionMessage<std::runtime_error>("Could not parse string as float"));

    REQUIRE_THROWS_MATCHES(
        [] { ObjModelParser p; p.parse("v 1 2 z"); }(),
        std::runtime_error,
        ContainsExceptionMessage<std::runtime_error>("Could not parse string as float"));
}

TEST_CASE("VT_Adds_Vertex_UV", "[content][ObjMaterialParser]")
{
    ObjModelParser parser;

    const char * ObjData = "vt 0.2 0.4\nvt 0.5 0.1";
    auto model = parser.parse(ObjData);

    REQUIRE(2 == (int)model->uv.size());
    REQUIRE(glm::vec2(0.2f, 0.4f) == model->uv[0]);
    REQUIRE(glm::vec2(0.5f, 0.1f) == model->uv[1]);
}

TEST_CASE("VT_Throws_Exception_If_S_T_Field_Missing", "[content][ObjMaterialParser]")
{
    // TODO: Test when there are more than two floats.
    REQUIRE_THROWS_MATCHES(
        [] { ObjModelParser p; p.parse("vt"); }(),
        std::runtime_error,
        ContainsExceptionMessage<std::runtime_error>("Expected next token to be a float but no token was found"));

    REQUIRE_THROWS_MATCHES(
        [] { ObjModelParser p; p.parse("vt 0"); }(),
        std::runtime_error,
        ContainsExceptionMessage<std::runtime_error>("Expected next token to be a float but no token was found"));

    REQUIRE_THROWS_MATCHES(
        [] { ObjModelParser p; p.parse("vt 0.2"); }(),
        std::runtime_error,
        ContainsExceptionMessage<std::runtime_error>("Expected next token to be a float but no token was found"));
}

TEST_CASE("VT_Throws_Exception_If_S_T_Field_Invalid", "[content][ObjMaterialParser]")
{
    REQUIRE_THROWS_MATCHES(
        [] { ObjModelParser p; p.parse("vt h 0.4"); }(),
        std::runtime_error,
        ContainsExceptionMessage<std::runtime_error>("Could not parse string as float"));

    REQUIRE_THROWS_MATCHES(
        [] { ObjModelParser p; p.parse("vt 0 j"); }(),
        std::runtime_error,
        ContainsExceptionMessage<std::runtime_error>("Could not parse string as float"));
}

TEST_CASE("VN_Adds_Vertex_Normal", "[content][ObjMaterialParser]")
{
    ObjModelParser parser;

    const char * ObjData = "vn 2 6.2 4\nvn 2.2 5 2.5";
    auto model = parser.parse(ObjData);

    REQUIRE(2 == (int)model->normals.size());
    REQUIRE(glm::vec3(2, 6.2, 4) == model->normals[0]);
    REQUIRE(glm::vec3(2.2, 5, 2.5) == model->normals[1]);
}

TEST_CASE("VN_Throws_Exception_If_X_Y_Z_Field_Missing", "[content][ObjMaterialParser]")
{
    // TODO: Test when there are more than three floats.
    REQUIRE_THROWS_MATCHES(
        [] { ObjModelParser p; p.parse("vn"); }(),
        std::runtime_error,
        ContainsExceptionMessage<std::runtime_error>("Expected next token to be a float but no token was found"));

    REQUIRE_THROWS_MATCHES(
        [] { ObjModelParser p; p.parse("vn 4"); }(),
        std::runtime_error,
        ContainsExceptionMessage<std::runtime_error>("Expected next token to be a float but no token was found"));

    REQUIRE_THROWS_MATCHES(
        [] { ObjModelParser p; p.parse("vn 1 7"); }(),
        std::runtime_error,
        ContainsExceptionMessage<std::runtime_error>("Expected next token to be a float but no token was found"));
}

TEST_CASE("VN_Throws_Exception_If_X_Y_Z_Field_Invalid", "[content][ObjMaterialParser]")
{
    REQUIRE_THROWS_MATCHES(
        [] { ObjModelParser p; p.parse("vn e"); }(),
        std::runtime_error,
        ContainsExceptionMessage<std::runtime_error>("Could not parse string as float"));

    REQUIRE_THROWS_MATCHES(
        [] { ObjModelParser p; p.parse("vn 3 r"); }(),
        std::runtime_error,
        ContainsExceptionMessage<std::runtime_error>("Could not parse string as float"));

    REQUIRE_THROWS_MATCHES(
        [] { ObjModelParser p; p.parse("vn 1.2 2 i"); }(),
        std::runtime_error,
        ContainsExceptionMessage<std::runtime_error>("Could not parse string as float"));
}

TEST_CASE("F_P_Only_Add_Face", "[content][ObjMaterialParser]")
{
    ObjModelParser parser;

    const char * ObjData = "v 10 20 30\nv 11 21 31\nv 12 22 32\nf 1 2 3\nf 3 2 1";
    auto model = parser.parse(ObjData);

    REQUIRE(2 == (int)model->groups[0].faces.size());
    REQUIRE(obj_face_t{ {1, 2, 3}, {0, 0, 0}, {0, 0, 0} } == model->groups[0].faces[0]);
    REQUIRE(obj_face_t{ {3, 2, 1}, {0, 0, 0}, {0, 0, 0} } == model->groups[0].faces[1]);
}

TEST_CASE("F_PT_Adds_Face", "[content][ObjMaterialParser]")
{
    ObjModelParser parser;

    const char * ObjData =
        "v 10 20 30\n vt 0.2 0.4\n vn 0.1 0.2 0.3\n"
        "v 11 21 31\n vt 0.3 0.5\n vn 0.4 0.5 0.6\n"
        "v 12 22 32\n vt 0.4 0.5\n vn 0.7 0.8 0.9\n"
        "f 1/2 2/1 3/3\n f 3/3 2/1 1/2";

    auto model = parser.parse(ObjData);

    REQUIRE(2 == (int)model->groups[0].faces.size());
    REQUIRE(obj_face_t{ {1, 2, 3}, {2, 1, 3}, {0, 0, 0} } == model->groups[0].faces[0]);
    REQUIRE(obj_face_t{ {3, 2, 1}, {3, 1, 2}, {0, 0, 0} } == model->groups[0].faces[1]);
}

TEST_CASE("F_Missing_One_Or_Two_Face_Elements_Throws_Exception", "[content][ObjMaterialParser]")
{
    ObjModelParser parser;

    const char * ObjData =
        "v 10 20 30\n vt 0.2 0.4\n vn 0.1 0.2 0.3\n"
        "v 11 21 31\n vt 0.3 0.5\n vn 0.4 0.5 0.6\n"
        "v 12 22 32\n vt 0.4 0.5\n vn 0.7 0.8 0.9\n"
        "f 1/2 2/1 3/3\n f 3/3 2/1 1/2";

    auto model = parser.parse(ObjData);

    REQUIRE(2 == (int)model->groups[0].faces.size());
    REQUIRE(obj_face_t{ {1, 2, 3}, {2, 1, 3}, {0, 0, 0} } == model->groups[0].faces[0]);
    REQUIRE(obj_face_t{ {3, 2, 1}, {3, 1, 2}, {0, 0, 0} } == model->groups[0].faces[1]);
}

TEST_CASE("F_PTN_Adds_Face", "[content][ObjMaterialParser]")
{
    ObjModelParser parser;

    const char * ObjData =
        "v 10 20 30\n vt 0.2 0.4\n vn 0.1 0.2 0.3\n"
        "v 11 21 31\n vt 0.3 0.5\n vn 0.4 0.5 0.6\n"
        "v 12 22 32\n vt 0.4 0.5\n vn 0.7 0.8 0.9\n"
        "f 1/2/3 2/1/1 3/3/2\n f 3/3/1 2/1/3 1/2/2";

    auto model = parser.parse(ObjData);

    REQUIRE(2 == (int)model->groups[0].faces.size());
    REQUIRE(obj_face_t { {1, 2, 3}, {2, 1, 3}, {3, 1, 2} } == model->groups[0].faces[0]);
    REQUIRE(obj_face_t { {3, 2, 1}, {3, 1, 2}, {1, 3, 2} } == model->groups[0].faces[1]);
}

TEST_CASE("F_PN_Adds_Face", "[content][ObjMaterialParser]")
{
    ObjModelParser parser;

    const char * ObjData =
        "v 10 20 30\n vt 0.2 0.4\n vn 0.1 0.2 0.3\n"
        "v 11 21 31\n vt 0.3 0.5\n vn 0.4 0.5 0.6\n"
        "v 12 22 32\n vt 0.4 0.5\n vn 0.7 0.8 0.9\n"
        "f 1//3 2//1 3//2\n f 3//1 2//3 1//2";

    auto model = parser.parse(ObjData);

    REQUIRE(2 == (int)model->groups[0].faces.size());
    REQUIRE(obj_face_t{ {1, 2, 3}, {0, 0, 0}, {3, 1, 2} } == model->groups[0].faces[0]);
    REQUIRE(obj_face_t{ {3, 2, 1}, {0, 0, 0}, {1, 3, 2} } == model->groups[0].faces[1]);
}

TEST_CASE("F_Supports_Negative_Indices", "[content][ObjMaterialParser]")
{
    ObjModelParser parser;

    const char* ObjData =
        "v 10 20 30\n vt 0.2 0.4\n vn 0.1 0.2 0.3\n"
        "v 11 21 31\n vt 0.3 0.5\n vn 0.4 0.5 0.6\n"
        "v 12 22 32\n vt 0.4 0.5\n vn 0.7 0.8 0.9\n"
        "f -2/-1/-3 -2/-2/1 3/-3/-1\n";

    auto model = parser.parse(ObjData);

    REQUIRE(1 == (int)model->groups[0].faces.size());
    REQUIRE(obj_face_t{ {2, 2, 3}, {3, 2, 1}, {1, 1, 3} } == model->groups[0].faces[0]);
}

TEST_CASE("F_Throws_Exception_If_Any_Index_Is_Zero", "[content][ObjMaterialParser]")
{
    const std::string& o =
        "v 10 20 30\n vt 0.2 0.4\n vn 0.1 0.2 0.3\n"
        "v 11 21 31\n vt 0.3 0.5\n vn 0.4 0.5 0.6\n"
        "v 12 22 32\n vt 0.4 0.5\n vn 0.7 0.8 0.9\n";

    REQUIRE_THROWS_MATCHES(
        [o] { ObjModelParser p; p.parse(o + "f 0 0 0"); }(),
        ObjModelException,
        ContainsExceptionMessage<ObjModelException>("Invalid index of zero"));

    REQUIRE_THROWS_MATCHES(
        [o] { ObjModelParser p; p.parse(o + "f 1/0 1/0 1/0"); }(),
        ObjModelException,
        ContainsExceptionMessage<ObjModelException>("Invalid index of zero"));

    REQUIRE_THROWS_MATCHES(
        [o] { ObjModelParser p; p.parse(o + "f 1/1/0 1/1/0 1/1/0"); }(),
        ObjModelException,
        ContainsExceptionMessage<ObjModelException>("Invalid index of zero"));

    REQUIRE_THROWS_MATCHES(
        [o] { ObjModelParser p; p.parse(o + "f 1//0 1//0 1//0"); }(),
        ObjModelException,
        ContainsExceptionMessage<ObjModelException>("Invalid index of zero"));
}

TEST_CASE("F_Adds_Faces_To_Current_Group", "[content][ObjMaterialParser]")
{
    ObjModelParser parser;

    const char * ObjData =
        "v 10 20 30\n vt 0.2 0.4\n vn 0.1 0.2 0.3\n"
        "v 11 21 31\n vt 0.3 0.5\n vn 0.4 0.5 0.6\n"
        "v 12 22 32\n vt 0.4 0.5\n vn 0.7 0.8 0.9\n"
        "f 1//3 2//1 3//2\ng group\n f 3//1 2//3 1//2";

    auto model = parser.parse(ObjData);

    REQUIRE(1 == (int)model->groups[0].faces.size());
    REQUIRE(obj_face_t{ {1, 2, 3}, {0, 0, 0}, {3, 1, 2} } == model->groups[0].faces[0]);

    REQUIRE(1 == (int)model->groups[1].faces.size());
    REQUIRE(obj_face_t{ {3, 2, 1}, {0, 0, 0}, {1, 3, 2} } == model->groups[1].faces[0]);
}

TEST_CASE("F_Throws_Exception_If_Any_Of_Three_Elements_Missing", "[content][ObjMaterialParser]")
{
    ObjModelParser p;
    const std::string& o =
        "v 10 20 30\n vt 0.2 0.4\n vn 0.1 0.2 0.3\n"
        "v 11 21 31\n vt 0.3 0.5\n vn 0.4 0.5 0.6\n"
        "v 12 22 32\n vt 0.4 0.5\n vn 0.7 0.8 0.9\n";

    // TODO: These exceptions are not clear, fix that.
    // TODO: Test when there are more than three elements.
    REQUIRE_THROWS_MATCHES(
        [o] { ObjModelParser p; p.parse(o + "f"); }(),
        ObjModelException,
        ContainsExceptionMessage<ObjModelException>("Missing face element"));

    REQUIRE_THROWS_MATCHES(
        [o] { ObjModelParser p; p.parse(o + "f 1"); }(),
        ObjModelException,
        ContainsExceptionMessage<ObjModelException>("Missing face element"));

    REQUIRE_THROWS_MATCHES(
        [o] { ObjModelParser p; p.parse(o + "f 1 2"); }(),
        ObjModelException,
        ContainsExceptionMessage<ObjModelException>("Missing face element"));
}

TEST_CASE("F_Throws_Exception_If_Index_Out_Of_Range", "[content][ObjMaterialParser]")
{
    ObjModelParser p;
    const std::string& o =
        "v 10 20 30\n vt 0.2 0.4\n vn 0.1 0.2 0.3\n"
        "v 11 21 31\n vt 0.3 0.5\n vn 0.4 0.5 0.6\n"
        "v 12 22 32\n vt 0.4 0.5\n vn 0.7 0.8 0.9\n";

    REQUIRE_THROWS_MATCHES(
        [o] { ObjModelParser p; p.parse(o + "f 4 2 3"); }(),
        ObjModelException,
        ContainsExceptionMessage<ObjModelException>("Index must be smaller than size of data array"));

    REQUIRE_THROWS_MATCHES(
        [o] { ObjModelParser p; p.parse(o + "f 1 4 3"); }(),
        ObjModelException,
        ContainsExceptionMessage<ObjModelException>("Index must be smaller than size of data array"));

    REQUIRE_THROWS_MATCHES(
        [o] { ObjModelParser p; p.parse(o + "f 1 2 4"); }(),
        ObjModelException,
        ContainsExceptionMessage<ObjModelException>("Index must be smaller than size of data array"));

    REQUIRE_THROWS_MATCHES(
        [o] { ObjModelParser p; p.parse(o + "f 4/1/2 2/1/3 3/1/2"); }(),
        ObjModelException,
        ContainsExceptionMessage<ObjModelException>("Index must be smaller than size of data array"));

    REQUIRE_THROWS_MATCHES(
        [o] { ObjModelParser p; p.parse(o + "f 1/2/3 1/4/3 3/2/1"); }(),
        ObjModelException,
        ContainsExceptionMessage<ObjModelException>("Index must be smaller than size of data array"));

    REQUIRE_THROWS_MATCHES(
        [o] { ObjModelParser p; p.parse(o + "f 1/3/2 2/3/1 1/3/4"); }(),
        ObjModelException,
        ContainsExceptionMessage<ObjModelException>("Index must be smaller than size of data array"));
}

TEST_CASE("F_Throws_Exception_Face_Data_Layout_Different", "[content][ObjMaterialParser]")
{
    ObjModelParser p;
    const std::string& o =
        "v 10 20 30\n vt 0.2 0.4\n vn 0.1 0.2 0.3\n"
        "v 11 21 31\n vt 0.3 0.5\n vn 0.4 0.5 0.6\n"
        "v 12 22 32\n vt 0.4 0.5\n vn 0.7 0.8 0.9\n";

    REQUIRE_THROWS_MATCHES(
        [o] { ObjModelParser p; p.parse(o + "f 1/2/3 1/2 1/2"); }(),
        ObjModelException,
        ContainsExceptionMessage<ObjModelException>("Face elements have inconsistent data layout"));

    REQUIRE_THROWS_MATCHES(
        [o] { ObjModelParser p; p.parse(o + "f 1/2/3 1 2"); }(),
        ObjModelException,
        ContainsExceptionMessage<ObjModelException>("Face elements have inconsistent data layout"));

    REQUIRE_THROWS_MATCHES(
        [o] { ObjModelParser p; p.parse(o + "f 1/2/3 1//2 2//3"); }(),
        ObjModelException,
        ContainsExceptionMessage<ObjModelException>("Face elements have inconsistent data layout"));

    REQUIRE_THROWS_MATCHES(
        [o] { ObjModelParser p; p.parse(o + "f 1 1//2 2//3"); }(),
        ObjModelException,
        ContainsExceptionMessage<ObjModelException>("Face elements have inconsistent data layout"));

    REQUIRE_THROWS_MATCHES(
        [o] { ObjModelParser p; p.parse(o + "f 1 2 3\nf 1 2 1/3"); }(),
        ObjModelException,
        ContainsExceptionMessage<ObjModelException>("Face elements have inconsistent data layout"));
}

TEST_CASE("G_Creates_A_New_Group", "[content][ObjMaterialParser]")
{
    ObjModelParser parser;

    const char * ObjData =
        "v 10 20 30\n vt 0.2 0.4\n vn 0.1 0.2 0.3\n"
        "v 11 21 31\n vt 0.3 0.5\n vn 0.4 0.5 0.6\n"
        "v 12 22 32\n vt 0.4 0.5\n vn 0.7 0.8 0.9\n"
        "f 1//3 2//1 3//2\ng first\n f 3//1 2//3 1//2\ng second";

    auto model = parser.parse(ObjData);

    REQUIRE(3 == (int)model->groups.size());
    REQUIRE(ObjModelParser::DefaultGroupName == model->groups[0].name);
    REQUIRE(std::string("first") == model->groups[1].name);
    REQUIRE(std::string("second") == model->groups[2].name);
}

TEST_CASE("O_Creates_A_New_Group", "[content][ObjMaterialParser]")
{
    ObjModelParser parser;

    const char * ObjData =
        "v 10 20 30\n vt 0.2 0.4\n vn 0.1 0.2 0.3\n"
        "v 11 21 31\n vt 0.3 0.5\n vn 0.4 0.5 0.6\n"
        "v 12 22 32\n vt 0.4 0.5\n vn 0.7 0.8 0.9\n"
        "f 1//3 2//1 3//2\no first\n f 3//1 2//3 1//2\no second";

    auto model = parser.parse(ObjData);

    REQUIRE(3 == (int)model->groups.size());
    REQUIRE(ObjModelParser::DefaultGroupName == model->groups[0].name);
    REQUIRE(std::string("first") == model->groups[1].name);
    REQUIRE(std::string("second") == model->groups[2].name);
}

TEST_CASE("MTLLIB_Adds_Material_Library", "[content][ObjMaterialParser]")
{
    ObjModelParser parser;

    const char * ObjData = "mtllib foobar.mtl\nmtllib foo.mtl \n mtllib blah.mtl";
    auto model = parser.parse(ObjData);
       
    REQUIRE(3 == (int)model->materialLibraries.size());
    REQUIRE("foobar.mtl" == model->materialLibraries[0]);
    REQUIRE("foo.mtl" == model->materialLibraries[1]);
    REQUIRE("blah.mtl" == model->materialLibraries[2]);
}

TEST_CASE("USEMTL_Sets_Material_On_Current_Group", "[content][ObjMaterialParser]")
{
    ObjModelParser parser;

    const char * ObjData =
        "v 10 20 30\n vt 0.2 0.4\n vn 0.1 0.2 0.3\n"
        "v 11 21 31\n vt 0.3 0.5\n vn 0.4 0.5 0.6\n"
        "v 12 22 32\n vt 0.4 0.5\n vn 0.7 0.8 0.9\n"
        "usemtl foo\nf 1//3 2//1 3//2\n"
        "g first\nusemtl bar\nf 3//1 2//3 1//2\n"
        "g second\nusemtl foobar\nf 1//2 3//1 2//3\n";

    auto model = parser.parse(ObjData);

    REQUIRE(3 == (int)model->groups.size());
    REQUIRE(std::string("foo") == model->groups[0].material);
    REQUIRE(std::string("bar") == model->groups[1].material);
    REQUIRE(std::string("foobar") == model->groups[2].material);
}

TEST_CASE("USEMTL_Starts_New_Group_With_Same_Name_If_Existing_Faces_Have_Different_Material", "[content][ObjMaterialParser]")
{
    ObjModelParser parser;

    const char * ObjData =
        "v 10 20 30\n vt 0.2 0.4\n vn 0.1 0.2 0.3\n"
        "v 11 21 31\n vt 0.3 0.5\n vn 0.4 0.5 0.6\n"
        "v 12 22 32\n vt 0.4 0.5\n vn 0.7 0.8 0.9\n"
        "g first\nusemtl mat1\nf 3//1 2//3 1//2\n"
        "usemtl mat2\nf 1//2 3//1 2//3\n";

    auto model = parser.parse(ObjData);

    REQUIRE(2 == (int)model->groups.size());

    REQUIRE(std::string("first") == model->groups[0].name);
    REQUIRE(std::string("mat1") == model->groups[0].material);
    REQUIRE(std::string("first") == model->groups[1].name);
    REQUIRE(std::string("mat2") == model->groups[1].material);
}

TEST_CASE("USEMTL_Replaces_Material_If_No_Existing_Faces_In_Current_Group", "[content][ObjMaterialParser]")
{
    ObjModelParser parser;

    const char * ObjData =
        "v 10 20 30\n vt 0.2 0.4\n vn 0.1 0.2 0.3\n"
        "v 11 21 31\n vt 0.3 0.5\n vn 0.4 0.5 0.6\n"
        "v 12 22 32\n vt 0.4 0.5\n vn 0.7 0.8 0.9\n"
        "g first\n"
        "usemtl mat1\n"
        "usemtl mat2\n";

    auto model = parser.parse(ObjData);

    REQUIRE(1 == (int)model->groups.size());
    REQUIRE(std::string("mat2") == model->groups[0].material);
}
