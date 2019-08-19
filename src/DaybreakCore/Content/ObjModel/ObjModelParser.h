#pragma once
#include <string>
#include <memory>
#include <vector>
#include <map>
#include <glm/glm.hpp>

#include "Utility/TextUtils.h" // TODO: Remove!

namespace Daybreak
{
    class MeshData;

    /** Obj model data. Note indices are one based and not zero based! */
    struct obj_face_t
    {
        int position[3];
        int uv[3];
        int normal[3];

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

    /** One vertex in an obj face. Note indices are one based not zero based! */
    struct obj_face_vertex_t
    {
        int p = 0;
        int t = 0;
        int n = 0;

        /** Check if element has a valid index into the texture data array. */
        bool hasUV() const noexcept { return t != 0; }

        /** Check if element has a valid index into the normal data array. */
        bool hasNormals() const noexcept { return n != 0; }

        /** Equality operator. */
        bool operator ==(const obj_face_vertex_t& rhs) const
        {
            return p == rhs.p && t == rhs.t && n == rhs.n;
        }
    };

    /** A named group of faces in a mesh with an optional material. */
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

    /** All data associated with an obj model. */
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

    /** Configurable loader for obj models. */
    class ObjModelParser
    {
    public:
        static const std::string DefaultGroupName;

        /** Return an obj model parsed from the provided obj file text data. */
        std::unique_ptr<obj_model_t> parse(const std::string_view& objData, const std::string& fileName = "");

    private:
        /** Evaluate one line from the obj file. */
        void parseLine(const std::string_view& line);

        /** Reset the state of the parser. */
        void reset() noexcept;

        /** Get the last created group in the obj model. If none it defaults to the DefaultGroup. */
        obj_group_t& currentGroup() noexcept;

        /**
         * Instruct parser to use the given group name for following faces. This will create a new group with the
         * active object name and this group name if the current group has existing faces.
         */
        void useGroupName(const std::string& groupName);

        /** 
         * Instruct parser to use the given object name for following faces. This will create a new group with this
         * object name and the active group name if the current group has existing faces.
         */
        void useObjectName(const std::string& objectName);

        /** 
         * Instruct parser to use the given material name for following faces. This will create a new group with the
         * active object and group name if the current group has existing faces. 
         */
        void useMaterial(const std::string& materialName);

        /** 
         * Creates a new group with the active object and group name if the current group has existing faces. Otherwise
         * if the current group has no faces this will change the current group's name to be the active object and
         * group name.
         */
        void startObjectGroupName();

        /** Adds a new group to the obj model and returns it. */
        obj_group_t& createNewGroup(const std::string& name);

        /**
         * Reads the provided arguments and returns a face element (with absolute one based indices).
         */
        obj_face_t readFace(Daybreak::TextUtils::StringSplitter& arguments);

        /**
         * Convert a face vertex from relative indices to absolute indices. This maintains the one based index nature
         * of obj indices.
         */
        obj_face_vertex_t resolveIndices( 
            const obj_face_vertex_t& element,   //< Face vertex to modify.
            const char * command,               //< Obj command that resulted in this function being called.
            const char * field) const;          //< Obj command field that resulted in this function being called.
        
        /** Generates a merged obj object and group name. */
        std::string createMergedObjectGroupName(
            const std::string& objectName,      //< Active object name to use (optional, empty for none).
            const std::string& groupName) const;//< Active group name to use (optional, empty for none).

    private:
        /**
         * Convert a relative index in a data array to an absolute array. A relative array is an index that is either
         * positive (points to an offset from the start) or negative (points to an offset from the end). Note that this
         * method maintains the one based index nature of obj indices.
         */
        int relativeToAbsoluteIndex(
            int relativeIndex,
            size_t arrayLength,
            const char * command,
            const char * field) const;

        /** Returns a face vertex parsed from the provided arguments. */
        obj_face_vertex_t readFaceElement(
            Daybreak::TextUtils::StringSplitter& arguments, //< Arguments from face command.
            const char * command,           //< Obj command that resulted in this function being called.
            const char * field) const;      //< Obj command field that resulted in this function being called.

    private:
        std::unique_ptr<obj_model_t> m_model;
        std::string m_fileName = "";
        size_t m_lineNumber = 1;
        std::string m_activeObjectName;
        std::string m_activeGroupName;
        std::string m_activeMaterialName;
        bool m_firstFace = true;
    };
}
