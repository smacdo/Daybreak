#include "stdafx.h"
#include "Content/ObjModel/MtlMaterialParser.h"
#include "Content/ObjModel/MtlMaterialException.h"
#include "Content\Materials\MaterialData.h"

#include "../TestHelpers.h"

using namespace Daybreak;

TEST_CASE("NEWMTL_Creates_A_New_Material_Group", "[content][MtlMaterialParser]")
{
    MtlMaterialParser parser;

    const char* MtlData =
        "newmtl first\n"
        "newmtl second\n";
    auto materials = parser.parse(MtlData);

    REQUIRE(2 == (int)materials.size());
    REQUIRE("first" == materials[0]->name());
    REQUIRE("second" == materials[1]->name());
}

TEST_CASE("Throws_Exception_If_No_Material_Defined_But_Command_REQUIRE_One", "[content][MtlMaterialParser]")
{
    REQUIRE_THROWS_MATCHES(
        [] { MtlMaterialParser p; p.parse("Ka 1 2 3\n"); }(),
        MtlMaterialException,
        ContainsExceptionMessage<MtlMaterialException>("No material group defined"));
}

TEST_CASE("Ka_Adds_Ambient_Color", "[content][MtlMaterialParser]")
{
    MtlMaterialParser parser;

    const std::string MtlData =
        "newmtl first\n"
        "Ka 0.25 0.5 0.75\n"
        "newmtl second\n"
        "Ka 0.1 1.0 0.0\n";
    auto materials = parser.parse(MtlData);

    REQUIRE(glm::vec3(0.25f, 0.5f, 0.75f) == materials[0]->getVector3Parameter(MaterialParameterType::AmbientColor));
    REQUIRE(glm::vec3(0.1f, 1.0f, 0.0f) == materials[1]->getVector3Parameter(MaterialParameterType::AmbientColor));
}

TEST_CASE("Ka_Throws_Exception_If_R_G_B_Field_Missing", "[content][MtlMaterialParser]")
{
    const std::string M = "newmtl first\n";

    REQUIRE_THROWS_MATCHES(
        [M] { MtlMaterialParser p; p.parse(M + "Ka"); }(),
        std::runtime_error,
        ContainsExceptionMessage<std::runtime_error>("Expected next token to be a float but no token was found"));

    REQUIRE_THROWS_MATCHES(
        [M] { MtlMaterialParser p; p.parse(M + "Ka 0"); }(),
        std::runtime_error,
        ContainsExceptionMessage<std::runtime_error>("Expected next token to be a float but no token was found"));

    REQUIRE_THROWS_MATCHES(
        [M] { MtlMaterialParser p; p.parse(M + "Ka 0 1"); }(),
        std::runtime_error,
        ContainsExceptionMessage<std::runtime_error>("Expected next token to be a float but no token was found"));

    REQUIRE_THROWS_MATCHES(
        [M] { MtlMaterialParser p; p.parse(M + "Ka r g b"); }(),
        std::runtime_error,
        ContainsExceptionMessage<std::runtime_error>("Could not parse string as float"));
}

TEST_CASE("Kd_Adds_Diffuse_Color", "[content][MtlMaterialParser]")
{
    MtlMaterialParser parser;

    const std::string MtlData =
        "newmtl first\n"
        "Kd 0.4 0.72 0.1\n"
        "newmtl second\n"
        "Kd 0.3 0.22 0.9\n";
    auto materials = parser.parse(MtlData);

    REQUIRE(glm::vec3(0.4f, 0.72f, 0.1f) == materials[0]->getVector3Parameter(MaterialParameterType::DiffuseColor));
    REQUIRE(glm::vec3(0.3f, 0.22f, 0.9f) == materials[1]->getVector3Parameter(MaterialParameterType::DiffuseColor));
}

