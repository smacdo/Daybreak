#include "stdafx.h"
#include "DaybreakApp.h"
#include "Daybreak.h"
#include "Scene/Scene.h"
#include "SceneRenderer.h"
#include "SystemInfo.h"

#include "OglRenderer/OglError.h"
#include "Input/SdlInputSource.h"
#include "Scene/FirstPersonCameraController.h"

#include "glad/glad.h"

#include <cassert>
#include <SDL.h>

using namespace Daybreak;
using namespace Daybreak::OpenGlRenderer;

//---------------------------------------------------------------------------------------------------------------------
DaybreakApp::DaybreakApp(unsigned int windowWidth, unsigned int windowHeight)
    : m_initialWindowWidth(windowWidth),
      m_initialWindowHeight(windowHeight),
      m_scene(new Scene)
{
    assert(windowWidth > 0 && "Window width must be larger than zero");
    assert(windowHeight > 0 && "Window height must be larger than zero");
}

//---------------------------------------------------------------------------------------------------------------------
DaybreakApp::~DaybreakApp() = default;

//---------------------------------------------------------------------------------------------------------------------
void DaybreakApp::Run()
{
    // Initialize SDL modules.
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        throw SdlException("Failed to initialize SDL subsystems");;
    }

    // Initialize SDL input controller.
    m_inputSource = std::make_shared<SdlInputSource>();
    m_inputSource->SetRelativeMouseMode(true);

    // Now initialize rendering.
    InitializeRendering();

    // Create the default scene renderer.
    // TODO: Load scene.
    m_sceneRenderer.reset(new SceneRenderer(m_windowWidth, m_windowHeight)); 

    // Configure camera controller.
    m_cameraController = std::make_unique<FirstPersonCameraController>(
        m_sceneRenderer->mainCamera(),
        m_inputSource);

    // Main game loop.
    auto frequency = SDL_GetPerformanceFrequency();
    auto t0 = SDL_GetPerformanceCounter();

    while (!m_quit)
    {
        // Get time elapsed since last frame.
        auto t1 = SDL_GetPerformanceCounter();
        auto deltaSeconds = (t1 - t0) / static_cast<double>(frequency);

        t0 = t1;

        // Input stuff...
        if (m_inputSource->IsKeyDown(InputKey::Escape))
        {
            m_quit = true;
        }
        
        if (m_inputSource->IsKeyDown(InputKey::f6))
        {
            m_sceneRenderer->SetWireframeEnabled(!m_sceneRenderer->IsWireframeEnabled());
        }

        // TODO: Update simulation.
        m_cameraController->update(TimeSpan::FromSeconds(deltaSeconds));

        // Process platform windowinng events.
        ProcessPendingEvents();

        // Draw current frame.
        RenderFrame(TimeSpan::FromSeconds(deltaSeconds));
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DaybreakApp::InitializeRendering()
{
    // Configure OpenGL before creating a SDL OpenGL window.
    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

#ifdef __APPLE__
    // TODO: Untested!
    glfwWindowHint(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
#endif

    // TODO: SDL_GET_SetAttribute(SDL_GL_FRAMEBUFFER_SRGB_CAPABLE).

#ifdef _DEBUG
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG | SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
#else
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
#endif

    // Create an OpenGL accelerated rendering window.
    SDL_CreateWindowAndRenderer(
        static_cast<int>(m_initialWindowWidth),
        static_cast<int>(m_initialWindowHeight),
        SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE,   // TODO: Support high dpi mode.
        &m_pWindow,
        &m_pSdlRenderer);

    CHECK_NOT_NULL(m_pWindow);
    CHECK_NOT_NULL(m_pSdlRenderer);

    // Record render window size.
    m_windowWidth = m_initialWindowWidth;
    m_windowHeight = m_initialWindowHeight;

    // Make sure the window was created as expected.
    SDL_RendererInfo rendererInfo = {};
    SDL_GetRendererInfo(m_pSdlRenderer, &rendererInfo);

    if ((rendererInfo.flags & SDL_RENDERER_ACCELERATED) == 0 || (rendererInfo.flags & SDL_RENDERER_TARGETTEXTURE) == 0)
    {
        throw SdlException("Unable to create hardware accelerated rendering window");
    }

    // Create an OpenGL context after creating the window.
    SDL_GLContext context = SDL_GL_CreateContext(m_pWindow);

    /*printf("Vendor:   %s\n", glGetString(GL_VENDOR));
    printf("Renderer: %s\n", glGetString(GL_RENDERER));
    printf("Version:  %s\n", glGetString(GL_VERSION));*/

    // Initialize GLAD (OpenGL extension loader).
    if (!gladLoadGLLoader(SDL_GL_GetProcAddress))
    {
        throw GladException("Unable to initialize GLAD");
    }

    // Enable v-sync.
    SDL_GL_SetSwapInterval(1);
}

//---------------------------------------------------------------------------------------------------------------------
void DaybreakApp::RenderFrame(const TimeSpan& deltaTime)
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    m_sceneRenderer->Render(*m_scene.get(), deltaTime);

    SDL_GL_SwapWindow(m_pWindow);
}

//---------------------------------------------------------------------------------------------------------------------
void DaybreakApp::ProcessPendingEvents()
{
    SDL_Event event;

    while (SDL_PollEvent(&event))
    {
        // Send event to input manager for potential processing.
        if (m_inputSource->TryProcessMessage(event))
        {
            continue;
        }

        // Handle other events.
        switch (event.type)
        {
        case SDL_WINDOWEVENT:
            if (event.window.event == SDL_WINDOWEVENT_RESIZED)
            {
                auto width = event.window.data1;
                auto height = event.window.data2;
                OnRenderWindowResized(width, height);
            }
            break;

        case SDL_QUIT:
            m_quit = true;
            break;
        }
    }

    // Allow input manager to finish event processing before continuing with update.
    m_inputSource->FinishProcessingMessages();
}

//---------------------------------------------------------------------------------------------------------------------
void DaybreakApp::OnRenderWindowResized(unsigned int newWidth, unsigned int newHeight)
{
    m_sceneRenderer->SetViewportSize(newWidth, newHeight);
}