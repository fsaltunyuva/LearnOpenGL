#version 330 core

out vec4 FragColor; // Output color variable
// uniform vec4 ourColor;
in vec3 ourColor;

void main()
{
   FragColor = vec4(ourColor, 1.0); // Set fixed orange color
}