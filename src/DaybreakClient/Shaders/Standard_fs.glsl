#version 330 core
out vec4 FragColor;

in vec2 texCoord;

uniform sampler2D texture1;
uniform sampler2D texture2;
uniform float renderTime;

void main()
{
    vec3 vertexColor = vec3(1.0f, 1.0f, 1.0f);
	FragColor = mix(texture(texture1, texCoord), texture(texture2, texCoord), 0.2) * vec4(vertexColor, 1.0);
}
