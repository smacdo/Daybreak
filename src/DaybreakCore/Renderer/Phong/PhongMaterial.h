#pragma once
#include <glm/vec3.hpp>

namespace Daybreak
{
    // Material with phong lighting properties.
    class PhongMaterial
    {
    public:
        // Constructor.
        PhongMaterial() = default;

        // Get ambient material color.
        glm::vec3 ambientColor() const { return m_ambientColor; }

        // Set ambient material color.
        void setAmbientColor(const glm::vec3& color) { m_ambientColor = color; }

        // Get diffuse material color.
        glm::vec3 diffuseColor() const { return m_diffuseColor; }

        // Set diffuse material color.
        void setDiffuseColor(const glm::vec3& color) { m_diffuseColor = color; ]

        // Get specular material color.
        glm::vec3 specularColor() const { return m_specularColor; }

        // Set specular material color.
        void setSpecularColor(const glm::vec3& color) { m_specularColor = color; }

        // Get shininess.
        float shininess() const { return m_shininess; }

        // Set shininess.
        void setShininess(float shininess) { m_shininess = shininess; }

    private:
        glm::vec3 m_ambientColor = { 0, 0, 0 };
        glm::vec3 m_diffuseColor = { 1, 1, 1 };
        glm::vec3 m_specularColor = { 0, 0, 0 };
        float m_shininess = 1;
        // TODO: texture references for diffuse, specular, emissive.
    };
}