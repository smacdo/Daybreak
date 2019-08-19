#pragma once
#include "app/time.h"
#include <cstdint>
#include <memory>

struct SDL_Window;
struct SDL_Renderer;
class SceneRenderer;

namespace Daybreak
{
    class Scene;
    class SdlInputSource;
    class FirstPersonCameraController;
}

/** Main application class that runs event loop. */
class DaybreakApp
{
public:
    /** Main constructor. */
    DaybreakApp(unsigned int windowWidth, unsigned int windowHeight);

    /** Destructor. */
    ~DaybreakApp();

public:
    // Check if OpenGL debug context is enabled.
    bool hasGlDebugContext() const { return m_hasGlDebugContext; }

    // Check if OpenGL debug context was requested.
    bool wasGlDebugContextRequested() const { return m_glDebugContext; }

    // Set if OpenGL context should have debug enabled.
    void setGlDebugContextRequested(bool isRequested) { m_glDebugContext = isRequested; }

public:
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
    void RenderFrame(Daybreak::timespan_t deltaTime);

private:
    unsigned int m_initialWindowWidth = 0;
    unsigned int m_initialWindowHeight = 0;

    unsigned int m_windowWidth = 0;
    unsigned int m_windowHeight = 0;

    bool m_quit = false;
    bool m_glDebugContext = false;
    bool m_hasGlDebugContext = false;

    SDL_Window * m_pWindow = nullptr;
    SDL_Renderer * m_pSdlRenderer = nullptr;

    std::unique_ptr<Daybreak::Scene> m_scene;
    std::unique_ptr<SceneRenderer> m_sceneRenderer;
    std::shared_ptr<Daybreak::SdlInputSource> m_inputSource;
    std::unique_ptr<Daybreak::FirstPersonCameraController> m_cameraController;
};

