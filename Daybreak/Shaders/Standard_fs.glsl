#version 330 core
out vec4 FragColor;

in vec3 vertexColor;
in vec2 texCoord;

uniform sampler2D texSampler;

in float _renderTime;		// TODO: Should this be a uniform?

void main()
{
	FragColor = texture(texSampler, texCoord) * vec4(vertexColor, 1.0);;
}
