#ifndef TEXTURE_H
#define TEXTURE_H

#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Texture
{
public:
    Texture(const char* path);
    unsigned int GetID();


private:
    // The ID of the texture, given by OpenGL
    unsigned int textureID;
};

#endif