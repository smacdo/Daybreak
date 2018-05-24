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
    if (m_inputSource == nullptr || m_camera == nullptr)
    {
        return;
    }

    const auto speed = 1.0f * static_cast<float>(deltaTime.totalSeconds());

    if (m_inputSource->IsKeyDown(InputKey::w))
    {
        m_camera->setPosition(m_camera->position() + m_camera->forward() * speed);
    }
    else if (m_inputSource->IsKeyDown(InputKey::s))
    {
        m_camera->setPosition(m_camera->position() - m_camera->forward() * speed);
    }
    else if (m_inputSource->IsKeyDown(InputKey::a))
    {
        m_camera->setPosition(m_camera->position() - m_camera->right() * speed);
    }
    else if (m_inputSource->IsKeyDown(InputKey::d))
    {
        m_camera->setPosition(m_camera->position() + m_camera->right() * speed);
    }
}
