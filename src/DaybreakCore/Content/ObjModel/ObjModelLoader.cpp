#include "stdafx.h"
#include "ObjModelLoader.h"
#include "Utility/TextUtils.h"
#include "Content/ObjModel/ObjModelException.h"
#include "Common/Error.h"

#include <sstream>
#include <charconv>

using namespace Daybreak;
using namespace Daybreak::TextUtils;

const std::string ObjModelParser::DefaultGroupName = "Default";

//---------------------------------------------------------------------------------------------------------------------
std::unique_ptr<obj_model_t> ObjModelParser::parse(const std::string_view& objData, const std::string& fileName)
{
    reset();
    m_fileName = fileName;

    // Create a default starting group for faces.
    m_model->groups.push_back(obj_group_t(DefaultGroupName));

    // Read the obj model data line by line (ignoring comments).
    StringLineReader lineReader(objData, '#');

    while (lineReader.hasNextLine())
    {
        auto line = lineReader.readNextLine();

        // Skip lines that are entirely empty space.
        if (isWhitespace(line))
        {
            continue;
        }

        // Split the line into tokens separated by whitespace. Look at the first token to determine what obj command
        // should be performed.
        StringSplitter splitter(line, " \t", true);
        auto command = splitter.readNextToken();

        if (command == "v")
        {
            m_model->positions.push_back({
                readFloat(splitter, "v", "x"),
                readFloat(splitter, "v", "y"),
                readFloat(splitter, "v", "z")});
        }
        else if (command == "vt")
        {
            m_model->uv.push_back({
                readFloat(splitter, "vt", "u"),
                readFloat(splitter, "vt", "v")});
        }
        else if (command == "vn")
        {
            m_model->normals.push_back({
                readFloat(splitter, "vn", "x"),
                readFloat(splitter, "vn", "y"),
                readFloat(splitter, "vn", "z")}); 
        }
        else if (command == "f")
        {
            currentGroup().faces.push_back(readFace(splitter));
        }
        else if (command == "g")
        {
            startGroup(readString(splitter, "g", "name"));
        }
        else if (command == "mtllib")
        {
            m_model->materialLibraries.push_back(readString(splitter, "mtllib", "filename"));
        }
        else
        {
            std::stringstream ss;
            ss << "Unknown obj command '" << command.data() << "' while reading line " << m_lineNumber;

            throw std::runtime_error(ss.str());
        }

        // Increment internal line counter (used for error reporting).
        m_lineNumber++;
    }

    return std::move(m_model);
}

//---------------------------------------------------------------------------------------------------------------------
void ObjModelParser::reset() noexcept
{
    m_model.reset(new obj_model_t);
    m_lineNumber = 0;
    m_fileName = "";
}

//---------------------------------------------------------------------------------------------------------------------
obj_group_t& ObjModelParser::currentGroup() noexcept
{
    return m_model->groups[m_model->groups.size() - 1];
}

//---------------------------------------------------------------------------------------------------------------------
void ObjModelParser::startGroup(const std::string& groupName)
{
    // Only create a new group if the current group has faces defined. Otherwise set the name of the current group.
    m_activeGroupName = groupName;

    if (currentGroup().faces.size() == 0)
    {
        currentGroup().name = groupName;
    }
    else
    {
        createNewGroup(createMergedObjectGroupName(m_activeGroupName, m_activeObjectName));
    }
}

//---------------------------------------------------------------------------------------------------------------------
void ObjModelParser::useObjectName(const std::string& objectName)
{
    // TODO: Throw an exception if the object name is empty.

    // Historically an object was a super collection of one or more groups but this does not seem to be how it is used
    // in most obj files that I've seen. Typically an exporter will use either 'g' or 'o' but not both to create
    // group names.
    // 
    // As a workaround this loader tries to support the historical use of 'o' by prefixing the object name to any group
    // created by 'g'. Since some files only use 'o' and not 'g' the loader will apply the object name to an obj group
    // if it does not already have a name.
    //
    // Only create a new group if the current group has faces defined. Otherwise set the name of the current group.
    m_activeObjectName = objectName;

    if (currentGroup().faces.size() == 0)
    {
        if (currentGroup().name.size() > 0)
        {
            // TODO: Warn that group name already set.
        }

        currentGroup().name = objectName;
    }
    else
    {
        createNewGroup(createMergedObjectGroupName(m_activeGroupName, m_activeObjectName));
    }
}

