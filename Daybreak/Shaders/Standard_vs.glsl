#version 330 core
layout (location = 0) in vec3 posIn;
layout (location = 1) in vec3 colorIn;
layout (location = 2) in vec2 texCoordIn;

out vec3 vertexColor;
out vec2 texCoord;

out float _renderTime;			// TODO: Should this be a uniform for perf when passing to PS?

uniform float renderTime;

void main()
{
	gl_Position = vec4(posIn, 1.0);
	vertexColor = colorIn;
	texCoord = texCoordIn;

	_renderTime = renderTime;
}