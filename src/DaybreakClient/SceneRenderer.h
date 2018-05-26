#pragma once
#include "Common/Time.h"
#include <string>
#include <memory>
#include <vector>
#include <glm/glm.hpp>

class Scene;
class Shader;
class Texture;

namespace Daybreak
{
    class Camera;
}

namespace Daybreak::OpenGlRenderer
{
    class OglInputLayout;
}

// Responsible for rendering a graphical scene.
class SceneRenderer
{
public:
    // Constructor.
    SceneRenderer(
        unsigned int viewportWidth,
        unsigned int viewportHeight);

    // Destructor.
    ~SceneRenderer();

    // Render the scene.
    void Render(const Scene& scene, const Daybreak::TimeSpan& deltaTime);

    // Resize the render window size (in pixels).
    void SetViewportSize(unsigned int width, unsigned int height);

    // Get if wireframe rendering is enabled.
    bool IsWireframeEnabled() const { return m_wireframe; }

    // Set if wireframe rendering is enabled.
    void SetWireframeEnabled(bool isEnabled) { m_wireframe = isEnabled; }

    // Get the camera.
    std::shared_ptr<Daybreak::Camera> mainCamera() { return m_camera; }

private:
    // Creates a default scene to render (since there is no scene support yet).
    void CreateDefaultScene(); // TODO: Remove this once we have scene loading.

private:
    // Viewport width.
    unsigned int m_viewportWidth = 0;
    
    // Viewport height.
    unsigned int m_viewportHeight = 0;

    // Flag that is set if wireframe rendering should be used.
    bool m_wireframe = false;

    // Amount of accumulated rendering time.
    Daybreak::TimeSpan m_renderTime;

    glm::vec3 m_lightPos = { 1.2f, 1.0f, 2.0f };

    std::shared_ptr<Daybreak::OpenGlRenderer::OglInputLayout> m_standardInputLayout;
    std::shared_ptr<Daybreak::OpenGlRenderer::OglInputLayout> m_lightInputLayout;
    std::unique_ptr<Shader> m_standardShader;
    std::unique_ptr<Shader> m_lightDebugShader;
    std::unique_ptr<Texture> m_texture;
    std::unique_ptr<Texture> m_texture2;
    std::shared_ptr<Daybreak::Camera> m_camera;
};

