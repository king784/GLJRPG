#include "Camera.h"

Camera::Camera(const int& screenWidth, const int& screenHeight, glm::vec3 startPosition, float startYaw, float startPitch)
{
    // Camera definitions
    // Positive z-axis goes towards the screen and towards you, so we want to move the camera that way.
    cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);

    // World up vector
    worldUp = glm::vec3(0.0f, 1.0f, 0.0f);

    // Camera front
    cameraFront = glm::vec3(0.0, 0.0, -1.0f);

    // Get camera right vector by getting the cross product of worldUp and cameraDirection vectors
    cameraRight = glm::normalize(glm::cross(worldUp, cameraFront));

    // Get camera up vector
    cameraUp = glm::cross(cameraFront, cameraRight);
    
    // Use glm::lookAt to define the view matrix by providing the function the camera position,
    // target position and world vector up.
    view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

    projection = glm::perspective(glm::radians(45.0f), (float)screenWidth / (float)screenHeight, 0.1f, 10000.0f);

    yaw = startYaw;
    pitch = startPitch;

    UpdateVectors();
}

glm::vec3 Camera::GetCameraPos()
{
    return cameraPos;
}

glm::vec3 Camera::GetCameraFront()
{
    return cameraFront;
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
    view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
}

void Camera::MoveCamera(Direction dir)
{
    switch (dir)
    {
    case Direction::Forward:
        cameraPos += cameraSpeed * cameraFront;
        break;

    case Direction::Right:
        cameraPos += cameraRight * cameraSpeed;
        break;

    case Direction::Back:
        cameraPos -= cameraSpeed * cameraFront;
        break;

    case Direction::Left:
        cameraPos -= cameraRight * cameraSpeed;
        break;
    
    default:
        break;
    }
    view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
}

void Camera::RotateCamera(Direction dir, float speed)
{
    switch (dir)
    {
    case Direction::Forward:
        pitch += 1.0f * speed;
        if(pitch > 89.0f)
        {
            pitch = 89.0f;
        }
        if(pitch < -89.0f)
        {
            pitch = -89.0f;
        }
        break;

    case Direction::Right:
        yaw += 1.0f * speed;
        break;

    case Direction::Back:
        pitch -= 1.0f * speed;
        if(pitch > 89.0f)
        {
            pitch = 89.0f;
        }
        if(pitch < -89.0f)
        {
            pitch = -89.0f;
        }
        break;

    case Direction::Left:
        yaw -= 1.0f * speed;
        break;
    
    default:
        break;
    }
    UpdateVectors();
}

void Camera::UpdateVectors()
{
    // Calculate the new Front vector
    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(front);
    // Also re-calculate the Right and Up vector
    cameraRight = glm::normalize(glm::cross(cameraFront, worldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
    cameraUp = glm::normalize(glm::cross(cameraRight, cameraFront));
    view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
}