#include "stdafx.h"
#include "SceneRenderer.h"
#include "Shader.h"
#include "OglRenderer/OglError.h"
#include "OglRenderer/OglInputLayout.h"
#include "OglRenderer/OglVertexBuffer.h"
#include "OglRenderer/OglIndexBuffer.h"
#include "OglRenderer/OglTexture.h"
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
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Enable wireframe rendering if requested otherwise use solid mode.
    glPolygonMode(GL_FRONT_AND_BACK, IsWireframeEnabled() ? GL_LINE : GL_FILL);
    
    // Bind the standard shader and standard vertex layout for cube rendering.
    m_standardShader->Activate();
    m_standardInputLayout->bind();
    
    // Update elapsed time and pass it to the shader.
    m_renderTime += deltaTime;
    m_standardShader->SetFloat("renderTime", static_cast<float>(m_renderTime.totalSeconds()));

    // Pass camera transformation matrices to the shader.
    //  Rotate objects around camera.
    float radius = 10.0f;
    float cameraX = (float)sin(m_renderTime.totalSeconds()) * radius;
    float cameraZ = (float)cos(m_renderTime.totalSeconds()) * radius;

    // Copy camera matrix to shader.
    auto view = m_camera->view();

    m_standardShader->SetMatrix4("view", view);
    m_standardShader->SetVector3f("viewPos", m_camera->position());

    // Copy projection matrix to shader.
    glm::mat4 projection(1);
    projection = glm::perspective(
        glm::radians(45.0f),
        static_cast<float>(m_viewportWidth) / static_cast<float>(m_viewportHeight),
        0.1f,
        100.0f);

    m_standardShader->SetMatrix4("projection", projection);

    // Set material shader params.
    m_diffuseTexture->bind(0);
    m_specularTexture->bind(1);

    m_standardShader->SetVector3f("material.ambientColor", 0.0f, 0.0f, 0.0f);
    m_standardShader->SetInt("material.diffuse", 0);
    m_standardShader->SetVector3f("material.diffuseColor", 0.0f, 0.0f, 0.0f);
    m_standardShader->SetBool("material.hasDiffuseTexture", true);
    m_standardShader->SetInt("material.specular", 1);
    m_standardShader->SetVector3f("material.specularColor", 0.0f, 0.0f, 0.0f);
    m_standardShader->SetBool("material.hasSpecularTexture", true);
    m_standardShader->SetFloat("material.shininess", 32.0f);

    // Set lighting shader params.
    m_standardShader->SetInt("directionalLightCount", 1);
    m_standardShader->SetVector3f("directionalLights[0].direction", { -0.2f, -1.0f, -0.3f });
    m_standardShader->SetVector3f("directionalLights[0].ambient", { 0.0f, 0.0f, 0.0f });
    m_standardShader->SetVector3f("directionalLights[0].diffuse", { 0.0f, 0.0f, 1.0f });
    m_standardShader->SetVector3f("directionalLights[0].specular", 1.0f, 1.0f, 1.0f);

    m_standardShader->SetInt("pointLightCount", 1);
    m_standardShader->SetVector3f("pointLights[0].position", m_lightPos);
    m_standardShader->SetVector3f("pointLights[0].ambient", m_lightColor / 2.0f);
    m_standardShader->SetVector3f("pointLights[0].diffuse", m_lightColor);
    m_standardShader->SetVector3f("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
    m_standardShader->SetFloat("pointLights[0].constant", 1.0f);
    m_standardShader->SetFloat("pointLights[0].linear", 0.09f);
    m_standardShader->SetFloat("pointLights[0].quadratic", 0.032f);
    
    // Render each box.
    m_vertexBuffer->bind();
    m_indexBuffer->bind();

    float i = 1;

    for (const auto& position : GCubePositions)
    {
        // Create cube model matrix by translating to the correct position and then rotating over time.
        glm::mat4 model(1);
        model = glm::translate(model, position);

        auto rotateOverTime = m_renderTime.totalSeconds() * glm::radians(20.0f * (i + 1) + 20.0f * i);
        model = glm::rotate(model, (float)rotateOverTime, glm::vec3(1.0f, 0.3f, 0.5f));

        i += 1.0f;

        m_standardShader->SetMatrix4("model", model);

        // Generate normal transformation matrix by taking the transpose of the inverse of the upper left corner
        // of the model matrix.
        glm::mat3 normal;
        
        normal[0][0] = model[0][0]; normal[0][1] = model[0][1]; normal[0][2] = model[0][2];
        normal[1][0] = model[1][0]; normal[1][1] = model[1][1]; normal[1][2] = model[1][2];
        normal[2][0] = model[2][0]; normal[2][1] = model[2][1]; normal[2][2] = model[2][2];

        normal = glm::inverse(normal);
        normal = glm::transpose(normal);
        
        m_standardShader->SetMatrix3("normalMatrix", normal);

        // Draw cube.
        //glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    // Draw the lamp.
    m_lightDebugShader->Activate();

    m_lightDebugShader->SetMatrix4("view", view);
    m_lightDebugShader->SetMatrix4("projection", projection);

    glm::mat4 model(1);

    model = glm::translate(model, m_lightPos);
    model = glm::scale(model, glm::vec3{ 0.2f, 0.2f, 0.2f });

    m_lightDebugShader->SetMatrix4("model", model);

    m_lightDebugShader->SetVector3f("tint", m_lightColor);

    m_lightInputLayout->bind();
     
    glDrawArrays(GL_TRIANGLES, 0, 36);
    //glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    // Unbind vertex array.
    glBindVertexArray(0);

    // Make sure no errors happened while drawing.
    glCheckForErrors();
}

//---------------------------------------------------------------------------------------------------------------------
void SceneRenderer::CreateDefaultScene()
{
    // Enable depth testing by default.
    glEnable(GL_DEPTH_TEST);

    // Create the standard VAO which defines Daybreak's standard vertex attribute layout.
    m_standardInputLayout = OglInputLayout::Generate();

    // Create a simple cube to render.
    auto cubeVertices = BasicGeometryGenerator::MakeCubeVertices();
    auto cubeIndices = BasicGeometryGenerator::MakeCubeIndices();

    m_vertexBuffer = OglVertexBuffer::generate(
        cubeVertices.data(),
        sizeof(BasicGeometryGenerator::VertexValueType),
        cubeVertices.size());

    m_indexBuffer = OglIndexBuffer::generate(
        cubeIndices.data(),
        sizeof(BasicGeometryGenerator::IndexValueType),
        cubeIndices.size());

    // Generate vertex attributes for the standard shader.
    // TODO: Move this work into a fluent-ish interface inside InputLayout.
    m_standardInputLayout->bind();

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glCheckForErrors();

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glCheckForErrors();

    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
    glEnableVertexAttribArray(2);
    glCheckForErrors();

    // Create debug light input layout.
    m_lightInputLayout = OglInputLayout::Generate();
    m_lightInputLayout->bind();

    m_vertexBuffer->bind();
    m_indexBuffer->bind(); // ?? not sure if needed

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);

    // Construct scene shader.
    m_standardShader = std::move(
        Shader::LoadFromFile(
            "Standard",
            "Content\\Shaders\\Standard_vs.glsl",
            "Content\\Shaders\\Standard_fs.glsl")); 

    m_lightDebugShader = std::move(
        Shader::LoadFromFile(
            "Standard",
            "Content\\Shaders\\LightDebug_vs.glsl",
            "Content\\Shaders\\LightDebug_fs.glsl"));

    // Load textures.
    auto image = Image::LoadFromFile("Content\\cube_diffuse.png");
    m_diffuseTexture = OglTexture2d::generate(*image.get(), TextureParameters(), TextureFormat::RGB);

    auto image2 = Image::LoadFromFile("Content\\cube_specular.png");
    m_specularTexture = OglTexture2d::generate(*image2.get(), TextureParameters(), TextureFormat::RGB);
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
