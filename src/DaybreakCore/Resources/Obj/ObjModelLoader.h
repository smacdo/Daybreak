#pragma once
#include <string>
#include <memory>
#include <vector>
#include <map>
#include <glm/glm.hpp>

#include "Utility/TextUtils.h"

namespace Daybreak
{
    class MeshData;

    /// Utility function that loads a mesh from an obj file.
    std::unique_ptr<MeshData> LoadObjModelFromText(const std::string& modeldata);

    /// Obj model data.
    struct obj_face_t
    {
        int position[3];
        int uv[3];
        int normal[3];

        /// Equality operator.
        bool operator ==(const obj_face_t& rhs) const
        {
            return
                position[0] == rhs.position[0] &&
                position[1] == rhs.position[1] &&
                position[2] == rhs.position[2] &&
                uv[0] == rhs.uv[0] &&
                uv[1] == rhs.uv[1] &&
                uv[2] == rhs.uv[2] &&
                normal[0] == rhs.normal[0] &&
                normal[1] == rhs.normal[1] &&
                normal[2] == rhs.normal[2];
        }
    };

    /// One vertex in an obj face.
    struct obj_face_vertex_t
    {
        int p = 0;
        int t = 0;
        int n = 0;

        /// Check if element has a valid index into the texture data array.
        bool hasUV() const noexcept { return t != 0; }

        /// Check if element has a valid index into the normal data array.
        bool hasNormals() const noexcept { return n != 0; }

        /// Equality operator.
        bool operator ==(const obj_face_vertex_t& rhs) const
        {
            return p == rhs.p && t == rhs.t && n == rhs.n;
        }
    };

    struct obj_group_t
    {
        obj_group_t(const std::string& name = "")
            : name(name)
        {
        }

        std::string name;
        std::string material;
        std::vector<obj_face_t> faces;
    };

    struct obj_model_t
    {
        std::vector<glm::vec3> positions;
        std::vector<glm::vec2> uv;
        std::vector<glm::vec3> normals;
        std::vector<obj_group_t> groups;
        std::vector<std::string> materialLibraries;

        bool hasUV = false;
        bool hasNormals = false;
    };

    /// Configurable loader for obj models.
    class ObjModelParser
    {
    public:
        static const std::string DefaultGroupName;

        std::unique_ptr<obj_model_t> parse(const std::string_view& objData, const std::string& fileName = "");

    private:
        void reset() noexcept;

        obj_group_t& currentGroup() noexcept;

        void startGroup(const std::string& groupName);
        void useObjectName(const std::string& objectName);
        void useMaterial(const std::string& materialName);

        obj_face_t readFace(Daybreak::TextUtils::StringSplitter& arguments);

        // Convert an obj_face_element_t from relative indices (positive or negative) to absolute (positive) only
        // values. This maintains the one based indexing nature of obj indices.
        obj_face_vertex_t resolveIndices(const obj_face_vertex_t& element) const;
        
        obj_group_t& createNewGroup(const std::string& name);
        std::string createMergedObjectGroupName(
            const std::string& objectName,
            const std::string& groupName) const;

    private:
        // Convert a relative index in a data array to an absolute array. A relative array is an index that is either
        // positive (points to an offset from the start) or negative (points to an offset from the end).
        // This method maintains the one based indexing nature of obj indices.
        static int relativeToAbsoluteIndex(int index, size_t count);

        obj_face_vertex_t readFaceElement(
            Daybreak::TextUtils::StringSplitter& arguments,
            const char * command,
            const char * field) const;

        float readFloat(
            Daybreak::TextUtils::StringSplitter& arguments,
            const char * command,
            const char * field) const;

        float parseFloat(
            const std::string_view& token,
            const char * command,
            const char * field) const;

        int readInt(
            Daybreak::TextUtils::StringSplitter& arguments,
            const char * command,
            const char * field) const;

        int parseInt(
            const std::string_view& token,
            const char * command,
            const char * field) const;

        std::string readString(
            Daybreak::TextUtils::StringSplitter& arguments,
            const char * command,
            const char * field) const;

    private:
        std::unique_ptr<obj_model_t> m_model;
        std::string m_fileName = "";
        size_t m_lineNumber = 1;
        std::string m_activeObjectName;
        std::string m_activeGroupName;
        std::string m_activeMaterialName;
        std::map<std::string, unsigned int> m_groups;
        bool m_firstFace = true;
    };
}
