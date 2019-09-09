#ifndef MODEL_H
#define MODEL_H

#include <glad/glad.h> // include glad to get all the required OpenGL headers
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include "Shader.h"
#include "Texture.h"
#include "Structs.h"  
#include "Enums.h"

class Model
{
public:
    // the program ID
    unsigned int ID;
  
    // Constructer reads and creates the shader
    Model(const char* modelPath, const char* texturePath, glm::vec3 startPosition = glm::vec3(0.0, 0.0, 0.0), glm::vec3 startScale = glm::vec3(1.0));

    // Destructor
    ~Model();

    // Use the shader
    void Draw(Shader& shader);

    // Set position of model
    void SetPosition(glm::vec3 newPosition);

    void Move(glm::vec3 direction, float speed = 0.1f);
private:
    unsigned int VBO, VAO, EBO;
    std::vector<Vertex> vertices;
    std::vector<glm::vec3> verticesPos;
    std::vector<glm::vec2> texCoords;
    std::vector<glm::vec3> normals;
    std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;

    Texture* texture;

    glm::vec3 position;
    glm::vec3 scale;
    //std::vector<GLushort> indices;
};
  
#endif