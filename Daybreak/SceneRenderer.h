#pragma once
#include <string>

class Scene;

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

private:
    /** Creates a default scene to render (since there is no scene support yet). */
    void CreateDefaultScene(); // TODO: Remove this once we have scene loading.

    /** Checks that the shader was compiled successfully otherwise throws an exception. */
    static void VerifyShaderCompiled(unsigned int shader, const std::string& shaderName); // TODO: Move to wherever shaders are loaded.
    
    /** Checks that the shader was linked successfully otherwise throws an exception. */
    static void VerifyShaderLinked(unsigned int program, const std::string& programName); // TODO: Move to wherever shaders are loaded.

private:
    // TODO: This needs to be moved once we have a better rendering class layout since it should be created once
    //       at program start and shared to everyone who needs it.
    unsigned int m_standardVAO = 0;

    // TODO: This needs to be moved once we have a better rendering class layout since it should be created once
    //       at program start and shared to everyone who needs it.
    unsigned int m_standardShaderProgram = 0;
};

