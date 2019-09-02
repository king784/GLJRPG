#include "Camera.h"

Camera::Camera(const int& screenWidth, const int& screenHeight)
{
    // Camera definitions
    // Positive z-axis goes towards the screen and towards you, so we want to move the camera that way.
    cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);

    // Camera direction
    cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
    // Direction is actually pointing in reverse direction of the target
    cameraDirection = glm::normalize(cameraPos - cameraTarget);

    // Get camera right vector by getting the cross product of worldUp and cameraDirection vectors
    worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
    cameraRight = glm::normalize(glm::cross(worldUp, cameraDirection));

    // Get camera up vector
    cameraUp = glm::cross(cameraDirection, cameraRight);

    // Camera front
    cameraFront = glm::vec3(0.0, 0.0, -1.0f);
    
    // Use glm::lookAt to define the view matrix by providing the function the camera position,
    // target position and world vector up.
    view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

    projection = glm::perspective(glm::radians(45.0f), (float)screenWidth / (float)screenHeight, 0.1f, 100.0f);
}

glm::vec3 Camera::GetCameraPos()
{
    return cameraPos;
}

glm::vec3 Camera::GetCameraTarget()
{
    return cameraTarget;
}

glm::vec3 Camera::GetCameraUp()
{
    return cameraUp;
}

glm::vec3 Camera::GetCameraRight()
{
    return cameraRight;
}

glm::mat4 Camera::GetView()
{
    return view;
}

glm::mat4 Camera::GetProjection()
{
    return projection;
}

void Camera::SetCameraPos(glm::vec3 newPos)
{
    cameraPos = newPos;
}

void Camera::MoveCamera(Direction dir)
{
    switch (dir)
    {
    case Direction::Forward:
        cameraPos += cameraSpeed * cameraFront;
        break;

    case Direction::Right:
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
        break;

    case Direction::Back:
        cameraPos -= cameraSpeed * cameraFront;
        break;

    case Direction::Left:
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
        break;
    
    default:
        break;
    }
    view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
}