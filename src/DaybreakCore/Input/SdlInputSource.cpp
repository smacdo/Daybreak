#include "stdafx.h"
#include "SdlInputSource.h"

#include <algorithm>

using namespace Daybreak;

//---------------------------------------------------------------------------------------------------------------------
SdlInputSource::SdlInputSource()
    : m_keys(static_cast<size_t>(InputKey::Count)),
      m_nextKeys(static_cast<size_t>(InputKey::Count))
{
}

//---------------------------------------------------------------------------------------------------------------------
SdlInputSource::~SdlInputSource() = default;

//---------------------------------------------------------------------------------------------------------------------
bool SdlInputSource::IsRelativeMouseModeEnabled() const
{
    return SDL_GetRelativeMouseMode() == SDL_TRUE;
}

//---------------------------------------------------------------------------------------------------------------------
void SdlInputSource::SetRelativeMouseMode(bool isEnabled)
{
    SDL_SetRelativeMouseMode(isEnabled ? SDL_TRUE : SDL_FALSE);
}

//---------------------------------------------------------------------------------------------------------------------
glm::ivec2 SdlInputSource::MousePosition() const
{
    return m_mousePos;
}

//---------------------------------------------------------------------------------------------------------------------
glm::ivec2 SdlInputSource::RelativeMouseMovemnet() const
{
    return m_relativeMouseMotion;
}

//---------------------------------------------------------------------------------------------------------------------
bool SdlInputSource::TryProcessMessage(const SDL_Event& event)
{
    switch (event.type)
    {
    case SDL_KEYDOWN:
        OnKeyDownEvent(event);
        return true;

    case SDL_KEYUP:
        OnKeyUpEvent(event);
        return true;

    case SDL_MOUSEMOTION:
        OnMouseMotion(event);
        return true;

    default:
        return false;
    }
}

//---------------------------------------------------------------------------------------------------------------------
void SdlInputSource::OnKeyDownEvent(const SDL_Event& event)
{
    auto key = Translate(event.key.keysym.sym);
    m_nextKeys[static_cast<size_t>(key)].pushState = PushState::Started;
}

//---------------------------------------------------------------------------------------------------------------------
void SdlInputSource::OnKeyUpEvent(const SDL_Event& event)
{
    auto key = Translate(event.key.keysym.sym);
    m_nextKeys[static_cast<size_t>(key)].pushState = PushState::Stopped;
}

//---------------------------------------------------------------------------------------------------------------------
void SdlInputSource::OnMouseMotion(const SDL_Event& event)
{
    m_nextMousePos = { event.motion.x, event.motion.y };
    m_nextRelativeMouseMotion = { event.motion.xrel, event.motion.yrel };
}

//---------------------------------------------------------------------------------------------------------------------
void SdlInputSource::FinishProcessingMessages()
{
    // Update mouse state.
    m_mousePos = m_nextMousePos;
    m_relativeMouseMotion = m_nextRelativeMouseMotion;

    m_nextRelativeMouseMotion = { 0, 0 };

    // Update input button state.
    for (size_t i = 0; i < m_keys.size(); ++i)
    {
        const auto nextPushState = m_nextKeys[i].pushState;
        auto& currentPushState = m_keys[i].pushState;

        // If the next key state is either a push starting or ending, update current key state accordingly.
        if (nextPushState == PushState::Started)
        {
            // New state is pushed. If the current key is not already pushed then start it being pushed.
            currentPushState = (currentPushState == PushState::Pushed ? PushState::Pushed : PushState::Started);
        }
        else if (nextPushState == PushState::Stopped)
        {
            // New state is released. Make the key always be released at this point.
            currentPushState = PushState::Stopped;
        }
        else
        {
            // There is no new state for this key. Keep the key in its current is/isn't pushed state but make sure to
            // convert started -> pushed or stopped -> none since we only get updates when state changes.
            if (currentPushState == PushState::Started)
            {
                currentPushState = PushState::Pushed;
            }
            else if (currentPushState == PushState::Stopped)
            {
                currentPushState = PushState::None;
            }
        }
    }

    // Reset next key state to zero for the next update.
    std::fill(m_nextKeys.begin(), m_nextKeys.end(), input_key_state_t {});
}

//---------------------------------------------------------------------------------------------------------------------
bool SdlInputSource::IsKeyDown(Daybreak::InputKey key, unsigned int controllerIndex) const
{
    return
        m_keys[static_cast<int>(key)].pushState == PushState::Started ||
        m_keys[static_cast<int>(key)].pushState == PushState::Pushed;
}