#pragma once
#include <glm/glm.hpp>      // TODO: Remove when we switch to internal math.
#include <memory>

namespace Daybreak
{
    class Camera;
    class TimeSpan;
    class IInputSource;

    // A simple first person camera that is controlled from a keyboard and mouse.
    class FirstPersonCameraController
    {
    public:
        // Constructor.
        FirstPersonCameraController(
            std::shared_ptr<Camera> camera,
            std::shared_ptr<IInputSource> inputSource);

        // Get the camera.
        std::shared_ptr<Camera> camera() const { return m_camera; }

        // Set the camera.
        void setCamera(std::shared_ptr<Camera> camera) { m_camera = camera; }

        // Get the input source.
        std::shared_ptr<IInputSource> inputSource() const { return m_inputSource; }

        // Set the input source.
        void setInputSource(std::shared_ptr<IInputSource> inputSource) { m_inputSource = inputSource; }

        // Tick camera once per update.
        void update(const Daybreak::TimeSpan& deltaTime);

    private:
        std::shared_ptr<Camera> m_camera;
        std::shared_ptr<IInputSource> m_inputSource;
    };
}