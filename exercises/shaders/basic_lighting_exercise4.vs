#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 lightPos;

out vec3 Normal;
out vec3 FragPos;
out vec3 LightPos;

// exercise 4
out vec3 ourColor;

float specularStrength = 0.5;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 viewPos;

void main()
{
   gl_Position = projection * view * model * vec4(aPos, 1.0);
   // FragPos = vec3(model * vec4(aPos, 1.0)); // we need world space so multiplying with model matrix is enough
   // Normal = mat3(transpose(inverse(model))) * aNormal; // non uniform scaling effects the normal vector, we need this

   // exercise 3 - view space
   FragPos = vec3(view * model * vec4(aPos, 1.0));
   Normal = mat3(transpose(inverse(view * model))) * aNormal;
   LightPos = vec3(view * vec4(lightPos, 1.0)); // lightPos do not need to be in clip space its only used in fragment shader



   vec3 norm = normalize(Normal);

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
   ourColor = result;
}