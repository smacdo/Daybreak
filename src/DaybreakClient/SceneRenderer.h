#pragma once
#include "app/timespan.h"
#include <string>
#include <memory>
#include <vector>
#include <glm/glm.hpp>

namespace Daybreak
{
    class Camera;
    class Scene;
    class Mesh;
    class InputLayout;
    class IShader;
    class IDeviceContext;
    class IRenderContext;
    class PhongLightingEffect;
    class ResourcesManager;

    /** Renders a graphical scene. */
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
        void Render(const Daybreak::Scene& scene, Daybreak::timespan_t deltaTime);

        // Resize the render window size (in pixels).
        void SetViewportSize(unsigned int width, unsigned int height);

        // Get if wireframe rendering is enabled.
        bool IsWireframeEnabled() const { return m_wireframe; }

        // Set if wireframe rendering is enabled.
        void SetWireframeEnabled(bool isEnabled) { m_wireframe = isEnabled; }

        // Get the camera.
        std::shared_ptr<Daybreak::Camera> mainCamera() { return m_camera; }

        // Creates a default scene to render (since there is no scene support yet).
        void CreateDefaultScene(); // TODO: Remove this once we have scene loading.

    private:
        // Flag that is set if wireframe rendering should be used.
        bool m_wireframe = false;

        // Amount of accumulated rendering time.
        Daybreak::timespan_t m_renderTime;

        std::shared_ptr<Daybreak::IDeviceContext> m_deviceContext;
        std::shared_ptr<Daybreak::IRenderContext> m_renderContext;
        std::shared_ptr<Daybreak::ResourcesManager> m_resources;

        std::shared_ptr<Daybreak::InputLayout> m_standardInputLayout; // TODO: Not here. Not sure where yet.
        std::shared_ptr<Daybreak::IShader> m_lightDebugShader;        // TODO: Move to effect class.
        std::shared_ptr<Daybreak::Camera> m_camera;

        std::shared_ptr<Daybreak::Mesh> m_mesh;                 // TODO: Move to scene.
        std::unique_ptr<Daybreak::PhongLightingEffect> m_phong;

        // TODO: Move to caller of SceneRenderr.
        std::unique_ptr<Daybreak::Scene> m_scene;
    };
}
