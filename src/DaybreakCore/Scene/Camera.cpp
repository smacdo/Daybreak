#include "stdafx.h"
#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>

using namespace Daybreak;

// Default world up direction for new cameras.
const glm::vec3 Camera::DefaultWorldUp = { 0.0f, 1.0f, 0.0f };

// Initial yaw for new cameras. This rotates the camera on the Y axis to face the -Z axis. This makes adhering to
// OpenGL's coordinate system simpler.
const float Camera::InitialYawDegrees = -90.0f;

//---------------------------------------------------------------------------------------------------------------------
Camera::Camera(const glm::vec3& position)
    : m_worldUp(DefaultWorldUp),
      m_yaw(InitialYawDegrees),
      m_position(position)
{
    regenerateCachedValues();
}

//---------------------------------------------------------------------------------------------------------------------
glm::vec3 Camera::forward() const
{
    regenerateCachedValuesIfDirty();
    return m_front;
}

//---------------------------------------------------------------------------------------------------------------------
float Camera::roll() const
{
    return m_roll;
}

//---------------------------------------------------------------------------------------------------------------------
void Camera::addRoll(float degrees)
{
    setRoll(m_roll + degrees);
}

//---------------------------------------------------------------------------------------------------------------------
void Camera::setRoll(float degrees)
{
    m_roll = degrees;
    m_dirty = true;
}

//---------------------------------------------------------------------------------------------------------------------
glm::vec3 Camera::right() const
{
    regenerateCachedValuesIfDirty();
    return m_right;
}

//---------------------------------------------------------------------------------------------------------------------
float Camera::pitch() const
{
    return m_pitch;
}

//---------------------------------------------------------------------------------------------------------------------
void Camera::addPitch(float degrees)
{
    setPitch(m_pitch + degrees);
}

//---------------------------------------------------------------------------------------------------------------------
void Camera::setPitch(float degrees)
{
    m_pitch = degrees;
    m_dirty = true;
}

//---------------------------------------------------------------------------------------------------------------------
glm::vec3 Camera::position() const
{
    return m_position;
}

//---------------------------------------------------------------------------------------------------------------------
void Camera::setPosition(const glm::vec3& position)
{
    m_position = position;
    m_dirty = true;
}

//---------------------------------------------------------------------------------------------------------------------
glm::vec3 Camera::up() const
{
    regenerateCachedValuesIfDirty();
    return m_up;
}

//---------------------------------------------------------------------------------------------------------------------
glm::mat4 Camera::view() const
{
    regenerateCachedValuesIfDirty();
    return m_view;
}

//---------------------------------------------------------------------------------------------------------------------
glm::vec3 Camera::worldUp() const
{
    return m_worldUp;
}

//---------------------------------------------------------------------------------------------------------------------
void Camera::setWorldUp(const glm::vec3& up)
{
    m_worldUp = up;
    m_dirty = true;
}

//---------------------------------------------------------------------------------------------------------------------
float Camera::yaw() const
{
    return m_yaw;
}

//---------------------------------------------------------------------------------------------------------------------
void Camera::addYaw(float degrees)
{
    setYaw(m_yaw + degrees);
}

//---------------------------------------------------------------------------------------------------------------------
void Camera::setYaw(float degrees)
{
    m_yaw = degrees;
    m_dirty = true;
}

//---------------------------------------------------------------------------------------------------------------------
void Camera::regenerateCachedValuesIfDirty() const
{
    if (m_dirty)
    {
        regenerateCachedValues();
        m_dirty = false;
    }
}

//---------------------------------------------------------------------------------------------------------------------
void Camera::regenerateCachedValues() const // const because it only regenerates mutable cached fields.
{
    // Caculate front (heading) vector from the camera yaw, pitch, roll values.
    m_front.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    m_front.y = sin(glm::radians(m_pitch));
    m_front.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    m_front = glm::normalize(m_front);

    // Recalculate the right and up vector once the new front vector is calculated.
    // Note that these are normalized because their length gets close to zero the more a player looks up or down and
    // this results in slower movement.
    m_right = glm::normalize(glm::cross(m_front, m_worldUp));
    m_up = glm::normalize(glm::cross(m_right, m_front));

    m_view = glm::lookAt(
        m_position,
        m_position + m_front,
        m_up);
}
