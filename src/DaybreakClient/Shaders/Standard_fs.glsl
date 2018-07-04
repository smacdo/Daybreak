#version 330 core
#define MAX_DIRECTIONAL_LIGHT_COUNT 3
#define MAX_POINT_LIGHT_COUNT 4

struct Material
{
    vec3 ambientColor;
    sampler2D diffuse;
    vec3 diffuseColor;
    sampler2D specular;
    vec3 specularColor;
    float shininess;

    bool hasDiffuseTexture;
    bool hasSpecularTexture;
};

struct DirectionalLight
{
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;  
};

struct PointLight
{
    vec3 position;
    float constant;
    float linear;
    float quadratic;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;  
};

out vec4 FragColor;

in vec2 ps_uv;
in vec3 ps_normal;
in vec3 ps_fragPos;

uniform Material material;

uniform float renderTime;
uniform vec3 viewPos;

uniform int directionalLightCount;
uniform DirectionalLight directionalLights[MAX_DIRECTIONAL_LIGHT_COUNT];
uniform int pointLightCount;
uniform PointLight pointLights[MAX_POINT_LIGHT_COUNT];

vec3 CalculateDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDirection);
vec3 CalculatePointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDirection);

void main()
{
    // Calculate shared properties.
    vec3 normal = normalize(ps_normal);
    vec3 viewDirection = normalize(viewPos - ps_fragPos);
    vec3 resultColor = material.ambientColor;

    // Add all directional light contribution.
    for (int i = 0; i < directionalLightCount; i++)
    {
        resultColor += CalculateDirectionalLight(directionalLights[i], normal, viewDirection);
    }

    // Add all point light contribution.
    for (int i = 0; i < pointLightCount; i++)
    {
        resultColor += CalculatePointLight(pointLights[i], normal, ps_fragPos, viewDirection);
    }

    // TODO: Add all spot light contribution.

	FragColor = vec4(resultColor, 1.0);
}

//---------------------------------------------------------------------------------------------------------------------
vec3 CalculateDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDirection)
{
    vec3 lightDirection = normalize(-light.direction);

    // Diffuse shading.
    float diffuseFactor = max(dot(normal, lightDirection), 0.0);
    vec3 diffuseColor = material.hasDiffuseTexture ? vec3(texture(material.diffuse, ps_uv)) : material.diffuseColor;

    // Specular shading.
    vec3 reflectDirection = reflect(-lightDirection, normal);
    float specularFactor = pow(max(dot(viewDirection, reflectDirection), 0.0), material.shininess);
    vec3 specularColor = material.hasSpecularTexture ? vec3(texture(material.specular, ps_uv)) : material.specularColor;

    // Combine lighting parts.
    vec3 ambient = light.ambient * diffuseColor;
    vec3 diffuse = light.diffuse * diffuseFactor * diffuseColor;
    vec3 specular = light.specular * specularFactor * specularColor;

    return (ambient + diffuse + specular);
}

//---------------------------------------------------------------------------------------------------------------------
vec3 CalculatePointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDirection)
{
    vec3 lightDirection = normalize(light.position - fragPos);

    // Diffuse shading.
    float diffuseFactor = max(dot(normal, lightDirection), 0.0);
    vec3 diffuseColor = material.hasDiffuseTexture ? vec3(texture(material.diffuse, ps_uv)) : material.diffuseColor;

    // Specular shading.
    vec3 reflectDirection = reflect(-lightDirection, normal);
    float specularFactor = pow(max(dot(viewDirection, reflectDirection), 0.0), material.shininess);
    vec3 specularColor = material.hasSpecularTexture ? vec3(texture(material.specular, ps_uv)) : material.specularColor;

    // Combine lighting parts.
    vec3 ambient = light.ambient * diffuseColor;
    vec3 diffuse = light.diffuse * diffuseFactor * diffuseColor;
    vec3 specular = light.specular * specularFactor * specularColor;

    // Attenuate light.
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    return (ambient + diffuse + specular);
}
