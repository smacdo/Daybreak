#pragma once
#include <string>
#include <memory>

class Scene;
class Shader;

/** Renders a scene. */
class SceneRenderer
{
public:
    /** Constructor. */
    SceneRenderer();

    /** Destructor. */
    ~SceneRenderer();

    /** Render the scene. */
    void Render(const Scene& scene);

    /** Get if wireframe rendering is enabled. */
    bool IsWireframeEnabled() const { return m_wireframe; }

    /** Set if wireframe rendering is enabled. */
    void SetWireframeEnabled(bool isEnabled) { m_wireframe = isEnabled; }

private:
    /** Creates a default scene to render (since there is no scene support yet). */
    void CreateDefaultScene(); // TODO: Remove this once we have scene loading.

    /** Checks that the shader was compiled successfully otherwise throws an exception. */
    static void VerifyShaderCompiled(unsigned int shader, const std::string& shaderName); // TODO: Move to wherever shaders are loaded.
    
    /** Checks that the shader was linked successfully otherwise throws an exception. */
    static void VerifyShaderLinked(unsigned int program, const std::string& programName); // TODO: Move to wherever shaders are loaded.

private:
    /** Flag that is set if wireframe rendering should be used. */
    bool m_wireframe = false;

    // TODO: This needs to be moved once we have a better rendering class layout since it should be created once
    //       at program start and shared to everyone who needs it.
    unsigned int m_standardVAO = 0;

    std::unique_ptr<Shader> m_standardShader;
};

