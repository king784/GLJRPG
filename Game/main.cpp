// Cpp includes
#include <iostream>

// Import GLAD before GLFW because GLAD contains the correct OpenGL header includes
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// Own includes
#include "Shader.h"

// Single header image loading library by Sean Barrett
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

// Global variables
const unsigned int SCREENWIDTH = 800;
const unsigned int SCREENHEIGHT = 600;

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
    GLFWwindow* window = glfwCreateWindow(SCREENWIDTH, SCREENHEIGHT, "LearnOpenGL", NULL, NULL);
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
    Shader unlitShader("C:/Users/teemu.turku/Documents/GitHub/GLJRPG/Game/Shaders/Unlit.vs", "C:/Users/teemu.turku/Documents/GitHub/GLJRPG/Game/Shaders/Unlit.fs");

    // -1, -1, 0 is bottom left of screen and 1, 1, 0 is top right of screen.
    // Triangle
    float vertices[] = 
    {
    -1.0f, -1.0f, 0.0f,
     0.5f, -1.5f, 0.0f,
     0.0f,  0.5f, 0.0f
    };  

    // Create memory on the GPU, where we want to store the vertex data by using Vertex Buffer Objects(VBO).
    // We can send a lot of data to the GPU at once using these buffers.
    // Once the data has been sent to GPU, it has almost instant access to the vertices, making the rendering fast.
    // Vertex Array Object can also be bound like VBO. VAO has an advantage that when configuring vertex attribute pointers you only
    // have to make those calls once and when we want to draw an object, we can just bind the corresponding VAO.
    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    // Bind vertex array
    glBindVertexArray(VAO);
    // Bind buffer so we can send the vertex buffer object.
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Unbind buffer
    glBindBuffer(GL_ARRAY_BUFFER, 0); 

    // Unbind VAO after so VAO calls won't accidentally modify this VAO.
    glBindVertexArray(0);

    unlitShader.Use();

    // Main loop
    while(!glfwWindowShouldClose(window))
    {
        // Process inputs
        ProcessInput(window);

        // Rendering
        glClearColor(0.1f, 0.2f, 0.35f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // Check and call events and swap buffers
        glfwPollEvents();  
        glfwSwapBuffers(window);  
    }

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
}