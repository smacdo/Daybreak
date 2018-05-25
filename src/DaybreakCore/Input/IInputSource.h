#pragma once
#include "InputKeys.h"
#include <glm/vec2.hpp>

namespace Daybreak
{
    // Interface that can be queried for current input (keyboard, mouse, gamepad) state.
    class IInputSource
    {
    public:
        // Destructor.
        virtual ~IInputSource() { };

        // Get if cursor is locked to window in relative movement mode.
        virtual bool IsRelativeMouseModeEnabled() const = 0;

        // Lock the cursor to the window and keep the cursor hidden.
        virtual void SetRelativeMouseMode(bool isEnabled) = 0;

        // Get mouse position.
        virtual glm::ivec2 MousePosition() const = 0;

        // Get relative mouse movement.
        virtual glm::ivec2 RelativeMouseMovemnet() const = 0;

        // Is input key being pushed?
        virtual bool IsKeyDown(InputKey key, unsigned int controllerIndex = 0) const = 0;
    };
}