TEST_CASE("Kd_Throws_Exception_If_R_G_B_Field_Missing", "[content][MtlMaterialParser]")
{
    const std::string M = "newmtl first\n";

    REQUIRE_THROWS_MATCHES(
        [M] { MtlMaterialParser p; p.parse(M + "Kd"); }(),
        std::runtime_error,
        ContainsExceptionMessage<std::runtime_error>("Expected next token to be a float but no token was found"));

    REQUIRE_THROWS_MATCHES(
        [M] { MtlMaterialParser p; p.parse(M + "Kd 0.25"); }(),
        std::runtime_error,
        ContainsExceptionMessage<std::runtime_error>("Expected next token to be a float but no token was found"));

    REQUIRE_THROWS_MATCHES(
        [M] { MtlMaterialParser p; p.parse(M + "Kd 0.5 0.03"); }(),
        std::runtime_error,
        ContainsExceptionMessage<std::runtime_error>("Expected next token to be a float but no token was found"));

    REQUIRE_THROWS_MATCHES(
        [M] { MtlMaterialParser p; p.parse(M + "Kd r g b"); }(),
        std::runtime_error,
        ContainsExceptionMessage<std::runtime_error>("Could not parse string as float"));
}

TEST_CASE("map_Kd_Adds_Diffuse_Texture_Map", "[content][MtlMaterialParser]")
{
    MtlMaterialParser parser;

    const std::string MtlData =
        "newmtl first\n"
        "map_Kd map1.dds\n"
        "newmtl second\n"
        "map_Kd map2.dds\n";
    auto materials = parser.parse(MtlData);

    REQUIRE("map1.dds" == materials[0]->getTextureParameter(MaterialParameterType::DiffuseMap).filepath);
    REQUIRE("map2.dds" == materials[1]->getTextureParameter(MaterialParameterType::DiffuseMap).filepath);
}

TEST_CASE("map_Kd_Throws_Exception_If_Path_Missing", "[content][MtlMaterialParser]")
{
    const std::string M = "newmtl first\n";

    REQUIRE_THROWS_MATCHES(
        [M] { MtlMaterialParser p; p.parse(M + "map_Kd"); }(),
        std::runtime_error,
        ContainsExceptionMessage<std::runtime_error>("Expected next token to be a string but no token was found"));

    REQUIRE_THROWS_MATCHES(
        [M] { MtlMaterialParser p; p.parse(M + "map_Kd   "); }(),
        std::runtime_error,
        ContainsExceptionMessage<std::runtime_error>("Expected next token to be a string but no token was found"));
}

TEST_CASE("Ks_Adds_Specular_Color", "[content][MtlMaterialParser]")
{
    MtlMaterialParser parser;

    const std::string MtlData =
        "newmtl first\n"
        "Ks 0.1 0.2 0.3\n"
        "newmtl second\n"
        "Ks 0.0 0.5 1\n";
    auto materials = parser.parse(MtlData);

    REQUIRE(glm::vec3(0.1f, 0.2f, 0.3f) == materials[0]->getVector3Parameter(MaterialParameterType::SpecularColor));
    REQUIRE(glm::vec3(0.0f, 0.5f, 1.0f) == materials[1]->getVector3Parameter(MaterialParameterType::SpecularColor));
}

TEST_CASE("Ks_Throws_Exception_If_R_G_B_Field_Missing", "[content][MtlMaterialParser]")
{
    const std::string M = "newmtl first\n";

    REQUIRE_THROWS_MATCHES(
        [M] { MtlMaterialParser p; p.parse(M + "Ks"); }(),
        std::runtime_error,
        ContainsExceptionMessage<std::runtime_error>("Expected next token to be a float but no token was found"));

    REQUIRE_THROWS_MATCHES(
        [M] { MtlMaterialParser p; p.parse(M + "Ks 1"); }(),
        std::runtime_error,
        ContainsExceptionMessage<std::runtime_error>("Expected next token to be a float but no token was found"));

    REQUIRE_THROWS_MATCHES(
        [M] { MtlMaterialParser p; p.parse(M + "Ks .2 0"); }(),
        std::runtime_error,
        ContainsExceptionMessage<std::runtime_error>("Expected next token to be a float but no token was found"));

    REQUIRE_THROWS_MATCHES(
        [M] { MtlMaterialParser p; p.parse(M + "Ks g f t"); }(),
        std::runtime_error,
        ContainsExceptionMessage<std::runtime_error>("Could not parse string as float"));
}

