#version 460 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D myTexture;

void main()
{
    FragColor = vec4(0.2f, 0.2f, 0.7f, 1.0f); //texture(myTexture, TexCoord);
}