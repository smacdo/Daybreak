#pragma once
#include "app/timespan.h"

#include <glm/glm.hpp>      // TODO: Remove when we switch to internal math.
#include <memory>

namespace Daybreak
{
    class Camera;
    class IInputSource;

    // A simple first person camera that is controlled from a keyboard and mouse.
    class FirstPersonCameraController
    {
    public:
        // Constructor.
        FirstPersonCameraController(
            std::shared_ptr<Camera> camera,
            std::shared_ptr<IInputSource> inputSource);

    public:
        // Get the camera.
        std::shared_ptr<Camera> camera() const { return m_camera; }

        // Set the camera.
        void setCamera(std::shared_ptr<Camera> camera) { m_camera = camera; }

        // Get the input source.
        std::shared_ptr<IInputSource> inputSource() const { return m_inputSource; }

        // Set the input source.
        void setInputSource(std::shared_ptr<IInputSource> inputSource) { m_inputSource = inputSource; }

        // Get look speed.
        float lookSpeed() const { return m_lookSpeed; }

        // Set look spped.
        void setLookSpeed(float speed) { m_lookSpeed = speed; }

        // Get max camera pitch in degrees.
        float maxPitchDegrees() const { return m_maxPitch; }

        // Set max camera pitch in degrees.
        void setMaxPitchDegrees(float degrees) { m_maxPitch = degrees; }

        // Get min camera pitch in degrees.
        float minPitchDegrees() const { return m_minPitch; }

        // Set min camera pitch in degrees.
        void setMinPitchDegrees(float degrees) { m_minPitch = degrees; }

        // Get walk speed.
        float walkSpeed() const { return m_walkSpeed; }

        // Set walk speed.
        void setWalkSpeed(float speed) { m_walkSpeed = speed; }

    public:
        // Tick camera once per update.
        void update(Daybreak::timespan_t deltaTime);

    private:
        std::shared_ptr<Camera> m_camera;
        std::shared_ptr<IInputSource> m_inputSource;
        float m_walkSpeed = 2.0f;
        float m_lookSpeed = 10.0f;
        float m_minPitch = -89.0f;
        float m_maxPitch = 89.0f;
    };
}