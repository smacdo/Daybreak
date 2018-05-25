#pragma once
#include <glm/glm.hpp>      // TODO: Remove when we switch to internal math.

namespace Daybreak
{
    // Standard camera for rendering.
    class Camera
    {
    public:
        // Constructor.
        Camera(const glm::vec3& position);

        // Get camera front vector ("facing").
        glm::vec3 forward() const;

        // Get camera right vector.
        glm::vec3 right() const;

        // Get roll in degrees (rotation around Z axis).
        float roll() const;

        // Add some roll in degrees (rotation around Z axis).
        void addRoll(float degrees);

        // Set roll in degrees (rotation around Z axis).
        void setRoll(float degrees);

        // Get pitch in degrees (rotation around X axis).
        float pitch() const;

        // Add some pitch in degrees (rotation around X axis).
        void addPitch(float degrees);

        // Set pitch in degrees (rotation around X axis).
        void setPitch(float degrees);

        // Get camera position.
        glm::vec3 position() const;

        // Set camera position.
        void setPosition(const glm::vec3& position);

        // Get camera up vector.
        glm::vec3 up() const;

        // Get view matrix.
        glm::mat4 view() const;

        // Get camera world up vector.
        glm::vec3 worldUp() const;

        // Set camera world up vector.
        void setWorldUp(const glm::vec3& up);

        // Get pitch in degrees (rotation around Y axis).
        float yaw() const;

        // Add some yaw in degrees (rotation around Y axis).
        void addYaw(float degrees);

        // Set pitch in degrees (rotation around Y axis).
        void setYaw(float degrees);

        // Calculate view matrix from a custom implementation of the standard "lookAt"  function.
        static glm::mat4 createLookAt(
            const glm::vec3& position,
            const glm::vec3& target,
            const glm::vec3& worldUp);

    private:
        void regenerateCachedValuesIfDirty() const;
        void regenerateCachedValues() const;
        

    private:
        static const glm::vec3 DefaultWorldUp;
        static const float InitialYawDegrees;

        // World up vector (allows change from default for techniques).
        glm::vec3 m_worldUp;

        // Camera pitch in degrees (rotation around X axis).
        float m_pitch = 0.0f;

        // Camera yaw in degrees (rotation around Y axis).
        float m_yaw = 0.0f;

        // Camera roll in degrees (rotation around Z axis).
        float m_roll = 0.0f;

        // Camera center position in world space.
        glm::vec3 m_position = { 0.0f, 0.0f, 0.0f };

    private:
        // Cached derived values dirty flag. When set to true this flag indicates that cached values like the view
        // matrix are out of date and need to be regenerated.
        mutable bool m_dirty = false;

        // Camera front vector ("heading").
        mutable glm::vec3 m_front = { 0.0f, 0.0f, 0.0f };

        // Camera right vector.
        mutable glm::vec3 m_right = { 0.0f, 0.0f, 0.0f };

        // Camera up vector.
        mutable glm::vec3 m_up = { 0.0f, 0.0f, 0.0f };

        // Cached view matrix.
        mutable glm::mat4 m_view;
    };
}