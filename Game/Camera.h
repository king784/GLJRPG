#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h> // include glad to get all the required OpenGL headers
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// My includes
#include "Enums.h"

class Camera
{
public:
   Camera(const int& screenWidth, const int& screenHeight);

    // Getters and setters
    glm::vec3 GetCameraPos();
    glm::vec3 GetCameraTarget();
    glm::vec3 GetCameraUp();
    glm::vec3 GetCameraRight();
    glm::mat4 GetView();
    glm::mat4 GetProjection();

    void SetCameraPos(glm::vec3 newPos);
    void MoveCamera(Direction dir);
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

    // Speed of camera
    float cameraSpeed = 0.1f;
};
  
#endif