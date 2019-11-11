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
#include "GameObject.h" // Contains basic shapes
#include "Camera.h"
#include "Texture.h"
#include "Enums.h"
#include "AudioManager.h"
#include "Debug.h"
#include "Drawable.h"

// Global variables
const unsigned int SCREENWIDTH = 800;
const unsigned int SCREENHEIGHT = 600;
Camera mainCamera(SCREENWIDTH, SCREENHEIGHT, glm::vec3(2.0, 2.0, 5.0), -10.0f, 5.0f);

// Function prototypes
void FramebufferSizeCallback(GLFWwindow* window, int width, int height);
void ProcessInput(GLFWwindow* window, Model& player);
void ErrorCallback(int error, const char* description);

int main()
{
    Debug::GetInstance().StartTimer();
    // Set error callback
    glfwSetErrorCallback(ErrorCallback);

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

    // Path at school: C:/Users/teemu.turku/Documents/GitHub/GLJRPG
    // at home: D:/Projects/OpenGL/GLJRPG
    std::string pathToRoot = "D:/Projects/OpenGL/GLJRPG"; 

    // Shaders
    Shader backgroundShader((pathToRoot + "/Game/Shaders/Background.vs").c_str(), (pathToRoot + "/Game/Shaders/Background.fs").c_str(), "backgroundShader");
    Shader unlitShader((pathToRoot + "/Game/Shaders/Unlit.vs").c_str(), (pathToRoot + "/Game/Shaders/Unlit.fs").c_str(), "Unlit");
    Shader colorNShader((pathToRoot + "/Game/Shaders/ColorN.vs").c_str(), (pathToRoot + "/Game/Shaders/ColorN.fs").c_str(), "colorNShader");

    // Textures
    Texture bgTexture((pathToRoot + "/Game/Images/Background.png").c_str());

    // Models
    ScreenQuad bg(bgTexture.GetID(), backgroundShader);

    // Player
    Drawable* player = new Model((pathToRoot + "/Game/Models/hugi/Hugis.obj").c_str(), (pathToRoot + "/Game/Models/Cube/Colors.png").c_str(), 
    unlitShader, glm::vec3(4.0, -1.0, 1.0), glm::vec3(0.005), false);

    Drawable* barrel = new Model((pathToRoot + "/Game/Models/Barrel/barrel.obj").c_str(), (pathToRoot + "/Game/Models/Barrel/barrel.png").c_str(), 
    unlitShader, glm::vec3(8.0, -0.5, 3.0), glm::vec3(0.5), false);

    Cube maskBoxCube(glm::vec3(9.5, 0.0, 2.0), glm::vec3(1.5));

    // Path to box: "/Game/Models/Cube/box.png"

    // Enable depth test
    glEnable(GL_DEPTH_TEST);

    // Audio
    // Audiomanager::GetInstance().StartAudioManager();

    Debug::GetInstance().EndTimer();
    Debug::GetInstance().PrintTime();

    // List of drawables
    std::vector<Drawable*> drawables;
    drawables.push_back(player);
    drawables.push_back(barrel);

    // Main loop
    while(!glfwWindowShouldClose(window))
    {
        // Process inputs
        Model* playerModel = dynamic_cast<Model*>(player);
        ProcessInput(window, *playerModel);

        // Rendering
        glClearColor(0.1f, 0.35f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Draw background
        bg.Draw();

        // glBindTexture(GL_TEXTURE_2D, bgTexture.GetID());

        // glBindVertexArray(VAO);
        // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // Clear depth buffer so the background stays in the back, behind everything.
        glClear(GL_DEPTH_BUFFER_BIT);

        // Don't write to color output, but write to depth so we can mask out objects.
        glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

        colorNShader.Use();
        colorNShader.SetMat4("view", mainCamera.GetView());
        colorNShader.SetMat4("projection", mainCamera.GetProjection());
        maskBoxCube.Draw(colorNShader);

        // Re-enable writing to color output.
        glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);

        unlitShader.Use();

        // Create transformations
        //model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));

        //view  = glm::translate(view, glm::vec3(0.0f, -2.0f, 2.0f));
        //projection = glm::perspective(glm::radians(45.0f), (float)SCREENWIDTH / (float)SCREENHEIGHT, 0.1f, 100.0f);

        unlitShader.SetMat4("view", mainCamera.GetView());
        unlitShader.SetMat4("projection", mainCamera.GetProjection());

        // Loop for drawables
        for(auto &it : drawables)
        {
            (it)->Draw();
        }

        // player.Draw();
        //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // barrel.Draw();

        // Check and call events and swap buffers
        glfwSwapBuffers(window); 
        glfwPollEvents();   
    }

    // De-allocate resources
    // glDeleteVertexArrays(1, &VAO);
    // glDeleteBuffers(1, &VBO);
    // glDeleteBuffers(1, &EBO);
    // delete player;

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
void ProcessInput(GLFWwindow* window, Model& player)  
{
    // Escape key to close window
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }

    // Move player
    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        player.Move(glm::vec3(1.0, 0.0, 0.0));
        player.Rotate(90.0f);
    }
    if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        player.Move(glm::vec3(0.0, 0.0, 1.0));
        player.Rotate(0.0f);
    }
    if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        player.Move(glm::vec3(-1.0, 0.0, 0.0));
        player.Rotate(200.0f);
    }
    if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        player.Move(glm::vec3(0.0, 0.0, -1.0));
        player.Rotate(180.0f);
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

void ErrorCallback(int error, const char* description)
{
    std::cout << "Error callback called! Here is what happened: " << description << std::endl;
}