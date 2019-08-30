#include "stdafx.h"
#include "SceneRenderer.h"
#include "OglRenderer/OglRenderContext.h"
#include "OglRenderer/OglShader.h"
#include "OglRenderer/OglError.h"
#include "OglRenderer/OglInputLayout.h"
#include "OglRenderer/OglVertexBuffer.h"
#include "OglRenderer/OglIndexBuffer.h"
#include "OglRenderer/OglTexture.h"
#include "OglRenderer/OglPhongLightingEffect.h"
#include "OglRenderer\OglDeviceContext.h"
#include "Renderer/Mesh.h"
#include "Content\Models\ModelData.h"
#include "Renderer/Phong/PhongMaterial.h"
#include "Renderer/Phong/PhongLight.h"
#include "Renderer/RenderContext.h"
#include "Graphics/BasicGeometryGenerator.h"
#include "Graphics/Mesh/MeshData.h"
#include "Content\Images\Image.h"
#include "Scene/Scene.h"
#include "Scene/Camera.h"
#include "Content/ResourcesManager.h"
#include "Content/DefaultFileSystem.h"

#include <glad\glad.h>
#include <string>
#include <memory>
#include <cassert>

using namespace Daybreak;
using namespace Daybreak::OpenGlRenderer;

// XXX: CAMERA START
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
// XXX: CAMERA END

// TODO: Hack to store multiple cube positions. Want to do this correctly in the future with scene
//       graph.
std::vector<glm::vec3> GCubePositions = {
    { 0.0f, 0.0f, 0.0f },
    { 2.0f, 5.0f, -15.0f },
    {-1.5f,-2.2f, -2.5f },
    { -3.0f, 2.0f, 3.0f },
    { 8.5f, -6.2f, -3.5f },
    { 2.0f, 0.0f, -2.0f },
    { 3.0f, -5.0f, 6.0f },
    { 0.5f, -6.2f, 3.5f },
    { 3.0f, 2.0f, -3.0f },
    { 4.5f, 6.5f, -0.5f }
};

//---------------------------------------------------------------------------------------------------------------------
SceneRenderer::SceneRenderer(
    unsigned int viewportWidth,
    unsigned int viewportHeight)
    : m_camera(std::make_shared<Camera>(glm::vec3{ 0.0f, 0.0f, 3.0f }))
{
    SetViewportSize(viewportWidth, viewportHeight);
    CreateDefaultScene();
}

//---------------------------------------------------------------------------------------------------------------------
SceneRenderer::~SceneRenderer()
{
}

//---------------------------------------------------------------------------------------------------------------------
void SceneRenderer::Render(const Daybreak::Scene& scene, timespan_t deltaTime)
{
    // Clear display from last render.
    m_renderContext->clearColorAndDepthBuffers();

    // Enable wireframe rendering if requested otherwise use solid mode.
    m_renderContext->setWireframeEnabled(IsWireframeEnabled());
    
    // Bind the standard shader and standard vertex layout for cube rendering.
    m_renderContext->bindInputLayout(m_standardInputLayout);        // TODO: Should happen when changing mesh vformat.
    
    // Update elapsed time and pass it to the shader.
    m_renderTime += deltaTime;
//    m_renderContext->setShaderFloat(
//        m_standardShader->getVariable("renderTime"),
//        static_cast<float>(m_renderTime.totalSeconds()));
  
    // Set per-pass shader parameters
    m_phong->setCamera(m_camera);
    m_phong->setMaterial(m_mesh->material());

    m_phong->setDirectionalLightCount(m_scene->directionalLightCount());

    for (size_t i = 0; i < m_scene->directionalLightCount(); ++i)
    {
        m_phong->setDirectionalLight(i, m_scene->directionalLight(i));
    }

    m_phong->setPointLightCount(m_scene->pointLightCount());

    for (size_t i = 0; i < m_scene->pointLightCount(); ++i)
    {
        m_phong->setPointLight(i, m_scene->pointLight(i));
    }

    // Render each box.
    m_phong->startPass(*m_renderContext.get());

    m_renderContext->bindVertexBuffer(m_mesh->vertexBuffer());
    m_renderContext->bindIndexBuffer(m_mesh->indexBuffer());

    float i = 1;

    for (const auto& position : GCubePositions)
    {
        // Create cube model matrix by translating to the correct position and then rotating over time.
        glm::mat4 model(1);
        model = glm::translate(model, position);

        auto rotateOverTime = m_renderTime.totalSeconds() * glm::radians(20.0f * (i + 1) + 20.0f * i);
        model = glm::rotate(model, (float)rotateOverTime, glm::vec3(1.0f, 0.3f, 0.5f));

        i += 1.0f;

        m_phong->setModelMatrix(model);

        // Generate normal transformation matrix by taking the transpose of the inverse of the upper left corner
        // of the model matrix.
        glm::mat3 normal;
        
        normal[0][0] = model[0][0]; normal[0][1] = model[0][1]; normal[0][2] = model[0][2];
        normal[1][0] = model[1][0]; normal[1][1] = model[1][1]; normal[1][2] = model[1][2];
        normal[2][0] = model[2][0]; normal[2][1] = model[2][1]; normal[2][2] = model[2][2];

        normal = glm::inverse(normal);
        normal = glm::transpose(normal);
        
        m_phong->setNormalMatrix(normal);

        // Draw cube.
        m_phong->startRenderObject(
            *m_renderContext.get(),
            0,
            static_cast<unsigned int>(m_mesh->indexBuffer()->elementCount()));
    }

    // Finish pass.
    m_phong->finishPass(*m_renderContext.get());

    // Draw the lamp.
    m_renderContext->bindShader(m_lightDebugShader);
    
    auto view = m_camera->view();
    auto projection = m_camera->perspective();

    m_renderContext->setShaderMatrix4(m_lightDebugShader->getVariable("view"), view);
    m_renderContext->setShaderMatrix4(m_lightDebugShader->getVariable("projection"), projection);;

    glm::mat4 model(1);

    model = glm::translate(model, m_scene->pointLight(0).position());
    model = glm::scale(model, glm::vec3{ 0.2f, 0.2f, 0.2f });

    m_renderContext->setShaderMatrix4(m_lightDebugShader->getVariable("model"), model);
    m_renderContext->setShaderVector3f(m_lightDebugShader->getVariable("tint"), m_scene->pointLight(0).diffuseColor());
     
    m_renderContext->drawTriangles(0, static_cast<unsigned int>(m_mesh->indexBuffer()->elementCount()));
}

