#pragma once
#include "Renderer/Texture.h"
#include <glm/vec3.hpp>
#include <memory>

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
        void setDiffuseColor(const glm::vec3& color) { m_diffuseColor = color; }

        // Get diffuse texture material.
        std::shared_ptr<ITexture2d> diffuseTexture() const { return m_diffuseTexture; }

        // Set diffuse texture material.
        void setDiffuseTexture(std::shared_ptr<ITexture2d> texture) { m_diffuseTexture = texture; }

        // Check if diffuse texture was set.
        bool hasDiffuseTexture() const { return m_diffuseTexture != nullptr; }

        // Get specular material color.
        glm::vec3 specularColor() const { return m_specularColor; }

        // Set specular material color.
        void setSpecularColor(const glm::vec3& color) { m_specularColor = color; }

        // Get specular texture material.
        std::shared_ptr<ITexture2d> specularTexture() const { return m_specularTexture; }

        // Set specular texture material.
        void setSpecularTexture(std::shared_ptr<ITexture2d> texture) { m_specularTexture = texture; }

        // Check if specular texture was set.
        bool hasSpecularTexture() const { return m_specularTexture != nullptr; }

        // Get shininess.
        float shininess() const { return m_shininess; }

        // Set shininess.
        void setShininess(float shininess) { m_shininess = shininess; }

        // Get emissive texture material.
        std::shared_ptr<ITexture2d> emissiveTexture() const { return m_emissiveTexture; }

        // Set emissive texture material.
        void setEmissiveTexture(std::shared_ptr<ITexture2d> texture) { m_emissiveTexture = texture; }

        // Check if emissive texture was set.
        bool hasEmissiveTexture() const { return m_emissiveTexture != nullptr; }

    private:
        std::shared_ptr<ITexture2d> m_diffuseTexture;
        std::shared_ptr<ITexture2d> m_specularTexture;
        std::shared_ptr<ITexture2d> m_emissiveTexture;
        glm::vec3 m_ambientColor = { 0, 0, 0 };
        glm::vec3 m_diffuseColor = { 0, 0, 0 };
        glm::vec3 m_specularColor = { 0, 0, 0 };
        float m_shininess = 1;
    };
}