#include "stdafx.h"
#include "SceneRenderer.h"
#include "Scene.h"
#include "Shader.h"
#include "Common/Error.h"
#include "Graphics/Image.h"
#include "Scene/Camera.h"
#include "Texture.h"

#include <glad\glad.h>
#include <string>
#include <memory>
#include <cassert>

using namespace Daybreak;

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
    {-1.5f,-2.2f, -2.5f }
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
void SceneRenderer::Render(const Scene& scene, const TimeSpan& deltaTime)
{
    // Clear display from last render.
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Enable wireframe rendering if requested otherwise use solid mode.
    glPolygonMode(GL_FRONT_AND_BACK, IsWireframeEnabled() ? GL_LINE : GL_FILL);

    // Bind textures (texture0 is main texture, texture1 is additional blend texture).
    m_texture->Activate(0);
    m_texture2->Activate(1);

    // Bind the standard shader and standard vertex layout for cube rendering.
    m_shader->Activate();
    glBindVertexArray(m_standardVAO);

    // Update elapsed time and pass it to the shader.
    m_renderTime += deltaTime;
    m_shader->SetFloat("renderTime", static_cast<float>(m_renderTime.totalSeconds()));

    // Pass camera transformation matrices to the shader.
    //  Rotate objects around camera.
    float radius = 10.0f;
    float cameraX = (float)sin(m_renderTime.totalSeconds()) * radius;
    float cameraZ = (float)cos(m_renderTime.totalSeconds()) * radius;

    //m_camera->setPosition(glm::vec3{ cameraX, 0.0f, cameraZ });

    // Copy camera matrix to shader.
    auto view = m_camera->view();
    m_shader->SetMatrix4("view", view);

    // Copy projection matrix to shader.
    glm::mat4 projection(1);
    projection = glm::perspective(
        glm::radians(45.0f),
        static_cast<float>(m_viewportWidth) / static_cast<float>(m_viewportHeight),
        0.1f,
        100.0f);

    m_shader->SetMatrix4("projection", projection);      // TODO: This doesn't change often, don't upload every time.

    // Render each box.
    for (const auto& position : GCubePositions)
    {
        // Create cube model matrix by translating to the correct position and then rotating over time.
        glm::mat4 model(1);
        model = glm::translate(model, position);

        auto rotateOverTime = m_renderTime.totalSeconds() * glm::radians(50.0f);
        model = glm::rotate(model, (float)rotateOverTime, glm::vec3(0.5f, 1.0f, 0.3f));

        m_shader->SetMatrix4("model", model);

        // Draw cube.
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    }

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
    glGenVertexArrays(1, &m_standardVAO);
    glBindVertexArray(m_standardVAO);

    // TODO: Remove this once scene loading is added.

    // Create a simple cube to render.
    // TODO: Remove this once scene loading is added.
    const float CubeVertices[] =
    {
        //  x      y      z       u     v
        -0.5f, -0.5f, -0.5f,   0.0f, 0.0f,          // Front
         0.5f, -0.5f, -0.5f,   1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,   1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,   1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,   0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,   0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,   0.0f, 0.0f,          // Back
         0.5f, -0.5f,  0.5f,   1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,   1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,   1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,   0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,   0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,   1.0f, 0.0f,          // Left
        -0.5f,  0.5f, -0.5f,   1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,   0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,   0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,   0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,   1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,   1.0f, 0.0f,          // Right
         0.5f,  0.5f, -0.5f,   1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,   0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,   0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,   0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,   1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,   0.0f, 1.0f,          // Bottom
         0.5f, -0.5f, -0.5f,   1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,   1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,   1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,   0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,   0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,   0.0f, 1.0f,          // Top
         0.5f,  0.5f, -0.5f,   1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,   1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,   1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,   0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,   0.0f, 1.0f,
    };

    const unsigned int CubeIndices[] =
    {
         0,  1,  2,  3,  4,  5,
         6,  7,  8,  9, 10, 11,
        12, 13, 14, 15, 16, 17,
        18, 19, 20, 21, 22, 23,
        24, 25, 26, 27, 28, 29,
        30, 31, 32, 33, 34, 35
    };

    // Upload vertex data.
    unsigned int vbo = 0;
    glGenBuffers(1, &vbo);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glCheckForErrors();

    glBufferData(GL_ARRAY_BUFFER, sizeof(CubeVertices), CubeVertices, GL_STATIC_DRAW);
    glCheckForErrors();

    // Upload index data.
    unsigned int ebo = 0;
    glGenBuffers(1, &ebo);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glCheckForErrors();

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(CubeIndices), CubeIndices, GL_STATIC_DRAW);
    glCheckForErrors();

    // Construct scene shader.
    m_shader = std::move(
        Shader::LoadFromFile(
            "Standard",
            "Content\\Shaders\\Standard_vs.glsl",
            "Content\\Shaders\\Standard_fs.glsl")); 

    m_shader->Activate();

    m_shader->SetInt("texture1", 0);
    m_shader->SetInt("texture2", 1); 

    // Generate vertex attributes for the standard shader.
    // TODO: Move to nearer vertex definition.
    // glVertexAttribPointer(slot, numberOfComponents, componentDataType, normalized?, stride, firstOffset);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glCheckForErrors();

//    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
//    glEnableVertexAttribArray(1);
//    glCheckForErrors();

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glCheckForErrors();
    
    // Load textures.
    auto image = Image::LoadFromFile("Content\\container.jpg");
    m_texture = Texture::Create2d(*image.get(), TextureParameters(), TextureFormat::RGB);

    auto image2 = Image::LoadFromFile("Content\\awesomeface.png");
    m_texture2 = Texture::Create2d(*image2.get(), TextureParameters(), TextureFormat::RGB);
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
