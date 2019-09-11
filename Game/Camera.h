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
   Camera(const int& screenWidth, const int& screenHeight, glm::vec3 startPosition = glm::vec3(0.0, 0.0, 3.0), float startYaw = 0.0f, float startPitch = 0.0f);

    // Getters and setters
    glm::vec3 GetCameraPos();
    glm::vec3 GetCameraFront();
    glm::vec3 GetCameraUp();
    glm::vec3 GetCameraRight();
    glm::mat4 GetView();
    glm::mat4 GetProjection();

    void SetCameraPos(glm::vec3 newPos);
    void MoveCamera(Direction dir);
    void RotateCamera(Direction dir);
private:
    // Update camera vectors
    void UpdateVectors();

    float yaw = 0.0f;
    float pitch = 0.0f;
    // Current camera position
    glm::vec3 cameraPos;

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