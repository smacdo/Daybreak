#pragma once
#include "InputKeys.h"

namespace Daybreak
{
    // Interface that can be queried for current input (keyboard, mouse, gamepad) state.
    class IInputSource
    {
    public:
        // Destructor.
        virtual ~IInputSource() { };

        // Is input key being pushed?
        virtual bool IsKeyDown(InputKey key, unsigned int controllerIndex = 0) const = 0;
    };
}
