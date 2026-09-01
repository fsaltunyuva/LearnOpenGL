#version 330 core

struct Material {
   sampler2D diffuse;
   sampler2D specular; // specular highlight
   float shininess; // scattering of the specular highlight
};

struct Light {
   vec3 position; // not needed for directional lights
   vec3 direction;
   float cutOff; // needed for spotlights
   float outerCutOff; // needed for spotlights

   vec3 ambient;
   vec3 diffuse;
   vec3 specular;

   // attenuation values
   float constant;
   float linear;
   float quadratic;
};

uniform Material material;
uniform Light light;
uniform vec3 viewPos;

in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos; // will be interpolated in GPU from the 3 world position vectors of the fragment triangle

out vec4 FragColor;

void main()
{
   // attenuation
   float distance    = length(light.position - FragPos);
   float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance)); // https://learnopengl.com/Lighting/Light-casters

   // directional light
   // vec3 lightDir = normalize(-light.direction); // we need the direction from fragment towards light source but light direction is reverse
   // point light
   vec3 lightDir = normalize(light.position - FragPos);
   float theta = dot(lightDir, normalize(-light.direction)); // theta angle of the spotlight
   // spotlight smooth edges
   float epsilon   = light.cutOff - light.outerCutOff;
   float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

   // ambient
   vec3 ambient  = light.ambient * vec3(texture(material.diffuse, TexCoords));

   // diffuse
   vec3 norm = normalize(Normal);
   float diff = max(dot(norm, lightDir), 0.0);
   vec3 diffuse  = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));

   // specular
   vec3 viewDir = normalize(viewPos - FragPos);
   vec3 reflectDir = reflect(-lightDir, norm); // lightDir points from the fragment towards the light source, so we negate it
   float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
   vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));

   // apply attenuation
   ambient  *= attenuation;
   diffuse  *= attenuation;
   specular *= attenuation;

   // spotlight smooth edges
   diffuse  *= intensity;
   specular *= intensity;

   vec3 result = ambient + diffuse + specular;

   FragColor = vec4(result, 1.0);
}