#include "stdafx.h"
#include "SceneRenderer.h"
#include "OglRenderer/OglRenderContext.h"
#include "OglRenderer/OglShader.h"
#include "OglRenderer/OglError.h"
#include "OglRenderer/OglInputLayout.h"
#include "OglRenderer/OglVertexBuffer.h"
#include "OglRenderer/OglIndexBuffer.h"
#include "OglRenderer/OglTexture.h"
#include "Renderer/Mesh.h"
#include "Renderer/Phong/PhongMaterial.h"
#include "Renderer/RenderContext.h"
#include "Graphics/BasicGeometryGenerator.h"
#include "Graphics/Image.h"
#include "Scene/Scene.h"
#include "Scene/Camera.h"

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
void SceneRenderer::Render(const Daybreak::Scene& scene, const TimeSpan& deltaTime)
{
    // Clear display from last render.
    m_renderContext->clearColorAndDepthBuffers();

    // Enable wireframe rendering if requested otherwise use solid mode.
    m_renderContext->setWireframeEnabled(IsWireframeEnabled());
    
    // Bind the standard shader and standard vertex layout for cube rendering.
    m_renderContext->bindShader(m_standardShader);
    m_renderContext->bindInputLayout(m_standardInputLayout);
    
    // Update elapsed time and pass it to the shader.
    m_renderTime += deltaTime;
    m_standardShader->setFloat("renderTime", static_cast<float>(m_renderTime.totalSeconds()));

    // Pass camera transformation matrices to the shader.
    //  Rotate objects around camera.
    float radius = 10.0f;
    float cameraX = (float)sin(m_renderTime.totalSeconds()) * radius;
    float cameraZ = (float)cos(m_renderTime.totalSeconds()) * radius;

    // Copy camera matrix to shader.
    auto view = m_camera->view();

    m_standardShader->setMatrix4("view", view);
    m_standardShader->setVector3f("viewPos", m_camera->position());

    // Copy projection matrix to shader.
    glm::mat4 projection(1);
    projection = glm::perspective(
        glm::radians(45.0f),
        static_cast<float>(m_viewportWidth) / static_cast<float>(m_viewportHeight),
        0.1f,
        100.0f);

    m_standardShader->setMatrix4("projection", projection);

    // Set material shader params.
    auto material = m_mesh->material();

    if (material->hasDiffuseTexture())
    {
        m_renderContext->bindTexture(material->diffuseTexture(), 0);

        m_standardShader->setInt("material.diffuse", 0);
        m_standardShader->setBool("material.hasDiffuseTexture", true);
    }
    else
    {
        m_standardShader->setBool("material.hasDiffuseTexture", false);
        m_standardShader->setVector3f("material.ambientColor", material->ambientColor());
        m_standardShader->setVector3f("material.diffuseColor", material->diffuseColor());
    }
    
    if (material->hasSpecularTexture())
    {
        m_renderContext->bindTexture(material->specularTexture(), 1);

        m_standardShader->setInt("material.specular", 1);
        m_standardShader->setBool("material.hasSpecularTexture", true);
    }
    else
    {
        m_standardShader->setBool("material.hasSpecularTexture", false);
        m_standardShader->setVector3f("material.specularColor", material->specularColor());
    }
    
    m_standardShader->setFloat("material.shininess", material->shininess());

    // Set lighting shader params.
    m_standardShader->setInt("directionalLightCount", 1);
    m_standardShader->setVector3f("directionalLights[0].direction", { -0.2f, -1.0f, -0.3f });
    m_standardShader->setVector3f("directionalLights[0].ambient", { 0.0f, 0.0f, 0.0f });
    m_standardShader->setVector3f("directionalLights[0].diffuse", { 0.0f, 0.0f, 1.0f });
    m_standardShader->setVector3f("directionalLights[0].specular", 1.0f, 1.0f, 1.0f);

    m_standardShader->setInt("pointLightCount", 1);
    m_standardShader->setVector3f("pointLights[0].position", m_lightPos);
    m_standardShader->setVector3f("pointLights[0].ambient", m_lightColor / 2.0f);
    m_standardShader->setVector3f("pointLights[0].diffuse", m_lightColor);
    m_standardShader->setVector3f("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
    m_standardShader->setFloat("pointLights[0].constant", 1.0f);
    m_standardShader->setFloat("pointLights[0].linear", 0.09f);
    m_standardShader->setFloat("pointLights[0].quadratic", 0.032f);
    
    // Render each box.
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

        m_standardShader->setMatrix4("model", model);

        // Generate normal transformation matrix by taking the transpose of the inverse of the upper left corner
        // of the model matrix.
        glm::mat3 normal;
        
        normal[0][0] = model[0][0]; normal[0][1] = model[0][1]; normal[0][2] = model[0][2];
        normal[1][0] = model[1][0]; normal[1][1] = model[1][1]; normal[1][2] = model[1][2];
        normal[2][0] = model[2][0]; normal[2][1] = model[2][1]; normal[2][2] = model[2][2];

        normal = glm::inverse(normal);
        normal = glm::transpose(normal);
        
        m_standardShader->setMatrix3("normalMatrix", normal);

        // Draw cube.
        m_renderContext->drawTriangles(0, 36);
    }

    // Draw the lamp.
    m_renderContext->bindShader(m_lightDebugShader);

    m_lightDebugShader->setMatrix4("view", view);
    m_lightDebugShader->setMatrix4("projection", projection);

    glm::mat4 model(1);

    model = glm::translate(model, m_lightPos);
    model = glm::scale(model, glm::vec3{ 0.2f, 0.2f, 0.2f });

    m_lightDebugShader->setMatrix4("model", model);

    m_lightDebugShader->setVector3f("tint", m_lightColor);

    m_renderContext->bindInputLayout(m_lightInputLayout);
     
    m_renderContext->drawTriangles(0, 36);
}

