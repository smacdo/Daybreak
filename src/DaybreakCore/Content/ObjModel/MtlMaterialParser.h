#pragma once
#include <string>
#include <memory>
#include <vector>

namespace Daybreak
{
    class MaterialData;

    /** Configurable parser for mtl materials. */
    class MtlMaterialParser
    {
    public:
        /** Return a list of materials parsed from the provided mtl file text data. */
        std::vector<std::unique_ptr<MaterialData>>&& parse(
            const std::string_view& mtlData,
            const std::string& fileName = "");

    private:
        /** Evaluate one line from the obj file. */
        void parseLine(const std::string_view& line);

        /** Reset the state of the parser. */
        void reset() noexcept;

        /** Get the active material. */
        MaterialData& currentMaterial();

    private:
        std::string m_fileName;
        size_t m_lineNumber;
        std::vector<std::unique_ptr<MaterialData>> m_materials;
    };
}
