#pragma once
#include <SDL.h>

namespace Daybreak
{
    // List of known input (keyboard, gamepad, mouse, etc) buttons.
    enum class InputKey
    {
        _0, _1, _2, _3, _4, _5, _6, _7, _8, _9,
        a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z,
        f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12,
        kp0, kp1, kp2, kp3, kp4, kp5, kp6, kp7, kp8, kp9,
        Up, Down, Left, Right,
        Backspace,
        Backslash,
        Backquote,
        Capslock,
        Comma,
        PrintScreen,
        Delete,
        End,
        Equals,
        Escape,
        Insert,
        Unknown,
        Count
    };

    // Translate from SDL keycode to Daybreak input key.
    InputKey Translate(SDL_Keycode keycode);
}