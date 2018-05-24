#version 330 core
layout (location = 0) in vec3 posIn;
layout (location = 1) in vec2 texCoordIn;

out vec2 texCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform float renderTime;

void main()
{
    // position = model * view * projection (but right to left order).
    gl_Position = projection * view * model * vec4(posIn, 1.0);
	texCoord = texCoordIn;
}
