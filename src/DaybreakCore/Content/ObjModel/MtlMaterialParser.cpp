#include "stdafx.h"
#include "MtlMaterialParser.h"
#include "Utility/TextUtils.h"
#include "Content/ObjModel/MtlMaterialException.h"
#include "Content/ObjModel/TextParsingUtils.h"
#include "Content/MaterialData.h"
#include "Common/Error.h"

#include <sstream>
#include <charconv>

using namespace Daybreak;
using namespace Daybreak::TextUtils;


//---------------------------------------------------------------------------------------------------------------------
std::vector<std::unique_ptr<MaterialData>>&& MtlMaterialParser::parse(
    const std::string_view& mtlData,
    const std::string& fileName)
{
    reset();
    m_fileName = fileName;
    
    // Read the mtl data line by line (ignoring comments).
    StringLineReader lineReader(mtlData, '#');

    while (lineReader.hasNextLine())
    {
        parseLine(lineReader.readNextLine());
        m_lineNumber++;
    }

    return std::move(m_materials);
}

//---------------------------------------------------------------------------------------------------------------------
void MtlMaterialParser::parseLine(const std::string_view& line)
{
    // Skip lines that are entirely empty space.
    if (isWhitespace(line))
    {
        return;
    }

    // Split the line into tokens separated by whitespace. Look at the first token to determine what mtl command
    // should be performed.
    StringSplitter splitter(line, " \t", true);
    auto command = splitter.readNextToken();

    if (command == "newmtl")
    {
        auto name = readExpectedString(splitter);
        m_materials.push_back(std::make_unique<MaterialData>(name, MaterialType::Traditional));
    }
    else if (command == "Ka")
    {
        auto r = readExpectedFloat(splitter);
        auto g = readExpectedFloat(splitter);
        auto b = readExpectedFloat(splitter);

        currentMaterial().setParameter(MaterialParameter::AmbientColor, glm::vec3(r, g, b));
    }
    else if (command == "map_Ka")
    {
        // TODO: Issue warning that ambient texture map not supported.
        auto filepath = readExpectedString(splitter);
    }
    else if (command == "Kd")
    {
        auto r = readExpectedFloat(splitter);
        auto g = readExpectedFloat(splitter);
        auto b = readExpectedFloat(splitter);

        currentMaterial().setParameter(MaterialParameter::DiffuseColor, glm::vec3(r, g, b));
    }
    else if (command == "map_Kd")
    {
        auto filepath = readExpectedString(splitter);
        currentMaterial().setParameter(MaterialParameter::DiffuseMap, material_texture_t{ filepath });
    }
    else if (command == "Ks")
    {
        auto r = readExpectedFloat(splitter);
        auto g = readExpectedFloat(splitter);
        auto b = readExpectedFloat(splitter);

        currentMaterial().setParameter(MaterialParameter::SpecularColor, glm::vec3(r, g, b));
    }
    else if (command == "map_Ks")
    {
        auto filepath = readExpectedString(splitter);
        currentMaterial().setParameter(MaterialParameter::SpecularMap, material_texture_t{ filepath });
    }
    else if (command == "Ns")
    {
        currentMaterial().setParameter(MaterialParameter::Shininess, readExpectedFloat(splitter));
    }
    else if (command == "map_Ns")
    {
        // TODO: Issue warning that specular power texture map not supported.
        auto filepath = readExpectedString(splitter);
    }
    else if (command == "d")
    {
        auto o = readExpectedFloat(splitter);

        if (o < 0.0f || o > 1.0f)
        {
            throw MtlMaterialException("Opacity must be in [0, 1] range", m_fileName, m_lineNumber, "d", "");
        }

        currentMaterial().setParameter(MaterialParameter::Opacity, o);
    }
    else if (command == "map_d")
    {
        // TODO: Issue warning that alpha texture map not supported.
        auto filepath = readExpectedString(splitter);
    }
    else if (command == "Tr")
    {
        auto o = readExpectedFloat(splitter);

        if (o < 0.0f || o > 1.0f)
        {
            throw MtlMaterialException("Opacity must be in [0, 1] range", m_fileName, m_lineNumber, "Tr", "");
        }

        currentMaterial().setParameter(MaterialParameter::Opacity, 1.0f - o);
    }
    else if (command == "map_bump" || command == "bump")
    {
        // TODO: Issue warning that bumps map not supported.
        auto filepath = readExpectedString(splitter);
    }
    else if (command == "disp")
    {
        auto filepath = readExpectedString(splitter);
        currentMaterial().setParameter(MaterialParameter::DisplacementMap, material_texture_t{ filepath });
    }
    else if (command == "map_Kn" || command == "norm")
    {
        auto filepath = readExpectedString(splitter);
        currentMaterial().setParameter(MaterialParameter::NormalMap, material_texture_t{ filepath });
    }
    else
    {
        throw MtlMaterialException("Error while parsing material file", m_fileName, m_lineNumber, "", "");
    }
}

//---------------------------------------------------------------------------------------------------------------------
void MtlMaterialParser::reset() noexcept
{
    m_materials.clear();
    m_lineNumber = 0;
    m_fileName = "";
}

//---------------------------------------------------------------------------------------------------------------------
MaterialData& MtlMaterialParser::currentMaterial()
{
    if (m_materials.size() == 0)
    {
        throw MtlMaterialException("No material group defined", m_fileName, m_lineNumber, "", "");
    }

    return *m_materials[m_materials.size() - 1].get();
}
