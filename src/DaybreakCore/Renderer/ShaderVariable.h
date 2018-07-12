#pragma once
#include <limits>

namespace Daybreak
{
    // Shader variable.
    class ShaderVariable
    {
    public:
        // Constructor (invalid address).
        ShaderVariable()
            : m_address(std::numeric_limits<unsigned int>::max())
        {
        }

        // Constructor.
        explicit ShaderVariable(unsigned int address)
            : m_address(address)
        {
        }

        // Get address for binding values.
        unsigned int address() const noexcept { return m_address; }

    private:
        unsigned int m_address = 0;
    };
}
