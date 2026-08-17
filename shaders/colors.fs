#version 330 core

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;

in vec3 Normal;
in vec3 FragPos; // will be interpolated in GPU from the 3 world position vectors of the fragment triangle

out vec4 FragColor;

void main()
{
   vec3 norm = normalize(Normal);
   vec3 lightDir = normalize(lightPos - FragPos);

   // diffuse
   float diff = max(dot(norm, lightDir), 0.0);
   vec3 diffuse = diff * lightColor;

   float ambientStrength = 0.1;
   vec3 ambient = ambientStrength * lightColor;

   vec3 result = (ambient + diffuse) * objectColor;
   FragColor = vec4(result, 1.0);
}