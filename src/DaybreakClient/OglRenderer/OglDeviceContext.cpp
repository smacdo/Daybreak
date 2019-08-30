#include "stdafx.h"
#include "OglDeviceContext.h"
#include "OglIndexBuffer.h"
#include "OglInputLayout.h"
#include "OglTexture.h"
#include "OglShader.h"
#include "OglVertexBuffer.h"

using namespace Daybreak;
using namespace Daybreak::OpenGlRenderer;

//---------------------------------------------------------------------------------------------------------------------
OglDeviceContext::OglDeviceContext()
    : IDeviceContext()
{
}

//---------------------------------------------------------------------------------------------------------------------
OglDeviceContext::~OglDeviceContext()
{
    destroy();
}

//---------------------------------------------------------------------------------------------------------------------
std::unique_ptr<ITexture2d> OglDeviceContext::createTexture2d(
    const Image& image,
    const TextureParameters& params)
{
    return OglTexture2d::generate(image, params);
}

//---------------------------------------------------------------------------------------------------------------------
std::unique_ptr<IndexBuffer> OglDeviceContext::createIndexBuffer(const MeshData& mesh)
{
    return OglIndexBuffer::generate(mesh);
}

//---------------------------------------------------------------------------------------------------------------------
std::unique_ptr<InputLayout> OglDeviceContext::createInputLayout(const InputLayoutDescription& layoutDescription)
{
    return OglInputLayout::generate(layoutDescription);
}

//---------------------------------------------------------------------------------------------------------------------
std::unique_ptr<IShader> OglDeviceContext::compileShader(
    const std::string& shaderName,
    std::string_view vertexShaderProgram,
    std::string_view fragmentShaderProgram)
{
    return OglShader::generate(
        shaderName,
        std::string(vertexShaderProgram),           // TODO: Convert ogl shader to use string_view.
        std::string(fragmentShaderProgram));
}

//---------------------------------------------------------------------------------------------------------------------
std::unique_ptr<VertexBuffer> OglDeviceContext::createVertexBuffer(const MeshData& mesh)
{
    return OglVertexBuffer::generate(mesh);
}

//---------------------------------------------------------------------------------------------------------------------
void OglDeviceContext::destroy()
{
    // TODO: Not sure if needed.
}
