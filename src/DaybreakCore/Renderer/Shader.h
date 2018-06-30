#pragma once
#include <string>
#include "Renderer/ShaderAttribute.h"

namespace Daybreak
{
    // Render API independent shader interface.
    class IShader
    {
    public:
        // Constructor.
        IShader() = default;

        // Copy constructor (deleted).
        IShader(const IShader&) = delete;

        // Destructor.
        virtual ~IShader() = default;

        // Assignment operator.
        IShader& operator =(const IShader&) = delete;

        // Get shader uniform address.
        virtual ShaderVariable getVariable(const std::string& name) const = 0;

        // Get name of shader.
        virtual std::string name() const = 0;
    };
}