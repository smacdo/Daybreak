#include "stdafx.h"
#include "ObjModelLoader.h"
#include "Utility/TextUtils.h"
#include "Content/ObjModel/ObjModelException.h"
#include "Content/ObjModel/TextParsingUtils.h"
#include "Common/Error.h"

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
        parseLine(lineReader.readNextLine());
        m_lineNumber++;
    }

    return std::move(m_model);
}

//---------------------------------------------------------------------------------------------------------------------
void ObjModelParser::parseLine(const std::string_view& line)
{
    // Skip lines that are entirely empty space.
    if (isWhitespace(line))
    {
        return;
    }

    // Split the line into tokens separated by whitespace. Look at the first token to determine what obj command
    // should be performed.
    StringSplitter splitter(line, " \t", true);
    auto command = splitter.readNextToken();

    try
    {
        if (command == "v")
        {
            m_model->positions.push_back({
                readExpectedFloat(splitter),
                readExpectedFloat(splitter),
                readExpectedFloat(splitter) });
        }
        else if (command == "vt")
        {
            m_model->uv.push_back({
                readExpectedFloat(splitter),
                readExpectedFloat(splitter) });
        }
        else if (command == "vn")
        {
            m_model->normals.push_back({
                readExpectedFloat(splitter),
                readExpectedFloat(splitter),
                readExpectedFloat(splitter) });
        }
        else if (command == "f")
        {
            currentGroup().faces.push_back(readFace(splitter));
        }
        else if (command == "g")
        {
            useGroupName(readExpectedString(splitter));
        }
        else if (command == "o")
        {
            useObjectName(readExpectedString(splitter));
        }
        else if (command == "usemtl")
        {
            useMaterial(readExpectedString(splitter));
        }
        else if (command == "mtllib")
        {
            m_model->materialLibraries.push_back(readExpectedString(splitter));
        }
        else
        {
            throw ObjModelException("Unknown obj command", m_fileName, m_lineNumber, "", "");
        }
    }
    catch (const std::runtime_error& e)
    {
        throw ObjModelException(e.what(), m_fileName, m_lineNumber, command.data(), "");
    }
}

//---------------------------------------------------------------------------------------------------------------------
void ObjModelParser::reset() noexcept
{
    m_model.reset(new obj_model_t);
    m_lineNumber = 1;
    m_fileName = "";
    m_activeObjectName = "";
    m_activeGroupName = "";
    m_activeMaterialName = "";
    m_firstFace = true;
}

//---------------------------------------------------------------------------------------------------------------------
obj_group_t& ObjModelParser::currentGroup() noexcept
{
    return m_model->groups[m_model->groups.size() - 1];
}

//---------------------------------------------------------------------------------------------------------------------
void ObjModelParser::useGroupName(const std::string& groupName)
{
    CHECK_NOT_EMPTY(groupName);
    m_activeGroupName = groupName;

    startObjectGroupName();
}

//---------------------------------------------------------------------------------------------------------------------
void ObjModelParser::useObjectName(const std::string& objectName)
{
    CHECK_NOT_EMPTY(objectName);
    m_activeObjectName = objectName;

    startObjectGroupName();
}

//---------------------------------------------------------------------------------------------------------------------
void ObjModelParser::startObjectGroupName()
{
    // Historically an object was a super collection of one or more groups but this does not seem to be how it is used
    // in most obj files that I've seen. Typically an exporter will use either 'g' or 'o' but not both to create
    // group names.
    // 
    // As a workaround this loader tries to support the historical use of 'o' by prefixing the object name to any group
    // created by 'g'. Since some files only use 'o' and not 'g' the loader will apply the object name to an obj group
    // if it does not already have a name.
    auto mergedName = createMergedObjectGroupName(m_activeObjectName, m_activeGroupName);

    if (currentGroup().faces.size() == 0)
    {
        currentGroup().name = mergedName;
    }
    else
    {
        createNewGroup(mergedName);
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
        ss << groupName;
    }

    return ss.str();
}

//---------------------------------------------------------------------------------------------------------------------
obj_face_t ObjModelParser::readFace(Daybreak::TextUtils::StringSplitter& arguments)
{
    // TODO: Handle face commands with more than three arguments by using the following:
    //  face (0, i, i + 1) [for i in 1..(n - 2)]
    // ref: http://stackoverflow.com/a/23724231

    // Read first three face elements.
    //  TODO: Handle face commands with more than three elments. Do not split it up here, instead add ability to have
    //        obj_faces with more than three elements and update the obj -> model code to handle conversion.
    auto a = resolveIndices(readFaceElement(arguments, "f", "0"), "f", "0");
    auto b = resolveIndices(readFaceElement(arguments, "f", "1"), "f", "1");
    auto c = resolveIndices(readFaceElement(arguments, "f", "2"), "f", "2");

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
        throw ObjModelException(
            "Face elements have inconsistent data layout",
            m_fileName,
            m_lineNumber,
            "f",
            "");
    }

    return { { a.p, b.p, c.p }, { a.t, b.t, c.t }, { a.n, b.n, c.n } };
}

//---------------------------------------------------------------------------------------------------------------------
obj_face_vertex_t ObjModelParser::resolveIndices(
    const obj_face_vertex_t& element,
    const char * command,
    const char * field) const
{
    obj_face_vertex_t result;

    result.p = relativeToAbsoluteIndex(element.p, m_model->positions.size(), command, field);
    result.t = relativeToAbsoluteIndex(element.t, m_model->uv.size(), command, field);
    result.n = relativeToAbsoluteIndex(element.n, m_model->normals.size(), command, field);

    return result;
}

//---------------------------------------------------------------------------------------------------------------------
int ObjModelParser::relativeToAbsoluteIndex(
    int relativeIndex,
    size_t arrayLength,
    const char * command,
    const char * field) const
{
    // Obj models with data arrays larger than sizeof(int) will break due to the casting in this method. If this
    // becomes a problem fix the overflow caused by casting and remove this check.
    if (arrayLength > std::numeric_limits<int>::max())
    {
        throw ObjModelException(
            "Obj data array size is too large for converter (fix this!)",
            m_fileName,
            m_lineNumber,
            command,
            field);
    }

    int arrayLengthInt = static_cast<int>(arrayLength);
    int index = relativeIndex;

    // Convert negative index to an absolute index relative to the end of the data array. For example a -1 index with a
    // data array of size 10 should be 10, and -2 would be 9.
    if (relativeIndex < 0)
    {
        index = arrayLengthInt - std::abs(relativeIndex + 1);
    }

    // Check the index size is valid. If the index was a relative index it should always be correct unless the above
    // code is bad. Note that the check is performed with > and not >= because .obj indices are one based.
    if (relativeIndex > arrayLength)
    {
        throw ObjModelException(
            "Index must be smaller than size of data array",
            m_fileName,
            m_lineNumber,
            command,
            field);
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
            element.p = parseInt(splitter.readNextToken());
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
                element.t = parseInt(token);
            }
        }

        // Try to read normal index (if it exists).
        if (splitter.hasNextToken())
        {
            auto token = splitter.readNextToken();

            if (token.size() > 0)
            {
                element.n = parseInt(token);
            }
        }

        return element;
    }
    else
    {
        throw ObjModelException("Missing face element", m_fileName, m_lineNumber, command, field);
    }
}
