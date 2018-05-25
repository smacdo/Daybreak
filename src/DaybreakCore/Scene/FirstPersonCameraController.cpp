#include "stdafx.h"
#include "FirstPersonCameraController.h"
#include "Common/Time.h"
#include "Input/IInputSource.h"
#include "Camera.h"

using namespace Daybreak;

//---------------------------------------------------------------------------------------------------------------------
FirstPersonCameraController::FirstPersonCameraController(
    std::shared_ptr<Camera> camera,
    std::shared_ptr<IInputSource> inputSource)
    : m_camera(camera),
      m_inputSource(inputSource)
{
}

//---------------------------------------------------------------------------------------------------------------------
void FirstPersonCameraController::update(const TimeSpan& deltaTime)
{
    // Bail out early if there is no input source or camera attached.
    if (m_inputSource == nullptr || m_camera == nullptr)
    {
        return;
    }

    // Walk logic.
    const auto deltaSeconds = static_cast<float>(deltaTime.totalSeconds());
    const auto walkDistance = m_walkSpeed * deltaSeconds;

    if (m_inputSource->IsKeyDown(InputKey::w))
    {
        m_camera->setPosition(m_camera->position() + m_camera->forward() * walkDistance);
    }
    else if (m_inputSource->IsKeyDown(InputKey::s))
    {
        m_camera->setPosition(m_camera->position() - m_camera->forward() * walkDistance);
    }
    else if (m_inputSource->IsKeyDown(InputKey::a))
    {
        m_camera->setPosition(m_camera->position() - m_camera->right() * walkDistance);
    }
    else if (m_inputSource->IsKeyDown(InputKey::d))
    {
        m_camera->setPosition(m_camera->position() + m_camera->right() * walkDistance);
    }

    // Mouse look.
    const float lookDistance = m_lookSpeed * deltaSeconds;
    const auto mouseMotion = m_inputSource->RelativeMouseMovemnet();

    m_camera->addYaw(mouseMotion.x * lookDistance);
    m_camera->addPitch(mouseMotion.y * lookDistance);

    // Clamp pitch (Y axis) to [-89, 89] to prevent weird issues at 90.
    if (m_camera->pitch() > m_maxPitch)
    {
        m_camera->setPitch(m_maxPitch);
    }
    else if (m_camera->pitch() < m_minPitch)
    {
        m_camera->setPitch(m_minPitch);
    }
}