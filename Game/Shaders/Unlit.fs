#version 460 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D myTexture;

void main()
{
    FragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f); //texture(myTexture, TexCoord);
}