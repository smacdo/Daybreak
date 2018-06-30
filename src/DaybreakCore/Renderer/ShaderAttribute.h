#pragma once

namespace Daybreak
{
    // Shader variable.
    class ShaderVariable
    {
    public:
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