//---------------------------------------------------------------------------------------------------------------------
void SceneRenderer::CreateDefaultScene()
{
    m_renderContext = std::make_unique<OglRenderContext>();

    // Enable depth testing by default.
    m_renderContext->setDepthTestEnabled(true);

    // Load textures.
    //  TODO: Use render context -> device -> createTexture
    auto diffuseImage = Image::LoadFromFile("Content\\cube_diffuse.png");
    auto diffuseTexture = OglTexture2d::generate(*diffuseImage.get(), TextureParameters(), TextureFormat::RGB);

    auto specularImage = Image::LoadFromFile("Content\\cube_specular.png");
    auto specularTexture = OglTexture2d::generate(*specularImage.get(), TextureParameters(), TextureFormat::RGB);


    // Create a simple cube to render.
    //  TODO: Use render context -> device -> createXXX
    auto cubeVertices = BasicGeometryGenerator::MakeCubeVertices();
    auto cubeIndices = BasicGeometryGenerator::MakeCubeIndices();

    auto vertexBuffer = OglVertexBuffer::generate(
        cubeVertices.data(),
        sizeof(BasicGeometryGenerator::VertexValueType),
        cubeVertices.size());

    auto indexBuffer = OglIndexBuffer::generate(
        cubeIndices.data(),
        sizeof(BasicGeometryGenerator::IndexValueType),
        cubeIndices.size());

    auto material = std::make_shared<PhongMaterial>();

    material->setDiffuseTexture(std::move(diffuseTexture));
    material->setSpecularTexture(std::move(specularTexture));
    material->setShininess(32.0f);

    m_mesh = std::make_unique<Mesh>(std::move(vertexBuffer), std::move(indexBuffer), material);

    // Generate vertex attributes for the standard shader.
    // TODO: Move this work into a fluent-ish interface inside InputLayout.
    m_renderContext->bindVertexBuffer(m_mesh->vertexBuffer());

    // Create the standard VAO which defines Daybreak's standard vertex attribute layout.
    //  TODO: Use render context -> device -> createInputLayout
    std::vector<IInputLayout::attribute_t> standardLayoutSlots =
    {
        { 0, IInputLayout::ElementType::Float, 3, false },
        { 1, IInputLayout::ElementType::Float, 2, false },
        { 2, IInputLayout::ElementType::Float, 3, false }
    };

    m_standardInputLayout = OglInputLayout::generate(
        standardLayoutSlots,
        m_renderContext,
        m_mesh->vertexBuffer());

    // Create debug light input layout.
    std::vector<IInputLayout::attribute_t> debugLightLayoutSlots =
    {
        { 0, IInputLayout::ElementType::Float, 3, false }
    };

    // TODO: Need more support in input layout for more complex layouts like the debug light.
    //  (eg notice how it makes the attribute pointer 0 be 8 elements for padding).
    m_lightInputLayout = OglInputLayout::generate(
        debugLightLayoutSlots,
        m_renderContext,
        m_mesh->vertexBuffer());

    // TODO: Do we need to rebind for light?
    m_renderContext->bindVertexBuffer(m_mesh->vertexBuffer());
    m_renderContext->bindIndexBuffer(m_mesh->indexBuffer());
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
    
    // Construct scene shader.
    m_standardShader = std::move(
        OglShader::generateFromFile(
            "Standard",
            "Content\\Shaders\\Standard_vs.glsl",
            "Content\\Shaders\\Standard_fs.glsl")); 

    m_lightDebugShader = std::move(
        OglShader::generateFromFile(
            "Standard",
            "Content\\Shaders\\LightDebug_vs.glsl",
            "Content\\Shaders\\LightDebug_fs.glsl"));
}

//---------------------------------------------------------------------------------------------------------------------
void SceneRenderer::SetViewportSize(unsigned int width, unsigned int height)
{
    assert(width > 0 && "Viewport width must be larger than zero");
    assert(height > 0 && "Viewport height must be larger than zero");

    m_viewportWidth = width;
    m_viewportHeight = height;

    glViewport(0, 0, static_cast<int>(m_viewportWidth), static_cast<int>(m_viewportHeight));
}
