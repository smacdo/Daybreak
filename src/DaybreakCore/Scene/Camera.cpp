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
    return m_forward;
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
    m_roll = fmod(degrees, 360.0f);
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
    m_pitch = degrees;// fmod(degrees, 360.0f);
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
    m_yaw = degrees;// fmod(degrees, 360.0f);
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
    glm::vec3 forward;

    forward.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    forward.y = sin(glm::radians(m_pitch));
    forward.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));

    m_forward = glm::normalize(forward);

    // Recalculate the right and up vector once the new front vector is calculated.
    // Note that these are normalized because their length gets close to zero the more a player looks up or down and
    // this results in slower movement.
    m_right = glm::normalize(glm::cross(m_forward, m_worldUp));
    m_up = glm::normalize(glm::cross(m_right, m_forward));

    m_view = createLookAt(
        m_position,
        m_position + m_forward,
        m_up);
}

//---------------------------------------------------------------------------------------------------------------------
glm::mat4 Camera::createLookAt(
    const glm::vec3& position,
    const glm::vec3& target,
    const glm::vec3& worldUp)
{
    // Calculate the camera direction vector.
    auto zAxis = glm::normalize(position - target);

    // Calculate right angle vector.
    auto xAxis = glm::normalize(glm::cross(glm::normalize(worldUp), zAxis));

    // Calculate camera up vector.
    auto yAxis = glm::cross(zAxis, xAxis);

    // Create lookAt matrix as a combination of a translation and rotation matrix.
    //  (NOTE: matrix is accessed as [col][row] due to column major layout).
    glm::mat4 translation(1);
    
    translation[3][0] = -position.x;        // Third column, first row.
    translation[3][1] = -position.y;        // Third column, second row.
    translation[3][2] = -position.z;        // Third column, third row.

    glm::mat4 rotation(1);

    rotation[0][0] = xAxis.x;               // First column, first row.
    rotation[1][0] = xAxis.y;               // ...           second row.
    rotation[2][0] = xAxis.z;               // ...           third row.
    rotation[0][1] = yAxis.x;               // Second column, first row.
    rotation[1][1] = yAxis.y;               // ...            second row.
    rotation[2][1] = yAxis.z;               // ...            third row.
    rotation[0][2] = zAxis.x;               // Third column, first row.
    rotation[1][2] = zAxis.y;               // ...           second row.
    rotation[2][2] = zAxis.z;               // ...           third row.

    // Order: translation then rotation although reversed in code because left - right ordering.
    return rotation * translation;
}