TEST_CASE("map_Ks_Adds_Specular_Texture_Map", "[content][MtlMaterialParser]")
{
    MtlMaterialParser parser;

    const std::string MtlData =
        "newmtl first\n"
        "map_Ks map1.dds\n"
        "newmtl second\n"
        "map_Ks map2.dds\n";
    auto materials = parser.parse(MtlData);

    REQUIRE("map1.dds" == materials[0]->getTextureParameter(MaterialParameterType::SpecularMap).filepath);
    REQUIRE("map2.dds" == materials[1]->getTextureParameter(MaterialParameterType::SpecularMap).filepath);
}

TEST_CASE("map_Ks_Throws_Exception_If_Path_Missing", "[content][MtlMaterialParser]")
{
    const std::string M = "newmtl first\n";

    REQUIRE_THROWS_MATCHES(
        [M] { MtlMaterialParser p; p.parse(M + "map_Ks"); }(),
        std::runtime_error,
        ContainsExceptionMessage<std::runtime_error>("Expected next token to be a string but no token was found"));

    REQUIRE_THROWS_MATCHES(
        [M] { MtlMaterialParser p; p.parse(M + "map_Ks   "); }(),
        std::runtime_error,
        ContainsExceptionMessage<std::runtime_error>("Expected next token to be a string but no token was found"));
}

TEST_CASE("Ns_Adds_Specular_Power", "[content][MtlMaterialParser]")
{
    MtlMaterialParser parser;

    const std::string MtlData =
        "newmtl first\n"
        "Ns 2\n"
        "newmtl second\n"
        "Ns 1.73\n";
    auto materials = parser.parse(MtlData);

    REQUIRE(2.0f == materials[0]->getFloatParameter(MaterialParameterType::Shininess));
    REQUIRE(1.73f == materials[1]->getFloatParameter(MaterialParameterType::Shininess));
}

TEST_CASE("Ns_Throws_Exception_If_Value_Field_Missing", "[content][MtlMaterialParser]")
{
    const std::string M = "newmtl first\n";

    REQUIRE_THROWS_MATCHES(
        [M] { MtlMaterialParser p; p.parse(M + "Ns"); }(),
        std::runtime_error,
        ContainsExceptionMessage<std::runtime_error>("Expected next token to be a float but no token was found"));

    REQUIRE_THROWS_MATCHES(
        [M] { MtlMaterialParser p; p.parse(M + "Ns x"); }(),
        std::runtime_error,
        ContainsExceptionMessage<std::runtime_error>("Could not parse string as float"));
}

TEST_CASE("d_Adds_Opacity", "[content][MtlMaterialParser]")
{
    MtlMaterialParser parser;

    const std::string MtlData =
        "newmtl first\n"
        "d 0.4\n"
        "newmtl second\n"
        "d 0\n";
    auto materials = parser.parse(MtlData);

    REQUIRE(0.4f == materials[0]->getFloatParameter(MaterialParameterType::Opacity));
    REQUIRE(0.0f == materials[1]->getFloatParameter(MaterialParameterType::Opacity));
}

TEST_CASE("d_Throws_Exception_If_Value_Field_Mising", "[content][MtlMaterialParser]")
{
    const std::string M = "newmtl first\n";

    REQUIRE_THROWS_MATCHES(
        [M] { MtlMaterialParser p; p.parse(M + "d"); }(),
        std::runtime_error,
        ContainsExceptionMessage<std::runtime_error>("Expected next token to be a float but no token was found"));

    REQUIRE_THROWS_MATCHES(
        [M] { MtlMaterialParser p; p.parse(M + "d w"); }(),
        std::runtime_error,
        ContainsExceptionMessage<std::runtime_error>("Could not parse string as float"));
}

