#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 Normal;
out vec3 FragPos; // we need fragment's position IN WORLD SPACE for shading in fs

void main()
{
   gl_Position = projection * view * model * vec4(aPos, 1.0);
   FragPos = vec3(model * vec4(aPos, 1.0)); // we need world space so multiplying with model matrix is enough
   Normal = mat3(transpose(inverse(model))) * aNormal; // non uniform scaling effects the normal vector, we need this
}