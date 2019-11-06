#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Shader.h"
#include "Model.h"
#include "Camera.h"
#include "GameObject.h"

// Global variables
const unsigned int SCREENWIDTH = 800;
const unsigned int SCREENHEIGHT = 600;
Camera mainCamera(SCREENWIDTH, SCREENHEIGHT, glm::vec3(0.0, 5.0, 5.0), 0.0f, -10.0f);

// Function prototypes
void FramebufferSizeCallback(GLFWwindow* window, int width, int height);
void ProcessInput(GLFWwindow* window);

int main()
{
    // Initialize GLFW and configure
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    // Create the window and set the width and height
    GLFWwindow* window = glfwCreateWindow(SCREENWIDTH, SCREENHEIGHT, "OpenGLRPG", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Initialize GLAD with OS-specific pointers so the glfwGetProcAddress defines the correct 
    // function based on which OS we're compiling for. 
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }    

    // Tell OpenGL the size of the rendering window
    glViewport(0, 0, SCREENWIDTH, SCREENHEIGHT);

    // Tell GLFW to call FramebufferSizeCallback on every window resize.
    glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback); 

    std::string pathToRoot = "D:/Projects/OpenGL/GLJRPG";

    Shader colorShader((pathToRoot + "/Game/Shaders/ColorShader.vs").c_str(), (pathToRoot + "/Game/Shaders/ColorShader.fs").c_str(), "Color");

    Cube* cube = new Cube(glm::vec3(0.0f), glm::vec3(1.0f));


    // Enable depth test
    glEnable(GL_DEPTH_TEST);

    // Rendering loop
    while(!glfwWindowShouldClose(window))
    {
        ProcessInput(window);

        glClearColor(0.1f, 0.35f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        colorShader.Use();
        colorShader.SetMat4("view", mainCamera.GetView());
        colorShader.SetMat4("projection", mainCamera.GetProjection());
        colorShader.SetVec3("myColor", glm::vec3(0.8f, 0.1f, 0.1f));
        cube->Draw(colorShader);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

// Resizes the viewport when window's size is changed.
void FramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void ProcessInput(GLFWwindow* window)  
{
    // Escape key to close window
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }

    // Debug Movement for moving the camera
    if(glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
    {
        mainCamera.MoveCamera(Direction::Forward);
    }
    if(glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
    {
        mainCamera.MoveCamera(Direction::Right);
    }
    if(glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
    {
        mainCamera.MoveCamera(Direction::Back);
    }
    if(glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
    {
        mainCamera.MoveCamera(Direction::Left);
    }
    // Rotating camera
    if(glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
    {
        mainCamera.RotateCamera(Direction::Right);
    }
    if(glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS)
    {
        mainCamera.RotateCamera(Direction::Left);
    }
    if(glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS)
    {
        mainCamera.RotateCamera(Direction::Forward);
    }
    if(glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS)
    {
        mainCamera.RotateCamera(Direction::Back);
    }
}