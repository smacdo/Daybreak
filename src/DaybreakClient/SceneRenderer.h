#pragma once
#include "Common/Time.h"
#include <string>
#include <memory>
#include <vector>
#include <glm/glm.hpp>

namespace Daybreak
{
    class Camera;
    class Scene;
    class Mesh;
    class ITexture2d;
    class IRenderContext;
}

namespace Daybreak::OpenGlRenderer
{
    class OglInputLayout;
    class OglVertexBuffer;
    class OglIndexBuffer;
    class OglTexture2d;
    class OglShader;
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
    void Render(const Daybreak::Scene& scene, const Daybreak::TimeSpan& deltaTime);

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

    std::shared_ptr<Daybreak::IRenderContext> m_renderContext;

    glm::vec3 m_lightPos = { 1.2f, 0.0f, 2.0f };        // TODO: Move to scene.
    glm::vec3 m_lightColor = { 0.8f, 0.8f, 1.0f };      // TODO: Move to scene.

    std::shared_ptr<Daybreak::OpenGlRenderer::OglInputLayout> m_standardInputLayout;
    std::shared_ptr<Daybreak::OpenGlRenderer::OglInputLayout> m_lightInputLayout;
    std::shared_ptr<Daybreak::OpenGlRenderer::OglShader> m_standardShader;
    std::shared_ptr<Daybreak::OpenGlRenderer::OglShader> m_lightDebugShader;
    std::shared_ptr<Daybreak::Camera> m_camera;

    std::shared_ptr<Daybreak::Mesh> m_mesh;
};

