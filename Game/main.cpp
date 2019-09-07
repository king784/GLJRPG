// Cpp includes
#include <iostream>

// Import GLAD before GLFW because GLAD contains the correct OpenGL header includes
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// OpenGL Mathematics includes
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Own includes
#include "Shader.h"
#include "Model.h"
#include "Camera.h"
#include "Texture.h"
#include "Enums.h"
#include "AudioManager.h"

// Global variables
const unsigned int SCREENWIDTH = 800;
const unsigned int SCREENHEIGHT = 600;
Camera mainCamera(SCREENWIDTH, SCREENHEIGHT);

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

    // Create the window and set width and height
    GLFWwindow* window = glfwCreateWindow(SCREENWIDTH, SCREENHEIGHT, "OpenGLRPG", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Initialize GLAD with OS-specific pointers so the glfwGetProcAddress defines the correct function based on which OS we're compiling for. 
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }    

    // Tell OpenGL the size of the rendering window
    glViewport(0, 0, SCREENWIDTH, SCREENHEIGHT);

    // Tell GLFW to call FramebufferSizeCallback on every window resize.
    glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback); 

    // Shaders

    // Path at school: C:/Users/teemu.turku/Documents/GitHub/GLJRPG
    // at home: D:/Projects/OpenGL/GLJRPG
    std::string pathToRoot = "D:/Projects/OpenGL/GLJRPG"; 
    Shader backgroundShader((pathToRoot + "/Game/Shaders/Background.vs").c_str(), (pathToRoot + "/Game/Shaders/Background.fs").c_str());
    Shader unlitShader((pathToRoot + "/Game/Shaders/Unlit.vs").c_str(), (pathToRoot + "/Game/Shaders/Unlit.fs").c_str());

    // Textures
    Texture bgTexture((pathToRoot + "/Game/Images/Background.png").c_str());

    // Path to box: "/Game/Models/Cube/box.png"

    // Models
    Model cube((pathToRoot + "/Game/Models/hugi/Hugis.obj").c_str(), (pathToRoot + "/Game/Models/Cube/box.png").c_str());

    // -1, -1, 0 is bottom left of screen and 1, 1, 0 is top right of screen.
    // Triangle
    float vertices[] = 
    {
        // positions        // texture coords
        1.0f,  1.0f, 0.0f, 1.0f, 1.0f,   // top right
        1.0f, -1.0f, 0.0f, 1.0f, 0.0f,   // bottom right
        -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,   // bottom left
        -1.0f,  1.0f, 0.0f, 0.0f, 1.0f    // top left 
    };
    
    // How to draw the vertices
    unsigned int indices[] = 
    {  
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };

    // Create memory on the GPU, where we want to store the vertex data by using Vertex Buffer Objects(VBO).
    // We can send a lot of data to the GPU at once using these buffers.
    // Once the data has been sent to GPU, it has almost instant access to the vertices, making the rendering fast.
    // Vertex Array Object can also be bound like VBO. VAO has an advantage that when configuring vertex attribute pointers you only
    // have to make those calls once and when we want to draw an object, we can just bind the corresponding VAO.
    // Element Buffer Objects let us use previously defined vertices to draw shapes so we don't need to define the same
    // vertices twice.
    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    // Bind vertex array
    glBindVertexArray(VAO);
    // Bind buffer so we can send the vertex buffer object.
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Positions
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Texture coordinates
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Unbind buffer
    glBindBuffer(GL_ARRAY_BUFFER, 0); 

    // Unbind VAO after so VAO calls won't accidentally modify this VAO.
    glBindVertexArray(0);

    // Enable depth test
    glEnable(GL_DEPTH_TEST);

    // Audio
    Audiomanager audiomanager = Audiomanager();

    // Main loop
    while(!glfwWindowShouldClose(window))
    {
        // Process inputs
        ProcessInput(window);

        // Rendering
        glClearColor(0.1f, 0.35f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        unlitShader.Use();
        // Create transformations
        glm::mat4 model = glm::mat4(1.0f);
        //model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
        model  = glm::translate(model, glm::vec3(6.0f, 0.0f, 0.0f));
        //view  = glm::translate(view, glm::vec3(0.0f, -2.0f, 2.0f));
        //projection = glm::perspective(glm::radians(45.0f), (float)SCREENWIDTH / (float)SCREENHEIGHT, 0.1f, 100.0f);

        unlitShader.SetMat4("model", model);
        unlitShader.SetMat4("view", mainCamera.GetView());
        unlitShader.SetMat4("projection", mainCamera.GetProjection());
        cube.Draw(unlitShader);
        //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // Draw background
        // backgroundShader.Use();
        // glBindTexture(GL_TEXTURE_2D, bgTexture.GetID());

        // glBindVertexArray(VAO);
        // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // Check and call events and swap buffers
        glfwSwapBuffers(window); 
        glfwPollEvents();   
    }

    // De-allocate resources
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    // Clean GLFW resources that were allocated.
    glfwTerminate();
    return 0;
}

// Functions
// Resizes the viewport when window's size is changed.
void FramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}
// Input
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