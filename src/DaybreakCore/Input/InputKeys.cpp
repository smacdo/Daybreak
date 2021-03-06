#include "stdafx.h"
#include "InputKeys.h"

using namespace Daybreak;

//---------------------------------------------------------------------------------------------------------------------
InputKey Daybreak::Translate(SDL_Keycode keycode)
{
    switch (keycode)
    {
    case SDLK_0:
        return InputKey::_0;
    case SDLK_1:
        return InputKey::_1;
    case SDLK_2:
        return InputKey::_2;
    case SDLK_3:
        return InputKey::_3;
    case SDLK_4:
        return InputKey::_4;
    case SDLK_5:
        return InputKey::_5;
    case SDLK_6:
        return InputKey::_6;
    case SDLK_7:
        return InputKey::_7;
    case SDLK_8:
        return InputKey::_8;
    case SDLK_9:
        return InputKey::_9;
    case SDLK_a:
        return InputKey::a;
    case SDLK_b:
        return InputKey::b;
    case SDLK_c:
        return InputKey::c;
    case SDLK_d:
        return InputKey::d;
    case SDLK_e:
        return InputKey::e;
    case SDLK_f:
        return InputKey::f;
    case SDLK_g:
        return InputKey::g;
    case SDLK_h:
        return InputKey::h;
    case SDLK_i:
        return InputKey::i;
    case SDLK_j:
        return InputKey::j;
    case SDLK_k:
        return InputKey::k;
    case SDLK_l:
        return InputKey::l;
    case SDLK_m:
        return InputKey::m;
    case SDLK_n:
        return InputKey::n;
    case SDLK_o:
        return InputKey::o;
    case SDLK_p:
        return InputKey::p;
    case SDLK_q:
        return InputKey::q;
    case SDLK_r:
        return InputKey::r;
    case SDLK_s:
        return InputKey::s;
    case SDLK_t:
        return InputKey::t;
    case SDLK_u:
        return InputKey::u;
    case SDLK_v:
        return InputKey::v;
    case SDLK_w:
        return InputKey::w;
    case SDLK_x:
        return InputKey::x;
    case SDLK_y:
        return InputKey::y;
    case SDLK_z:
        return InputKey::z;
    case SDLK_F1:
        return InputKey::f1;
    case SDLK_F2:
        return InputKey::f2;
    case SDLK_F3:
        return InputKey::f3;
    case SDLK_F4:
        return InputKey::f4;
    case SDLK_F5:
        return InputKey::f5;
    case SDLK_F6:
        return InputKey::f6;
    case SDLK_F7:
        return InputKey::f7;
    case SDLK_F8:
        return InputKey::f8;
    case SDLK_F9:
        return InputKey::f9;
    case SDLK_F10:
        return InputKey::f10;
    case SDLK_F11:
        return InputKey::f11;
    case SDLK_F12:
        return InputKey::f12;
    case SDLK_KP_0:
        return InputKey::kp0;
    case SDLK_KP_1:
        return InputKey::kp1;
    case SDLK_KP_2:
        return InputKey::kp2;
    case SDLK_KP_3:
        return InputKey::kp3;
    case SDLK_KP_4:
        return InputKey::kp4;
    case SDLK_KP_5:
        return InputKey::kp5;
    case SDLK_KP_6:
        return InputKey::kp6;
    case SDLK_KP_7:
        return InputKey::kp7;
    case SDLK_KP_8:
        return InputKey::kp8;
    case SDLK_KP_9:
        return InputKey::kp9;
    case SDLK_UP:
        return InputKey::Up;
    case SDLK_DOWN:
        return InputKey::Down;
    case SDLK_LEFT:
        return InputKey::Left;
    case SDLK_RIGHT:
        return InputKey::Right;
    case SDLK_BACKSLASH:
        return InputKey::Backslash;
    case SDLK_BACKSPACE:
        return InputKey::Backspace;
    case SDLK_BACKQUOTE:
        return InputKey::Backquote;
    case SDLK_CAPSLOCK:
        return InputKey::Capslock;
    case SDLK_COMMA:
        return InputKey::Comma;
    case SDLK_ESCAPE:
        return InputKey::Escape;
    
    default:
        return InputKey::Unknown;
    }
}