TEST_CASE("d_Throws_Exception_If_Value_Out_Of_Range", "[content][MtlMaterialParser]")
{
    const std::string M = "newmtl first\n";

    REQUIRE_THROWS_MATCHES(
        [M] { MtlMaterialParser p; p.parse(M + "d 1.1"); }(),
        MtlMaterialException,
        ContainsExceptionMessage<MtlMaterialException>("Opacity must be in [0, 1] range"));

    REQUIRE_THROWS_MATCHES(
        [M] { MtlMaterialParser p; p.parse(M + "d -0.00001"); }(),
        MtlMaterialException,
        ContainsExceptionMessage<MtlMaterialException>("Opacity must be in [0, 1] range"));
}

TEST_CASE("Tr_Adds_Opacity", "[content][MtlMaterialParser]")
{
    MtlMaterialParser parser;

    const std::string MtlData =
        "newmtl first\n"
        "Tr 0.4\n"
        "newmtl second\n"
        "Tr 0\n";
    auto materials = parser.parse(MtlData);

    REQUIRE(0.6f == materials[0]->getFloatParameter(MaterialParameterType::Opacity));
    REQUIRE(1.0f == materials[1]->getFloatParameter(MaterialParameterType::Opacity));
}

TEST_CASE("Tr_Throws_Exception_If_Value_Field_Mising", "[content][MtlMaterialParser]")
{
    const std::string M = "newmtl first\n";

    REQUIRE_THROWS_MATCHES(
        [M] { MtlMaterialParser p; p.parse(M + "Tr"); }(),
        std::runtime_error,
        ContainsExceptionMessage<std::runtime_error>("Expected next token to be a float but no token was found"));

    REQUIRE_THROWS_MATCHES(
        [M] { MtlMaterialParser p; p.parse(M + "Tr q"); }(),
        std::runtime_error,
        ContainsExceptionMessage<std::runtime_error>("Could not parse string as float"));
}

TEST_CASE("Tr_Throws_Exception_If_Value_Out_Of_Range", "[content][MtlMaterialParser]")
{
    const std::string M = "newmtl first\n";

    REQUIRE_THROWS_MATCHES(
        [M] { MtlMaterialParser p; p.parse(M + "d 1.00021"); }(),
        MtlMaterialException,
        ContainsExceptionMessage<MtlMaterialException>("Opacity must be in [0, 1] range"));

    REQUIRE_THROWS_MATCHES(
        [M] { MtlMaterialParser p; p.parse(M + "d -0.17"); }(),
        MtlMaterialException,
        ContainsExceptionMessage<MtlMaterialException>("Opacity must be in [0, 1] range"));
}

TEST_CASE("map_Kn_Or_norm_Adds_Normal_Map", "[content][MtlMaterialParser]")
{
    MtlMaterialParser parser;

    const std::string MtlData =
        "newmtl first\n"
        "map_Kn foo.dds\n"
        "newmtl bar\n"
        "norm bar.dds\n";
    auto materials = parser.parse(MtlData);

    REQUIRE("foo.dds" == materials[0]->getTextureParameter(MaterialParameterType::NormalMap).filepath);
    REQUIRE("bar.dds" == materials[1]->getTextureParameter(MaterialParameterType::NormalMap).filepath);
}

TEST_CASE("map_Kn_Or_norm_Throws_Exception_If_Path_Missing", "[content][MtlMaterialParser]")
{
    const std::string M = "newmtl first\n";

    REQUIRE_THROWS_MATCHES(
        [M] { MtlMaterialParser p; p.parse(M + "map_Kn"); }(),
        std::runtime_error,
        ContainsExceptionMessage<std::runtime_error>("Expected next token to be a string but no token was found"));

    REQUIRE_THROWS_MATCHES(
        [M] { MtlMaterialParser p; p.parse(M + "map_Kn   "); }(),
        std::runtime_error,
        ContainsExceptionMessage<std::runtime_error>("Expected next token to be a string but no token was found"));

    REQUIRE_THROWS_MATCHES(
        [M] { MtlMaterialParser p; p.parse(M + "norm"); }(),
        std::runtime_error,
        ContainsExceptionMessage<std::runtime_error>("Expected next token to be a string but no token was found"));

    REQUIRE_THROWS_MATCHES(
        [M] { MtlMaterialParser p; p.parse(M + "norm   "); }(),
        std::runtime_error,
        ContainsExceptionMessage<std::runtime_error>("Expected next token to be a string but no token was found"));
}

