#version 330 core

struct Material {
   sampler2D diffuse;
   sampler2D specular; // specular highlight
   float shininess; // scattering of the specular highlight
};

struct DirLight {
   vec3 direction;

   vec3 ambient;
   vec3 diffuse;
   vec3 specular;
};

struct PointLight {
   vec3 position;

   float constant;
   float linear;
   float quadratic;

   vec3 ambient;
   vec3 diffuse;
   vec3 specular;
};
#define NR_POINT_LIGHTS 4

struct SpotLight {
   vec3 position;
   vec3 direction;
   float cutOff;
   float outerCutOff;

   float constant;
   float linear;
   float quadratic;

   vec3 ambient;
   vec3 diffuse;
   vec3 specular;
};

uniform Material material;
uniform vec3 viewPos;

uniform DirLight dirLight;
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform SpotLight spotLight;

in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos; // will be interpolated in GPU from the 3 world position vectors of the fragment triangle

out vec4 FragColor;

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(SpotLight spotLight, vec3 norm, vec3 fragPos, vec3 viewDir);

void main()
{
   // properties
   vec3 norm = normalize(Normal);
   vec3 viewDir = normalize(viewPos - FragPos);

   // phase 1: Directional lighting
   vec3 result = CalcDirLight(dirLight, norm, viewDir);

   // phase 2: Point lights
   for(int i = 0; i < NR_POINT_LIGHTS; i++)
   result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);

   // phase 3: Spot light
   result += CalcSpotLight(spotLight, norm, FragPos, viewDir);

   FragColor = vec4(result, 1.0);
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
   vec3 lightDir = normalize(-light.direction);

   // diffuse shading
   float diff = max(dot(normal, lightDir), 0.0);

   // specular shading
   vec3 reflectDir = reflect(-lightDir, normal);
   float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

   // combine results
   vec3 ambient  = light.ambient  * vec3(texture(material.diffuse, TexCoords));
   vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.diffuse, TexCoords));
   vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));

   return (ambient + diffuse + specular);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
   vec3 lightDir = normalize(light.position - fragPos);

   // diffuse shading
   float diff = max(dot(normal, lightDir), 0.0);

   // specular shading
   vec3 reflectDir = reflect(-lightDir, normal);
   float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

   // attenuation
   float distance    = length(light.position - fragPos);
   float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

   // combine results
   vec3 ambient  = light.ambient  * vec3(texture(material.diffuse, TexCoords));
   vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.diffuse, TexCoords));
   vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
   ambient  *= attenuation;
   diffuse  *= attenuation;
   specular *= attenuation;

   return (ambient + diffuse + specular);
}

vec3 CalcSpotLight(SpotLight spotLight, vec3 normal, vec3 fragPos, vec3 viewDir)
{
   vec3 lightDir = normalize(spotLight.position - fragPos);
   float theta = dot(lightDir, normalize(-spotLight.direction)); // theta angle of the spotlight

   // spotlight smooth edges
   float epsilon   = spotLight.cutOff - spotLight.outerCutOff;
   float intensity = clamp((theta - spotLight.outerCutOff) / epsilon, 0.0, 1.0);

   // diffuse shading
   float diff = max(dot(normal, lightDir), 0.0);

   // specular shading
   vec3 reflectDir = reflect(-lightDir, normal);
   float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

   // attenuation
   float distance    = length(spotLight.position - fragPos);
   float attenuation = 1.0 / (spotLight.constant + spotLight.linear * distance + spotLight.quadratic * (distance * distance));

   // combine results
   vec3 ambient  = spotLight.ambient  * vec3(texture(material.diffuse, TexCoords));
   vec3 diffuse  = spotLight.diffuse  * diff * vec3(texture(material.diffuse, TexCoords));
   vec3 specular = spotLight.specular * spec * vec3(texture(material.specular, TexCoords));
   ambient  *= attenuation;
   diffuse  *= attenuation;
   specular *= attenuation;

   // spotlight smooth edges
   diffuse  *= intensity;
   specular *= intensity;

   return (ambient + diffuse + specular);
}
