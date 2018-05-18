#pragma once
#include "Common/Time.h"
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
    DaybreakApp(unsigned int windowWidth, unsigned int windowHeight);

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
    void OnRenderWindowResized(unsigned int newWidth, unsigned int newHeight);

    /** Called to render a frame. */
    void RenderFrame(const Daybreak::TimeSpan& deltaTime);

private:
    unsigned int m_initialWindowWidth = 0;
    unsigned int m_initialWindowHeight = 0;

    unsigned int m_windowWidth = 0;
    unsigned int m_windowHeight = 0;

    bool m_quit = false;

    SDL_Window * m_pWindow = nullptr;
    SDL_Renderer * m_pSdlRenderer = nullptr;

    std::unique_ptr<Scene> m_scene;
    std::unique_ptr<SceneRenderer> m_sceneRenderer;
};

