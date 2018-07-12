#include "stdafx.h"
#include "SdlException.h"

#include <string>
#include <exception>

#include <SDL.h>
//#include <glad\glad.h>

//---------------------------------------------------------------------------------------------------------------------
SdlException::SdlException(const std::string& message)
    : std::runtime_error(message + std::string(": ") + GetLastSdlError()),
    m_message(message)
{
}

//---------------------------------------------------------------------------------------------------------------------
std::string SdlException::GetLastSdlError()
{
    return SDL_GetError();
}
