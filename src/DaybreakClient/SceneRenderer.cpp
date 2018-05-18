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

using namespace Daybreak;

// XXX: CAMERA START
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
// XXX: CAMERA END

//---------------------------------------------------------------------------------------------------------------------
SceneRenderer::SceneRenderer(unsigned int viewportWidth, unsigned int viewportHeight)
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
    // Enable wireframe rendering if requested otherwise use solid mode.
    glPolygonMode(GL_FRONT_AND_BACK, IsWireframeEnabled() ? GL_LINE : GL_FILL);

    m_texture->Activate(0);
    m_texture2->Activate(1);

    m_shader->Activate();
    glBindVertexArray(m_standardVAO);

    // XXX: CAMERA START. 
    // Create model transform and send it to shader.
    glm::mat4 model(1);
    model = glm::rotate(model, glm::radians(-55.f), glm::vec3(1.0f, 0.0f, 0.0f));
    
    glm::mat4 view(1);
    view = glm::translate(view, glm::vec3{ 0.0f, 0.0f, -3.0f });

    glm::mat4 projection(1);
    projection = glm::perspective( 
        glm::radians(45.0f),
        static_cast<float>(m_viewportWidth) / static_cast<float>(m_viewportHeight),
        0.1f,
        100.0f);
     
    m_shader->SetMatrix4("model", model);
    m_shader->SetMatrix4("view", view);
    m_shader->SetMatrix4("projection", projection);

    // XXX: CAMERA END.

    // Update elapsed time and pass it to the shader.
    m_renderTime += deltaTime; 
    m_shader->SetFloat("renderTime", static_cast<float>(m_renderTime.totalSeconds()));

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
            "Content\\Shaders\\Standard_vs.glsl",
            "Content\\Shaders\\Standard_fs.glsl")); 

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
//---------------------------------------------------------------------------------------------------------------------
void SceneRenderer::SetViewportSize(unsigned int width, unsigned int height)
{
    assert(width > 0 && "Viewport width must be larger than zero");
    assert(height > 0 && "Viewport height must be larger than zero");

    m_viewportWidth = width;
    m_viewportHeight = height;

    glViewport(0, 0, static_cast<int>(m_viewportWidth), static_cast<int>(m_viewportHeight));
}
