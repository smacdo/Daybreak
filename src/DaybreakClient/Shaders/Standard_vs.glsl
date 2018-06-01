#version 330 core
layout (location = 0) in vec3 posIn;
layout (location = 1) in vec2 texCoordIn;
layout (location = 2) in vec3 normalIn;

out vec2 ps_uv;
out vec3 ps_normal;
out vec3 ps_fragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat3 normalMatrix;

uniform float renderTime;

void main()
{
    // position = model * view * projection (but right to left order).
    gl_Position = projection * view * model * vec4(posIn, 1.0);

    // Texture coordinates are passed unmodified.
	ps_uv = texCoordIn;

    // Lighting is done in world space coordinate system.
    ps_normal = normalMatrix * normalIn;
    ps_fragPos = vec3(model * vec4(posIn, 1.0));
}
