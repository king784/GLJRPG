#ifndef MODEL_H
#define MODEL_H

#include <glad/glad.h> // include glad to get all the required OpenGL headers
#include <glm/glm.hpp>

#include <vector>  
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Model
{
public:
    // the program ID
    unsigned int ID;
  
    // Constructer reads and creates the shader
    Model(const char* modelPath);
    // Use the shader
    void Draw();
private:
    unsigned int VBO, VAO, EBO;
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec2> texCoods;
    std::vector<glm::vec3> normals;
    //std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
    std::vector<GLushort> indices;
};
  
#endif