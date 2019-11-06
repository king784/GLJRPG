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
Camera mainCamera(SCREENWIDTH, SCREENHEIGHT, glm::vec3(0.0, 0.0, 0.0), 0.0f, 0.0f);

// Function prototypes
void FramebufferSizeCallback(GLFWwindow* window, int width, int height);

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

    std::string pathToRoot = "C:/Users/teemu.turku/Documents/GitHub/GLJRPG";

    Shader unlitShader((pathToRoot + "/Game/Shaders/Unlit.vs").c_str(), (pathToRoot + "/Game/Shaders/Unlit.fs").c_str(), "Unlit");

    Model box((pathToRoot + "/Game/Models/Cube/box.obj").c_str(), (pathToRoot + "/Game/Models/Cube/TestBox.png").c_str(), 
    unlitShader, glm::vec3(15.0, 0.0, 5.0), glm::vec3(3.0), false);


    // Enable depth test
    glEnable(GL_DEPTH_TEST);

    // Rendering loop
    while(!glfwWindowShouldClose(window))
    {
        glClearColor(0.1f, 0.35f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        unlitShader.Use();
        unlitShader.SetMat4("view", mainCamera.GetView());
        unlitShader.SetMat4("projection", mainCamera.GetProjection());
        box.Rotate((float)glfwGetTime());
        box.Draw();

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