#include "Camera.h"

Camera::Camera(int& screenWidth, int& screenHeight)
{
    // Camera definitions
    // Positive z-axis goes towards the screen and towards you, so we want to move the camera that way.
    cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);

    // Camera direction
    cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
    // Direction is actually pointing in reverse direction of the target
    cameraDirection = glm::normalize(cameraPos - cameraTarget);

    // Get camera right vector by getting the cross product of worldUp and cameraDirection vectors
    glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 cameraRight = glm::normalize(glm::cross(worldUp, cameraDirection));

    // Get camera up vector
    glm::vec3 cameraUp = glm::cross(cameraDirection, cameraRight);

    // Camera front
    glm::vec3 cameraFront = glm::vec3(0.0, 0.0, -1.0f);
    
    // Use glm::lookAt to define the view matrix by providing the function the camera position,
    // target position and world vector up.
    glm::mat4 view;
    view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

    projection = glm::perspective(glm::radians(45.0f), (float)screenWidth / (float)screenHeight, 0.1f, 100.0f);
}