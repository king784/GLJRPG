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
#include "Collider.h"

class Collider;
class Shader;
class Texture;

class Model
{
public:
    // the program ID
    unsigned int ID;
  
    // Constructer reads and creates the shader
    Model(const char* modelPath, const char* texturePath, Shader theShader, glm::vec3 startPosition = glm::vec3(0.0, 0.0, 0.0), 
    glm::vec3 startScale = glm::vec3(1.0), bool hasCol = false);

    // Destructor
    ~Model();

    // Use the shader
    void Draw();

    // Set position of model
    void SetPosition(glm::vec3 newPosition);

    void Move(glm::vec3 direction, float speed = 0.1f);

    void Rotate(float newAngle);
private:
    void ProcessVertex(std::vector<std::string> vertexData,float* textureArray, float* normalsArray);
    std::vector<std::string> Split(std::string strToSplit, char delimeter);

    unsigned int VBO, VAO, EBO;
    std::vector<Vertex> vertices;
    std::vector<glm::vec3> verticesPos;
    std::vector<glm::vec2> texCoords;
    std::vector<glm::vec3> normals;
    std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;

    Texture* texture;

    glm::vec3 position;
    glm::vec3 scale;

    Shader* shader;
    Collider* collider;

    glm::mat4 model = glm::mat4(1.0f);
    float currentRot = 0.0f;
    float lastGivenAngle = 0.0f;
    float desiredRot;
    bool needToRotate = false;
    //std::vector<GLushort> indices;
};
  
#endif