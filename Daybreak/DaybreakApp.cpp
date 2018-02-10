#include "stdafx.h"
#include "DaybreakApp.h"
#include "Daybreak.h"
#include "Scene.h"
#include "SceneRenderer.h"
#include "SystemInfo.h"

#include "glad/glad.h"

#include <cassert>
#include <SDL.h>

//---------------------------------------------------------------------------------------------------------------------
DaybreakApp::DaybreakApp(size_t windowWidth, size_t windowHeight)
    : m_initialWindowWidth(windowWidth),
      m_initialWindowHeight(windowHeight),
      m_scene(new Scene)
{
    assert(windowWidth > 0 && "Window width must be larger than zero");
    assert(windowHeight > 0 && "Window height must be larger than zero");
}

//---------------------------------------------------------------------------------------------------------------------
DaybreakApp::~DaybreakApp()
{
}

#include <iostream>

//---------------------------------------------------------------------------------------------------------------------
void DaybreakApp::Run()
{
    // Initialize SDL modules.
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        throw SdlException("Failed to initialize SDL subsystems");;
    }

    // Now initialize rendering.
    InitializeRendering();

    // Create the default scene renderer.
    m_sceneRenderer.reset(new SceneRenderer); 

    // TODO: Load scene.

    // Main game loop.
    auto frequency = SDL_GetPerformanceFrequency();
    auto t0 = SDL_GetPerformanceCounter();

    while (!m_quit)
    {
        // Get time elapsed since last frame.
        auto t1 = SDL_GetPerformanceCounter();
        float deltaSeconds = (t1 - t0) / static_cast<float>(frequency);

        t0 = t1;

        // TODO: Update simulation.

        // Process platform windowinng events.
        ProcessPendingEvents();

        // Draw current frame.
        RenderFrame(deltaSeconds);
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

    assert(m_pWindow != nullptr && "Window must be created");
    assert(m_pSdlRenderer != nullptr && "Renderer must be created");

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

    // Make sure window is resized to initial size.
    OnRenderWindowResized(m_initialWindowWidth, m_initialWindowHeight);

    // Enable v-sync.
    SDL_GL_SetSwapInterval(1);
}

//---------------------------------------------------------------------------------------------------------------------
void DaybreakApp::RenderFrame(float deltaSeconds)
{
    glClearColor(0.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    m_sceneRenderer->Render(*m_scene.get(), deltaSeconds);

    SDL_GL_SwapWindow(m_pWindow);
}

//---------------------------------------------------------------------------------------------------------------------
void DaybreakApp::ProcessPendingEvents()
{
    SDL_Event event;

    while (SDL_PollEvent(&event))
    {
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

        case SDL_KEYDOWN:
            switch (event.key.keysym.sym)
            {
            case SDLK_ESCAPE:
                m_quit = true;
                break;

            case SDLK_F6:
                m_sceneRenderer->SetWireframeEnabled(!m_sceneRenderer->IsWireframeEnabled());
                break;
            }
            break;

        case SDL_QUIT:
            m_quit = true;
            break;
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DaybreakApp::OnRenderWindowResized(size_t newWidth, size_t newHeight)
{
    assert(newWidth > 0 && "Render window width must be larger than zero");
    assert(newHeight > 0 && "Render window height must be larger than zero");

    m_windowWidth = newWidth;
    m_windowHeight = newHeight;

    glViewport(0, 0, static_cast<int>(m_windowWidth), static_cast<int>(m_windowHeight));
}