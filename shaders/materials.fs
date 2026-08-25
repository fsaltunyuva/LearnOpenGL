#version 330 core

struct Material {
   vec3 ambient; // ambient light
   vec3 diffuse; // color of the surface under diffuse lighting
   vec3 specular; // specular highlight
   float shininess; // scattering of the specular highlight
};

struct Light {
   vec3 position;

   vec3 ambient;
   vec3 diffuse;
   vec3 specular;
};

uniform Material material;
uniform Light light;
uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 viewPos;

in vec3 Normal;
in vec3 FragPos; // will be interpolated in GPU from the 3 world position vectors of the fragment triangle

out vec4 FragColor;

void main()
{
   // ambient
   vec3 ambient  = light.ambient * material.ambient;

   // diffuse
   vec3 norm = normalize(Normal);
   vec3 lightDir = normalize(light.position - FragPos);
   float diff = max(dot(norm, lightDir), 0.0);
   vec3 diffuse  = light.diffuse * (diff * material.diffuse);

   // specular
   vec3 viewDir = normalize(viewPos - FragPos);
   vec3 reflectDir = reflect(-lightDir, norm); // lightDir points from the fragment towards the light source, so we negate it
   float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
   vec3 specular = light.specular * (spec * material.specular);

   vec3 result = ambient + diffuse + specular;
   FragColor = vec4(result, 1.0);
}