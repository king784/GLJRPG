// Shader with color and only normals and positions.
#version 460 core
out vec4 FragColor;

uniform vec4 myColor;

void main()
{
    FragColor = vec4(myColor);
}