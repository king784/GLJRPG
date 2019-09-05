#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h> // include glad to get all the required OpenGL headers
#include <glm/glm.hpp>
  
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
  

class Shader
{
public:
    // the program ID
    unsigned int ID;
  
    // Constructer reads and creates the shader
    Shader(const char* vertexPath, const char* fragmentPath);
    // Use the shader
    void Use();
    // utility uniform functions
    void SetBool(const std::string &name, bool value) const;  
    void SetInt(const std::string &name, int value) const;  
    void SetFloat(const std::string &name, float value) const;
    void SetVec3(const std::string &name, const glm::vec3 &value) const;
    void SetMat4(const std::string &name, const glm::mat4 &mat) const;
};
  
#endif