TEST_CASE("disp_Adds_Displacement_Map", "[content][MtlMaterialParser]")
{
    MtlMaterialParser parser;

    const std::string MtlData =
        "newmtl first\n"
        "disp bar.dds\n"
        "newmtl bar\n"
        "disp foo.dds\n";
    auto materials = parser.parse(MtlData);

    REQUIRE("bar.dds" == materials[0]->getTextureParameter(MaterialParameterType::DisplacementMap).filepath);
    REQUIRE("foo.dds" == materials[1]->getTextureParameter(MaterialParameterType::DisplacementMap).filepath);
}

TEST_CASE("disp_Throws_Exception_If_Path_Missing", "[content][MtlMaterialParser]")
{
    const std::string M = "newmtl first\n";

    REQUIRE_THROWS_MATCHES(
        [M] { MtlMaterialParser p; p.parse(M + "disp"); }(),
        std::runtime_error,
        ContainsExceptionMessage<std::runtime_error>("Expected next token to be a string but no token was found"));

    REQUIRE_THROWS_MATCHES(
        [M] { MtlMaterialParser p; p.parse(M + "disp   "); }(),
        std::runtime_error,
        ContainsExceptionMessage<std::runtime_error>("Expected next token to be a string but no token was found"));
}

TEST_CASE("Parse_Two_Simple_Materials", "[content][MtlMaterialParser]")
{
    MtlMaterialParser parser;

    const std::string MtlData =
        "newmtl a\n"
        "Ka 0.25 0.5 0.75\n"
        "Kd 1.0 1.0 1.0\n"
        "Ks 0.2 0.4 0.7\n"
        "Tr 0.2\n"
        "newmtl b\n"
        "Ka 0.1 1.0 0.0\n"
        "Ks 0.2 0.1 0.5\n"
        "Kd 0.0 1.0 0.0\n"
        "Ns 7\n"
        "d 1\n";
    auto materials = parser.parse(MtlData);

    REQUIRE(2 == (int)materials.size());
    REQUIRE("a" == materials[0]->name());
    REQUIRE("b" == materials[1]->name());

    REQUIRE(glm::vec3(0.25f, 0.5f, 0.75f) == materials[0]->getVector3Parameter(MaterialParameterType::AmbientColor));
    REQUIRE(glm::vec3(0.1f, 1.0f, 0.0f) == materials[1]->getVector3Parameter(MaterialParameterType::AmbientColor));

    REQUIRE(glm::vec3(1.0, 1.0f, 1.0f) == materials[0]->getVector3Parameter(MaterialParameterType::DiffuseColor));
    REQUIRE(glm::vec3(0.0f, 1.0f, 0.0f) == materials[1]->getVector3Parameter(MaterialParameterType::DiffuseColor));

    REQUIRE(glm::vec3(0.2f, 0.4f, 0.7f) == materials[0]->getVector3Parameter(MaterialParameterType::SpecularColor));
    REQUIRE(glm::vec3(0.2f, 0.1f, 0.5f) == materials[1]->getVector3Parameter(MaterialParameterType::SpecularColor));

    REQUIRE(!(materials[0]->isParameterDefined(MaterialParameterType::Shininess)));
    REQUIRE(7.0f == materials[1]->getFloatParameter(MaterialParameterType::Shininess));

    REQUIRE(0.8f == materials[0]->getFloatParameter(MaterialParameterType::Opacity));
    REQUIRE(1.0f == materials[1]->getFloatParameter(MaterialParameterType::Opacity));
}
