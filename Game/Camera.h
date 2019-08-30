#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h> // include glad to get all the required OpenGL headers
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Camera
{
public:
   Camera(int& screenWidth, int& screenHeight);
private:
    // Current camera position
    glm::vec3 cameraPos;

    // Camera direction
    glm::vec3 cameraTarget;
    // Direction is actually pointing in reverse direction of the target
    glm::vec3 cameraDirection;

    // Get camera right vector by getting the cross product of worldUp and cameraDirection vectors
    glm::vec3 worldUp;
    glm::vec3 cameraRight;

    // Get camera up vector
    glm::vec3 cameraUp;

    // Camera front
    glm::vec3 cameraFront;
    
    // Use glm::lookAt to define the view matrix by providing the function the camera position,
    // target position and world vector up.
    glm::mat4 view;
    glm::mat4 projection;
};
  
#endif