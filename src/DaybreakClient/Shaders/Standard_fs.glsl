#version 330 core
struct Material
{
    vec3 ambientColor;
    sampler2D diffuse;
    vec3 diffuseColor;
    sampler2D specular;
    vec3 specularColor;
    float shininess;  
};

struct Light
{
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

out vec4 FragColor;

in vec2 ps_uv;
in vec3 ps_normal;
in vec3 ps_fragPos;

uniform Material material;
uniform Light light;

uniform float renderTime;
uniform vec3 viewPos;

void main()
{
    // Ambient lighting.
    vec3 ambient = light.ambient * material.ambientColor;

    // Diffuse lighting.
    vec3 normal = normalize(ps_normal);
    vec3 lightDirection = normalize(light.position - ps_fragPos);      // Vector from fragment to light.
    
    float diffuseFactor = max(dot(normal, lightDirection), 0.0);
    vec3 diffuse = light.diffuse * diffuseFactor * vec3(texture(material.diffuse, ps_uv)) * material.diffuseColor;

    // Specular lighting.
    float specularStrength = 0.5;
    vec3 viewDirection = normalize(viewPos - ps_fragPos);
    vec3 reflectDirection = reflect(-lightDirection, ps_normal);

    float specularFactor = pow(max(dot(viewDirection, reflectDirection), 0.0), material.shininess);
    vec3 specular = light.specular * specularFactor /* * vec3(texture(material.specular, ps_uv))*/ * material.specularColor;

    // Combine lighting terms with texture into final output color.
    vec3 result = ambient + diffuse + specular;

	FragColor = vec4(result, 1.0);
}
