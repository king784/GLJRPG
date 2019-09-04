#include "Texture.h"

// Single header image loading library by Sean Barrett
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Texture::Texture(const char* path)
{
    // Load images correct side up
    stbi_set_flip_vertically_on_load(true);  

    // Generate texture from image
    glGenTextures(1, &textureID);

    // Image loading
    int width, height, nrChannels;
    unsigned char* data = stbi_load(path, &width, &height, &nrChannels, 0);

    if(data)
    {
        GLenum format;
        if(nrChannels == 1)
        {
            format = GL_RED;        
        }
        else if(nrChannels == 3)
        {
            format = GL_RGB;        
        }
        else if(nrChannels == 4)
        {
            format = GL_RGBA;        
        }

        // Bind texture so we can generate the texture using image data
        glBindTexture(GL_TEXTURE_2D, textureID);

        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        // Set texture wrapping and filtering options on bound object
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }
    else
    {
        std::cout << "Failed to load texture!" << std::endl;
    }   

    // Free image memory
    stbi_image_free(data);
}

unsigned int Texture::GetID()
{
    return textureID;
}