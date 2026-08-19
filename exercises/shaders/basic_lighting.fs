#version 330 core

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 viewPos;

in vec3 Normal;
in vec3 FragPos; // will be interpolated in GPU from the 3 world position vectors of the fragment triangle
in vec3 LightPos;

out vec4 FragColor;

float specularStrength = 0.5;

void main()
{
   vec3 norm = normalize(Normal);
   // vec3 lightDir = normalize(lightPos - FragPos);

   // exercise 3 - view space
   vec3 lightDir = normalize(LightPos - FragPos);

   // diffuse
   float diff = max(dot(norm, lightDir), 0.0);
   vec3 diffuse = diff * lightColor;

   // specular
   vec3 viewDir = normalize(vec3(0,0,0) - FragPos);
   vec3 reflectDir = reflect(-lightDir, norm); // lightDir points from the fragment towards the light source, so we negate it
   float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
   vec3 specular = specularStrength * spec * lightColor;

   // ambient
   float ambientStrength = 0.1;
   vec3 ambient = ambientStrength * lightColor;

   vec3 result = (ambient + diffuse + specular) * objectColor;
   FragColor = vec4(result, 1.0);
}