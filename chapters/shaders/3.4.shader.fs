#version 330 core

out vec4 FragColor; // Output color variable

in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D texture1; // for texture objects
uniform sampler2D texture2; // for texture objects

void main()
{
//   FragColor = vec4(ourColor, 1.0);
//   FragColor = texture(texture1, TexCoord); // texture function samples corresponding color value using the texture params we set earlier
//   FragColor = texture(texture1, TexCoord) * vec4(ourColor, 1.0);
   FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.2); // mix linearly interpolates based on third argument
   // if 3rd argument is 0, it returns the first input
   // if 3rd argument is 0.2, it returns 80% of the first, and 20% of the second color
}