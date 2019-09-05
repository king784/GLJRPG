#ifndef STRUCTS_H
#define STRUCTS_H

#include <vector>

#include <glad/glad.h> // include glad to get all the required OpenGL headers
#include <glm/glm.hpp>

struct Vertex
{
    glm::vec3 vertice;
    glm::vec3 normal;
    glm::vec2 texCoord;

    Vertex(glm::vec3 newVertice, glm::vec3 newNormal, glm::vec2 newTexCoord)
    {
        vertice = newVertice;
        normal = newNormal;
        texCoord = newTexCoord;
    }
};

#endif