//---------------------------------------------------------------------------------------------------------------------
void ObjModelParser::useMaterial(const std::string& materialName)
{
    // TODO: Throw an exception if the group name is empty.

    // Only creatae a new group if the current group has faces defined. Otherwise set the current group's material
    // property.
    m_activeMaterialName = materialName;

    if (currentGroup().faces.size() == 0)
    {
        // TODO: Warn if there was a material already set.
        currentGroup().material = materialName;
    }
    else
    {
        // Create a new group (and set it as current) with the active object/group/material name. Make sure its
        // material is set to this.
        auto& group = createNewGroup(createMergedObjectGroupName(m_activeGroupName, m_activeObjectName));
        group.material = materialName;
    }
}

//---------------------------------------------------------------------------------------------------------------------
obj_group_t& ObjModelParser::createNewGroup(const std::string& name)
{    
    m_model->groups.push_back(obj_group_t(name));
    return m_model->groups[m_model->groups.size() - 1];
}

//---------------------------------------------------------------------------------------------------------------------
std::string ObjModelParser::createMergedObjectGroupName(
    const std::string& objectName,
    const std::string& groupName) const
{
    std::stringstream ss;

    // If no parameters are defined then create an initial default group.
    if (objectName.size() == 0 && groupName.size() == 0)
    {
        return DefaultGroupName;
    }
    
    if (objectName.size() > 0)
    {
        ss << objectName;

        if (groupName.size() > 0)
        {
            ss << ":";
            ss << groupName;
        }
    }
    else
    {
        ss << objectName;
    }

    return ss.str();
}

//---------------------------------------------------------------------------------------------------------------------
obj_face_t ObjModelParser::readFace(Daybreak::TextUtils::StringSplitter& arguments)
{
    // Read first three face elements.
    //  TODO: Handle face commands with more than three elments. Do not split it up here, instead add ability to have
    //        obj_faces with more than three elements and update the obj -> model code to handle conversion.
    auto a = resolveIndices(readFaceElement(arguments, "f", "0"));
    auto b = resolveIndices(readFaceElement(arguments, "f", "1"));
    auto c = resolveIndices(readFaceElement(arguments, "f", "2"));

    if (m_firstFace)
    {
        m_model->hasUV = a.hasUV();
        m_model->hasNormals = a.hasNormals();

        m_firstFace = false;
    }

    // Check that face elements have the same data types as previous faces in this model.
    if (m_model->hasUV != a.hasUV() || m_model->hasNormals != a.hasNormals() ||
        m_model->hasUV != b.hasUV() || m_model->hasNormals != b.hasNormals() ||
        m_model->hasUV != c.hasUV() || m_model->hasNormals != c.hasNormals())
    {
        // TODO: Better exception.
        throw std::runtime_error("Face elmeents have inconsistent data types (uv and or normal)");
    }

    return { { a.p, b.p, c.p }, { a.t, b.t, c.t }, { a.n, b.n, c.n } };
}

//---------------------------------------------------------------------------------------------------------------------
obj_face_vertex_t ObjModelParser::resolveIndices(const obj_face_vertex_t& element) const
{
    // TODO: Check type consistency.
    obj_face_vertex_t result;

    result.p = relativeToAbsoluteIndex(element.p, m_model->positions.size());
    result.t = relativeToAbsoluteIndex(element.t, m_model->uv.size());
    result.n = relativeToAbsoluteIndex(element.n, m_model->normals.size());

    return result;
}

