#pragma once
#include "IInputSource.h"
#include "InputKeys.h"

#include <vector>
#include <SDL.h>
#include <glm/vec2.hpp>

namespace Daybreak
{
    // Operating system event driven input manager using SDL library.
    class SdlInputSource : public IInputSource
    {
    public:
        // Constructor.
        SdlInputSource();

        // Destructor.
        virtual ~SdlInputSource();

        // Get if cursor is locked to window in relative movement mode.
        virtual bool IsRelativeMouseModeEnabled() const override;

        // Lock the cursor to the window and keep the cursor hidden.
        virtual void SetRelativeMouseMode(bool isEnabled) override;

        // Get mouse position.
        virtual glm::ivec2 MousePosition() const override;

        // Get relative mouse movement.
        virtual glm::ivec2 RelativeMouseMovemnet() const override;

        // Update input manager with an SDL event. Returns true if the message was processed, or false if it was
        // ignored and another system should deal with it.
        bool TryProcessMessage(const SDL_Event& event);

        // Called during each update immediately after all input events have been processed.
        void FinishProcessingMessages();

        // Is input key being pushed?
        virtual bool IsKeyDown(InputKey key, unsigned int controllerIndex = 0) const override;

    private:
        enum class PushState
        {
            None,           // Not pushed
            Started,        // Started being push this update.
            Pushed,         // Has been pushed for more than a single update.
            Stopped,        // Stopped being pushed this update.
        };

        struct input_key_state_t
        {
            PushState pushState = PushState::None;
            unsigned int controllerId = 0;
        };

    private:
        void OnKeyDownEvent(const SDL_Event& event);
        void OnKeyUpEvent(const SDL_Event& event);
        void OnMouseMotion(const SDL_Event& event);

    private:
        // Current key state built from currentKeys and previousKeys.
        std::vector<input_key_state_t> m_keys;

        // New state updates during this update tick.
        std::vector<input_key_state_t> m_nextKeys;

        glm::ivec2 m_mousePos = { 0, 0 };
        glm::ivec2 m_nextMousePos = { 0, 0 };
        glm::ivec2 m_relativeMouseMotion = { 0, 0 };
        glm::ivec2 m_nextRelativeMouseMotion = { 0, 0 };
    };
}