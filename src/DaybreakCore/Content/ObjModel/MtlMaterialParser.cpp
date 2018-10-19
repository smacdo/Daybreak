#include "stdafx.h"
#include "MtlMaterialParser.h"
#include "Utility/TextUtils.h"
#include "Content/ObjModel/MtlMaterialException.h"
#include "Content/ObjModel/TextParsingUtils.h"
#include "Graphics/MaterialData.h"
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
    else
    {
        throw MtlMaterialException("Unknown mtl command", m_fileName, m_lineNumber, "", "");
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
MaterialData& MtlMaterialParser::currentGroup() noexcept
{
    EXPECT(m_materials.size() > 0, "MTL parser should always have a material defined");
    return *m_materials[m_materials.size() - 1].get();
}
