#pragma once
#include "Common/Time.h"
#include <string>
#include <memory>

class Scene;
class Shader;
class Texture;

// Responsible for rendering a graphical scene.
class SceneRenderer
{
public:
    // Constructor.
    SceneRenderer(unsigned int viewportWidth, unsigned int viewportHeight);

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

    // TODO: This needs to be moved once we have a better rendering class layout since it should be created once
    //       at program start and shared to everyone who needs it.
    unsigned int m_standardVAO = 0;

    std::unique_ptr<Shader> m_shader;
    std::unique_ptr<Texture> m_texture;
    std::unique_ptr<Texture> m_texture2;
};

