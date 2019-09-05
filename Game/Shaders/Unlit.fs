#version 460 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D myTexture;

void main()
{
    FragColor = texture(myTexture, TexCoord);// vec4(1.0);
}