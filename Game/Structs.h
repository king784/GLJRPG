#ifndef STRUCTS_H
#define STRUCTS_H

#include <vector>

#include <glad/glad.h> // include glad to get all the required OpenGL headers
#include <glm/glm.hpp>

struct Vertex
{
    glm::vec3 vertices;
    glm::vec3 normals;
    glm::vec2 texCoods;

    Vertex(glm::vec3 newVertices, glm::vec3 newNormals, glm::vec2 newTexCoords)
    {
        vertices = newVertices;
        normals = newNormals;
        texCoods = newTexCoords;
    }
};

#endif