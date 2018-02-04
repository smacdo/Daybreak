#pragma once

#include <cstdint>
#include <memory>

struct SDL_Window;
struct SDL_Renderer;
class Scene;
class SceneRenderer;

/** Main application class that runs event loop. */
class DaybreakApp
{
public:
    /** Main constructor. */
    DaybreakApp(size_t windowWidth, size_t windowHeight);

    /** Destructor. */
    ~DaybreakApp();

    /** Run main event loop and return when completed. */
    void Run();

private:
    /** Create hardware accelerated OpenGL rendering window and context. */
    void InitializeRendering();

    /** Handle pending events from OS. */
    void ProcessPendingEvents();

    /** Called when rendering window size is changed. */
    void OnRenderWindowResized(size_t newWidth, size_t newHeight);

    /** Loads graphical resources. This should be removed once scenes and scene loading is added. */
    void InitResources();

    /** Called to render a frame. */
    void RenderFrame();

private:
    size_t m_initialWindowWidth = 0;
    size_t m_initialWindowHeight = 0;

    size_t m_windowWidth = 0;
    size_t m_windowHeight = 0;

    bool m_quit = false;

    SDL_Window * m_pWindow = nullptr;
    SDL_Renderer * m_pSdlRenderer = nullptr;

    std::unique_ptr<Scene> m_scene;
    std::unique_ptr<SceneRenderer> m_sceneRenderer;
};

