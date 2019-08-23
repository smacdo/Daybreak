#include "stdafx.h"
#include "CppUnitTest.h"
#include "Content/ObjModel/MtlMaterialParser.h"
#include "Content/ObjModel/MtlMaterialException.h"
#include "Content/MaterialData.h"
#include "../TestHelpers.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Daybreak;

TEST_CLASS(MtlMaterialParserTests)
{
public:
    TEST_METHOD(NEWMTL_Creates_A_New_Material_Group)
    {
        MtlMaterialParser parser;

        const char* MtlData =
            "newmtl first\n"
            "newmtl second\n";
        auto materials = parser.parse(MtlData);

        Assert::AreEqual(2, (int)materials.size());
        Assert::AreEqual("first", materials[0]->name().c_str());
        Assert::AreEqual("second", materials[1]->name().c_str());
    }

    TEST_METHOD(Throws_Exception_If_No_Material_Defined_But_Command_Requires_One)
    {
        Assert::ExpectException<MtlMaterialException>([] { MtlMaterialParser p; p.parse("Ka 1 2 3\n"); });
    }

    TEST_METHOD(Ka_Adds_Ambient_Color)
    {
        MtlMaterialParser parser;

        const std::string MtlData =
            "newmtl first\n"
            "Ka 0.25 0.5 0.75\n"
            "newmtl second\n"
            "Ka 0.1 1.0 0.0\n";
        auto materials = parser.parse(MtlData);

        Assert::AreEqual(
            glm::vec3(0.25f, 0.5f, 0.75f),
            materials[0]->getVector3Parameter(MaterialParameter::AmbientColor));
        Assert::AreEqual(
            glm::vec3(0.1f, 1.0f, 0.0f),
            materials[1]->getVector3Parameter(MaterialParameter::AmbientColor));
    }

    TEST_METHOD(Ka_Throws_Exception_If_R_G_B_Field_Missing)
    {
        const std::string M = "newmtl first\n";

        Assert::ExpectException<std::runtime_error>([M] { MtlMaterialParser p; p.parse(M + "Ka"); });
        Assert::ExpectException<std::runtime_error>([M] { MtlMaterialParser p; p.parse(M + "Ka 0"); });
        Assert::ExpectException<std::runtime_error>([M] { MtlMaterialParser p; p.parse(M + "Ka 0 0"); });
        Assert::ExpectException<std::runtime_error>([M] { MtlMaterialParser p; p.parse(M + "Ka r g b"); });
    }

    TEST_METHOD(Kd_Adds_Diffuse_Color)
    {
        MtlMaterialParser parser;

        const std::string MtlData =
            "newmtl first\n"
            "Kd 0.4 0.72 0.1\n"
            "newmtl second\n"
            "Kd 0.3 0.22 0.9\n";
        auto materials = parser.parse(MtlData);

        Assert::AreEqual(
            glm::vec3(0.4f, 0.72f, 0.1f),
            materials[0]->getVector3Parameter(MaterialParameter::DiffuseColor));
        Assert::AreEqual(
            glm::vec3(0.3f, 0.22f, 0.9f),
            materials[1]->getVector3Parameter(MaterialParameter::DiffuseColor));
    }

    TEST_METHOD(Kd_Throws_Exception_If_R_G_B_Field_Missing)
    {
        const std::string M = "newmtl first\n";

        Assert::ExpectException<std::runtime_error>([M] { MtlMaterialParser p; p.parse(M + "Kd"); });
        Assert::ExpectException<std::runtime_error>([M] { MtlMaterialParser p; p.parse(M + "Kd 0"); });
        Assert::ExpectException<std::runtime_error>([M] { MtlMaterialParser p; p.parse(M + "Kd 0 0"); });
        Assert::ExpectException<std::runtime_error>([M] { MtlMaterialParser p; p.parse(M + "Kd r g b"); });
    }

    TEST_METHOD(map_Kd_Adds_Diffuse_Texture_Map)
    {
        MtlMaterialParser parser;

        const std::string MtlData =
            "newmtl first\n"
            "map_Kd map1.dds\n"
            "newmtl second\n"
            "map_Kd map2.dds\n";
        auto materials = parser.parse(MtlData);

        Assert::AreEqual(
            "map1.dds",
            materials[0]->getTextureParameter(MaterialParameter::DiffuseMap).filepath.c_str());
        Assert::AreEqual(
            "map2.dds",
            materials[1]->getTextureParameter(MaterialParameter::DiffuseMap).filepath.c_str());
    }

    TEST_METHOD(map_Kd_Throws_Exception_If_Path_Missing)
    {
        const std::string M = "newmtl first\n";
        Assert::ExpectException<std::runtime_error>([M] { MtlMaterialParser p; p.parse(M + "map_Kd"); });
        Assert::ExpectException<std::runtime_error>([M] { MtlMaterialParser p; p.parse(M + "map_Kd   "); });
    }

    TEST_METHOD(Ks_Adds_Specular_Color)
    {
        MtlMaterialParser parser;

        const std::string MtlData =
            "newmtl first\n"
            "Ks 0.1 0.2 0.3\n"
            "newmtl second\n"
            "Ks 0.0 0.5 1\n";
        auto materials = parser.parse(MtlData);

        Assert::AreEqual(
            glm::vec3(0.1f, 0.2f, 0.3f),
            materials[0]->getVector3Parameter(MaterialParameter::SpecularColor));
        Assert::AreEqual(
            glm::vec3(0.0f, 0.5f, 1.0f),
            materials[1]->getVector3Parameter(MaterialParameter::SpecularColor));
    }

    TEST_METHOD(Ks_Throws_Exception_If_R_G_B_Field_Missing)
    {
        const std::string M = "newmtl first\n";

        Assert::ExpectException<std::runtime_error>([M] { MtlMaterialParser p; p.parse(M + "Ks"); });
        Assert::ExpectException<std::runtime_error>([M] { MtlMaterialParser p; p.parse(M + "Ks 0"); });
        Assert::ExpectException<std::runtime_error>([M] { MtlMaterialParser p; p.parse(M + "Ks 0 0"); });
        Assert::ExpectException<std::runtime_error>([M] { MtlMaterialParser p; p.parse(M + "Ks r g b"); });
    }

    TEST_METHOD(map_Ks_Adds_Specular_Texture_Map)
    {
        MtlMaterialParser parser;

        const std::string MtlData =
            "newmtl first\n"
            "map_Ks map1.dds\n"
            "newmtl second\n"
            "map_Ks map2.dds\n";
        auto materials = parser.parse(MtlData);

        Assert::AreEqual(
            "map1.dds",
            materials[0]->getTextureParameter(MaterialParameter::SpecularMap).filepath.c_str());
        Assert::AreEqual(
            "map2.dds",
            materials[1]->getTextureParameter(MaterialParameter::SpecularMap).filepath.c_str());
    }

    TEST_METHOD(map_Ks_Throws_Exception_If_Path_Missing)
    {
        const std::string M = "newmtl first\n";
        Assert::ExpectException<std::runtime_error>([M] { MtlMaterialParser p; p.parse(M + "map_Ks"); });
        Assert::ExpectException<std::runtime_error>([M] { MtlMaterialParser p; p.parse(M + "map_Ks   "); });
    }

    TEST_METHOD(Ns_Adds_Specular_Power)
    {
        MtlMaterialParser parser;

        const std::string MtlData =
            "newmtl first\n"
            "Ns 2\n"
            "newmtl second\n"
            "Ns 1.73\n";
        auto materials = parser.parse(MtlData);

        Assert::AreEqual(2.0f, materials[0]->getFloatParameter(MaterialParameter::Shininess));
        Assert::AreEqual(1.73f, materials[1]->getFloatParameter(MaterialParameter::Shininess));
    }

    TEST_METHOD(Ns_Throws_Exception_If_Value_Field_Missing)
    {
        const std::string M = "newmtl first\n";

        Assert::ExpectException<std::runtime_error>([M] { MtlMaterialParser p; p.parse(M + "Ns"); });
        Assert::ExpectException<std::runtime_error>([M] { MtlMaterialParser p; p.parse(M + "Ns x"); });
    }

    TEST_METHOD(d_Adds_Opacity)
    {
        MtlMaterialParser parser;

        const std::string MtlData =
            "newmtl first\n"
            "d 0.4\n"
            "newmtl second\n"
            "d 0\n";
        auto materials = parser.parse(MtlData);

        Assert::AreEqual(0.4f, materials[0]->getFloatParameter(MaterialParameter::Opacity));
        Assert::AreEqual(0.0f, materials[1]->getFloatParameter(MaterialParameter::Opacity));
    }

    TEST_METHOD(d_Throws_Exception_If_Value_Field_Mising)
    {
        const std::string M = "newmtl first\n";

        Assert::ExpectException<std::runtime_error>([M] { MtlMaterialParser p; p.parse(M + "d"); });
        Assert::ExpectException<std::runtime_error>([M] { MtlMaterialParser p; p.parse(M + "d q"); });
    }

    TEST_METHOD(d_Throws_Exception_If_Value_Out_Of_Range)
    {
        const std::string M = "newmtl first\n";

        Assert::ExpectException<std::runtime_error>([M] { MtlMaterialParser p; p.parse(M + "d 1.1"); });
        Assert::ExpectException<std::runtime_error>([M] { MtlMaterialParser p; p.parse(M + "d -0.0001"); });
    }

    TEST_METHOD(Tr_Adds_Opacity)
    {
        MtlMaterialParser parser;

        const std::string MtlData =
            "newmtl first\n"
            "Tr 0.4\n"
            "newmtl second\n"
            "Tr 0\n";
        auto materials = parser.parse(MtlData);

        Assert::AreEqual(0.6f, materials[0]->getFloatParameter(MaterialParameter::Opacity));
        Assert::AreEqual(1.0f, materials[1]->getFloatParameter(MaterialParameter::Opacity));
    }

    TEST_METHOD(Tr_Throws_Exception_If_Value_Field_Mising)
    {
        const std::string M = "newmtl first\n";

        Assert::ExpectException<std::runtime_error>([M] { MtlMaterialParser p; p.parse(M + "Tr"); });
        Assert::ExpectException<std::runtime_error>([M] { MtlMaterialParser p; p.parse(M + "Tr q"); });
    }

    TEST_METHOD(Tr_Throws_Exception_If_Value_Out_Of_Range)
    {
        const std::string M = "newmtl first\n";

        Assert::ExpectException<std::runtime_error>([M] { MtlMaterialParser p; p.parse(M + "Tr  1.00021"); });
        Assert::ExpectException<std::runtime_error>([M] { MtlMaterialParser p; p.parse(M + "Tr -0.17"); });
    }

    TEST_METHOD(map_Kn_Or_norm_Adds_Normal_Map)
    {
        MtlMaterialParser parser;

        const std::string MtlData =
            "newmtl first\n"
            "map_Kn foo.dds\n"
            "newmtl bar\n"
            "norm bar.dds\n";
        auto materials = parser.parse(MtlData);

        Assert::AreEqual(
            "foo.dds",
            materials[0]->getTextureParameter(MaterialParameter::NormalMap).filepath.c_str());
        Assert::AreEqual(
            "bar.dds",
            materials[1]->getTextureParameter(MaterialParameter::NormalMap).filepath.c_str());
    }

    TEST_METHOD(map_Kn_Or_norm_Throws_Exception_If_Path_Missing)
    {
        const std::string M = "newmtl first\n";
        Assert::ExpectException<std::runtime_error>([M] { MtlMaterialParser p; p.parse(M + "map_Kn"); });
        Assert::ExpectException<std::runtime_error>([M] { MtlMaterialParser p; p.parse(M + "map_Kn   "); });
        Assert::ExpectException<std::runtime_error>([M] { MtlMaterialParser p; p.parse(M + "normal"); });
        Assert::ExpectException<std::runtime_error>([M] { MtlMaterialParser p; p.parse(M + "normal   "); });
    }

    TEST_METHOD(disp_Adds_Displacement_Map)
    {
        MtlMaterialParser parser;

        const std::string MtlData =
            "newmtl first\n"
            "disp bar.dds\n"
            "newmtl bar\n"
            "disp foo.dds\n";
        auto materials = parser.parse(MtlData);

        Assert::AreEqual(
            "bar.dds",
            materials[0]->getTextureParameter(MaterialParameter::DisplacementMap).filepath.c_str());
        Assert::AreEqual(
            "foo.dds",
            materials[1]->getTextureParameter(MaterialParameter::DisplacementMap).filepath.c_str());
    }

    TEST_METHOD(disp_Throws_Exception_If_Path_Missing)
    {
        const std::string M = "newmtl first\n";
        Assert::ExpectException<std::runtime_error>([M] { MtlMaterialParser p; p.parse(M + "disp"); });
        Assert::ExpectException<std::runtime_error>([M] { MtlMaterialParser p; p.parse(M + "disp   "); });
    }

    TEST_METHOD(Parse_Two_Simple_Materials)
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

        Assert::AreEqual(2, (int)materials.size());
        Assert::AreEqual("a", materials[0]->name().c_str());
        Assert::AreEqual("b", materials[1]->name().c_str());

        Assert::AreEqual(
            glm::vec3(0.25f, 0.5f, 0.75f),
            materials[0]->getVector3Parameter(MaterialParameter::AmbientColor));
        Assert::AreEqual(
            glm::vec3(0.1f, 1.0f, 0.0f),
            materials[1]->getVector3Parameter(MaterialParameter::AmbientColor));

        Assert::AreEqual(
            glm::vec3(1.0, 1.0f, 1.0f),
            materials[0]->getVector3Parameter(MaterialParameter::DiffuseColor));
        Assert::AreEqual(
            glm::vec3(0.0f, 1.0f, 0.0f),
            materials[1]->getVector3Parameter(MaterialParameter::DiffuseColor));

        Assert::AreEqual(
            glm::vec3(0.2f, 0.4f, 0.7f),
            materials[0]->getVector3Parameter(MaterialParameter::SpecularColor));
        Assert::AreEqual(
            glm::vec3(0.2f, 0.1f, 0.5f),
            materials[1]->getVector3Parameter(MaterialParameter::SpecularColor));

        Assert::IsFalse(materials[0]->isParameterDefined(MaterialParameter::Shininess));
        Assert::AreEqual(7.0f, materials[1]->getFloatParameter(MaterialParameter::Shininess));

        Assert::AreEqual(0.8f, materials[0]->getFloatParameter(MaterialParameter::Opacity));
        Assert::AreEqual(1.0f, materials[1]->getFloatParameter(MaterialParameter::Opacity));
    }

};
