#pragma once
#include <glm/vec3.hpp>

namespace Daybreak
{
    // Phong light.
    class PhongLight
    {
    public:
        // Default constructor.
        PhongLight() = default;

        // Simple constructor that sets ambient to fraction of diffuse, and specular to same as diffuse.
        PhongLight(const glm::vec3& color, float ambientAmount = 0)
            : m_ambientColor(color * ambientAmount),
              m_diffuseColor(color),
              m_specularColor(ambientAmount)
        {
        }

        // Value constructor.
        PhongLight(
            const glm::vec3& ambient,
            const glm::vec3& diffuse,
            const glm::vec3& specular)
            : m_ambientColor(ambient),
              m_diffuseColor(diffuse),
              m_specularColor(specular)
        {
        }

        // Set color by setting diffuse and specular to the same value, and ambient as a portion of diffuse.
        void setColor(const glm::vec3& color, float ambientAmount = 0)
        {
            m_ambientColor = color * ambientAmount;
            m_diffuseColor = color;
            m_specularColor = color;
        }

        // Get ambient light color.
        glm::vec3 ambientColor() const { return m_ambientColor; }

        // Set ambient light color.
        void setAmbientColor(const glm::vec3& color) { m_ambientColor = color; }

        // Get diffuse light color.
        glm::vec3 diffuseColor() const { return m_diffuseColor; }

        // Set diffuse light color.
        void setDiffuseColor(const glm::vec3& color) { m_diffuseColor = color; }

        // Get specular light color.
        glm::vec3 specularColor() const { return m_specularColor; }

        // Set specular light color.
        void setSpecularColor(const glm::vec3& color) { m_specularColor = color; }

    private:
        glm::vec3 m_ambientColor = { 0, 0, 0 };
        glm::vec3 m_diffuseColor = { 0, 0, 0 };
        glm::vec3 m_specularColor = { 0, 0, 0 };
    };

    // Directional phong light.
    class DirectionalPhongLight : public PhongLight
    {
    public:
        // Default constructor.
        DirectionalPhongLight() = default;

        // Simple constructor that sets ambient to fraction of diffuse, and specular to same as diffuse.
        DirectionalPhongLight(const glm::vec3& direction, const glm::vec3& color, float ambientAmount = 0)
            : PhongLight(color, ambientAmount),
              m_direction(direction)
        {
        }

        // Value constructor.
        DirectionalPhongLight(
            const glm::vec3& direction,
            const glm::vec3& ambient,
            const glm::vec3& diffuse,
            const glm::vec3& specular)
            : PhongLight(ambient, diffuse, specular),
              m_direction(direction)
        {
        }

        // Get direction light is shining.
        glm::vec3 direction() const { return m_direction; }
    
        // Set direction light is shining.
        void setDirection(const glm::vec3& direction) { m_direction = direction; }

    private:
        glm::vec3 m_direction = { 0, 0, -1 };
    };

    // Point phong light.
    class PointPhongLight : public PhongLight
    {
    public:
        // Default constructor.
        PointPhongLight() = default;

        // Simple constructor that sets ambient to fraction of diffuse, and specular to same as diffuse.
        PointPhongLight(
            const glm::vec3& position,
            float linear,
            float quadratic,
            const glm::vec3& color,
            float ambientAmount = 0)
            : PhongLight(color, ambientAmount),
              m_position(position),
              m_linear(linear),
              m_quadratic(quadratic)
        {
        }

        // Value constructor.
        PointPhongLight(
            const glm::vec3& position,
            float constant,
            float linear,
            float quadratic,
            const glm::vec3& ambient,
            const glm::vec3& diffuse,
            const glm::vec3& specular)
            : PhongLight(ambient, diffuse, specular),
              m_position(position),
              m_constant(constant),
              m_linear(linear),
              m_quadratic(quadratic)
        {
        }

        // Get point light position.
        glm::vec3 position() const { return m_position; }

        // Set point light is position.
        void setPosition(const glm::vec3& position) { m_position = position; }

        // Get constant falloff coefficient.
        float constant() const { return m_constant; }

        // Set constant falloff coefficient.
        void setConstant(float v) { m_constant = v; }
        
        // Get linear falloff coefficient.
        float linear() const { return m_linear; }

        // Set linear falloff coefficient.
        void setLinear(float v) { m_linear = v; }

        // Get quadratic falloff coefficient.
        float quadratic() const { return m_quadratic; }

        // Set quadratic falloff coefficient.
        void setQuadratic(float v) { m_quadratic = v; }

    private:
        glm::vec3 m_position = { 0, 0, 0 };
        float m_constant = 1;
        float m_linear = 0.09f;
        float m_quadratic = 0.032f;
    };

    // Spot phong light.
    class SpotPhongLight : public PhongLight
    {
    public:
        // Default constructor.
        SpotPhongLight() = default;

        // Simple constructor that sets ambient to fraction of diffuse, and specular to same as diffuse.
        SpotPhongLight(
            const glm::vec3& position,
            const glm::vec3& direction,
            const glm::vec3& color,
            float ambientAmount = 0)
            : PhongLight(color, ambientAmount),
              m_position(position),
              m_direction(direction)
        {
        }

        // Value constructor.
        SpotPhongLight(
            const glm::vec3& position,
            const glm::vec3& direction,
            const glm::vec3& ambient,
            const glm::vec3& diffuse,
            const glm::vec3& specular)
            : PhongLight(ambient, diffuse, specular),
              m_position(position),
              m_direction(direction)
        {
        }

        // Get spot light position.
        glm::vec3 position() const { return m_position; }

        // Set spot light position.
        void setPosition(const glm::vec3& position) { m_position = position; }

        // Get direction light is shining.
        glm::vec3 direction() const { return m_direction; }

        // Set direction light is shining.
        void setDirection(const glm::vec3& direction) { m_direction = direction; }

    private:
        glm::vec3 m_position = { 0, 0, 0 };
        glm::vec3 m_direction = { 0, 0, -1 };
    };

}