//---------------------------------------------------------------------------------------------------------------------
void SceneRenderer::CreateDefaultScene()
{
    m_deviceContext = std::make_shared<OglDeviceContext>();
    m_renderContext = std::make_unique<OglRenderContext>();

    m_resources = std::make_shared<ResourcesManager>(
        m_deviceContext,
        std::make_shared<DefaultFileSystem>("Content"));

    // Enable depth testing by default.
    m_renderContext->setDepthTestEnabled(true);

    // Create the default scene.
    m_scene = std::make_unique<Scene>();
    m_camera->setPerspective(45.0f, 0.1f, 100.0f);

    // Create a simple cube to render.
    auto modelData = m_resources->loadModel("cube.obj");
    auto material = std::make_shared<PhongMaterial>(modelData->group(0).materialRef());

    m_mesh = std::make_unique<Mesh>(
        OglVertexBuffer::generate(modelData->mesh()),
        OglIndexBuffer::generate(modelData->mesh()),
        material);

    // Generate vertex attributes for the standard shader.
    // TODO: Move this work into a fluent-ish interface inside InputLayout.
    m_renderContext->bindVertexBuffer(m_mesh->vertexBuffer());

    // Create the standard VAO which defines Daybreak's standard vertex attribute layout.
    //  TODO: Use render context -> device -> createInputLayout
    auto standardVertexAttributes = std::make_shared<InputLayoutDescription>(std::vector<InputAttribute>
    {
        { InputAttribute::SemanticName::Position, 0, InputAttribute::StorageType::Float, 3 },
        { InputAttribute::SemanticName::Texture, 0, InputAttribute::StorageType::Float, 2 },
        { InputAttribute::SemanticName::Normal, 0, InputAttribute::StorageType::Float, 3 }
    });

    m_standardInputLayout = OglInputLayout::generate(
        standardVertexAttributes,
        m_renderContext,
        m_mesh->vertexBuffer());

    // Construct scene shader.
    m_phong = std::make_unique<OglPhongLightingEffect>(
        OglShader::generateFromFile(
            "Standard",
            "Content\\Shaders\\Standard_vs.glsl",
            "Content\\Shaders\\Standard_fs.glsl"));

    m_lightDebugShader = std::move(
        OglShader::generateFromFile(
            "Standard",
            "Content\\Shaders\\LightDebug_vs.glsl",
            "Content\\Shaders\\LightDebug_fs.glsl"));

    // Configure scene lights.
    m_scene->setDirectionalLightCount(1);
    m_scene->setDirectionalLight(0, {
        { -0.2f, -1.0f, -0.3f },    // direction.
        { 0.0f, 0.0f, 0.0f },       // ambient.
        { 0.0f, 0.0f, 1.0f },       // diffuse.
        { 1.0f, 1.0f, 1.0f }        // specular.
    });

    m_scene->setPointLightCount(1);
    m_scene->setPointLight(0,{
        { 1.2f, 0.0f, 2.0f },       // position.
        1.0f, 0.09f, 0.032f,        // constant, linear, quadratic coefficients.
        { 0.4f, 0.4f, 0.5f },       // ambient.
        { 0.8f, 0.8f, 1.0f },       // diffuse.
        { 1.0f, 1.0f, 1.0f }        // specular.
    });
}

//---------------------------------------------------------------------------------------------------------------------
void SceneRenderer::SetViewportSize(unsigned int width, unsigned int height)
{
    assert(width > 0 && "Viewport width must be larger than zero");
    assert(height > 0 && "Viewport height must be larger than zero");

    CHECK_NOT_NULL(m_camera);
    m_camera->setViewportSize(width, height);

    glViewport(0, 0, static_cast<int>(width), static_cast<int>(height));
}