//---------------------------------------------------------------------------------------------------------------------
int ObjModelParser::relativeToAbsoluteIndex(int index, size_t count)
{
    if (index < 0)
    {
        if (static_cast<size_t>(-index) > count)
        {
            // TODO: Better exception.
            throw std::runtime_error("Negative index must be smaller than current size of data array");
        }

        index = static_cast<int>(count) - index;
    }
    else if (static_cast<size_t>(index) > count)
    {
        // TODO: Better exception.
        throw std::runtime_error("Positive index must be smaller than current size of data array");
    }

    return index;
}

//---------------------------------------------------------------------------------------------------------------------
obj_face_vertex_t ObjModelParser::readFaceElement(
    Daybreak::TextUtils::StringSplitter& arguments,
    const char * command,
    const char * field) const
{
    if (arguments.hasNextToken())
    {
        obj_face_vertex_t element;

        // Split the face token into each part p/t/n and make sure empty tokens are not skipped.
        auto faceToken = arguments.readNextToken();
        StringSplitter splitter(faceToken, "/", false);
        
        // Read position index.
        if (splitter.hasNextToken())
        {
            element.p = parseInt(splitter.readNextToken(), command, "positionIndex");
        }
        else
        {
            throw ObjModelException("Missing face position element", m_fileName, m_lineNumber, command, field);
        }

        // Try to read texture index (if it exists).
        if (splitter.hasNextToken())
        {
            auto token = splitter.readNextToken();

            if (token.size() > 0)
            {
                element.t = parseInt(token, command, "uvIndex");
            }
        }

        // Try to read normal index (if it exists).
        if (splitter.hasNextToken())
        {
            auto token = splitter.readNextToken();

            if (token.size() > 0)
            {
                element.n = parseInt(token, command, "normalIndex");
            }
        }

        return element;
    }
    else
    {
        throw ObjModelException("Missing face element", m_fileName, m_lineNumber, command, field);
    }
}

//---------------------------------------------------------------------------------------------------------------------
float ObjModelParser::readFloat(
    Daybreak::TextUtils::StringSplitter& arguments,
    const char * command,
    const char * field) const
{
    if (arguments.hasNextToken())
    {
        return parseFloat(arguments.readNextToken(), command, field);
    }
    else
    {
        throw ObjModelException("Missing float value", m_fileName, m_lineNumber, command, field);
    }
}

//---------------------------------------------------------------------------------------------------------------------
float ObjModelParser::parseFloat(
    const std::string_view& token,
    const char * command,
    const char * field) const
{
    float value = 0.0f;
    auto result = std::from_chars(
        token.data(),
        token.data() + token.size(),
        value);

    if (result.ec == std::errc::invalid_argument || result.ec == std::errc::result_out_of_range)
    {
        throw ObjModelException("Invalid float value", m_fileName, m_lineNumber, command, field);
    }

    return value;
}

//---------------------------------------------------------------------------------------------------------------------
int ObjModelParser::readInt(
    Daybreak::TextUtils::StringSplitter& arguments,
    const char * command,
    const char * field) const
{
    if (arguments.hasNextToken())
    {
        return parseInt(arguments.readNextToken(), command, field);
    }
    else
    {
        throw ObjModelException("Missing int value", m_fileName, m_lineNumber, command, field);
    }
}

//---------------------------------------------------------------------------------------------------------------------
int ObjModelParser::parseInt(
    const std::string_view& token,
    const char * command,
    const char * field) const
{
    int value = 0;
    auto result = std::from_chars(
        token.data(),
        token.data() + token.size(),
        value);

    if (result.ec == std::errc::invalid_argument || result.ec == std::errc::result_out_of_range)
    {
        throw ObjModelException("Invalid int value", m_fileName, m_lineNumber, command, field);
    }

    return value;
}

//---------------------------------------------------------------------------------------------------------------------
std::string ObjModelParser::readString(
    Daybreak::TextUtils::StringSplitter& arguments,
    const char * command,
    const char * field) const
{
    if (arguments.hasNextToken())
    {
        auto view = arguments.readNextToken();
        return std::string(view.data(), view.size());
    }
    else
    {
        throw ObjModelException("Missing string value", m_fileName, m_lineNumber, command, field);
    }
}
