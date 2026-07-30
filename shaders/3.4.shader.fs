#version 330 core

out vec4 FragColor; // Output color variable

in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D texture1; // for texture objects

void main()
{
//   FragColor = vec4(ourColor, 1.0);
//   FragColor = texture(texture1, TexCoord); // texture function samples corresponding color value using the texture params we set earlier
   FragColor = texture(texture1, TexCoord) * vec4(ourColor, 1.0);
}