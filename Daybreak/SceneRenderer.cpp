#include "stdafx.h"
#include "SceneRenderer.h"
#include "Scene.h"
#include "Shader.h"
#include "ErrorHandling.h"
#include "Image.h"
#include "Texture.h"

#include <glad\glad.h>
#include <string>
#include <memory>
#include <cassert>

//---------------------------------------------------------------------------------------------------------------------
SceneRenderer::SceneRenderer()
{
    CreateDefaultScene();
}

//---------------------------------------------------------------------------------------------------------------------
SceneRenderer::~SceneRenderer()
{
}

//---------------------------------------------------------------------------------------------------------------------
void SceneRenderer::Render(const Scene& scene, float deltaSeconds)
{
    // Enable wireframe rendering if requested otherwise use solid mode.
    glPolygonMode(GL_FRONT_AND_BACK, IsWireframeEnabled() ? GL_LINE : GL_FILL);

    m_texture->Activate(0);
    m_texture2->Activate(1);

    m_shader->Activate();
    glBindVertexArray(m_standardVAO);

    // Update elapsed time and pass it to the shader.
    m_sceneSeconds += deltaSeconds;
    m_shader->SetFloat("renerTime", m_sceneSeconds);

    // Draw rect.
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    // Unbind vertex array.
    glBindVertexArray(0);

    // Make sure no errors happened while drawing.
    glCheckForErrors();
}

//---------------------------------------------------------------------------------------------------------------------
void SceneRenderer::CreateDefaultScene()
{
    // Create the standard VAO which defines Daybreak's standard vertex attribute layout.
    glGenVertexArrays(1, &m_standardVAO);
    glBindVertexArray(m_standardVAO);

    // Create a simple rectangle to render.
    // TODO: Remove this once scene loading is added.
    const float DefaultVertices[] =
    {
        // Positions.         // Colors.          // Texture.
         0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,     // Top right.
         0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,     // Bottom right.
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,     // Bottom left.
        -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f      // Top left.
    };

    const unsigned int DefaultIndices[] =
    {
        0, 1, 3,    // Left triangle.
        1, 2, 3     // Right triangle.
    };

    // Upload vertex data.
    unsigned int vbo = 0;
    glGenBuffers(1, &vbo);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glCheckForErrors();

    glBufferData(GL_ARRAY_BUFFER, sizeof(DefaultVertices), DefaultVertices, GL_STATIC_DRAW);
    glCheckForErrors();

    // Upload index data.
    unsigned int ebo = 0;
    glGenBuffers(1, &ebo);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glCheckForErrors();

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(DefaultIndices), DefaultIndices, GL_STATIC_DRAW);
    glCheckForErrors();

    // Construct scene shader.
    m_shader = std::move(
        Shader::LoadFromFile(
            "Standard",
            "Shaders\\Standard_vs.glsl",
            "Shaders\\Standard_fs.glsl")); 

    m_shader->Activate();

    m_shader->SetInt("texture1", 0);
    m_shader->SetInt("texture2", 1); 

    // Generate vertex attributes for the standard shader.
    // TODO: Move to nearer vertex definition.
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glCheckForErrors();

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glCheckForErrors();

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
    glCheckForErrors();
    
    // Load textures.
    auto image = Image::LoadFromFile("Content\\container.jpg");
    m_texture = Texture::Create2d(*image.get(), TextureParameters(), TextureFormat::RGB);

    auto image2 = Image::LoadFromFile("Content\\awesomeface.png");
    m_texture2 = Texture::Create2d(*image2.get(), TextureParameters(), TextureFormat::RGB);
}
