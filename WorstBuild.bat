@echo off
color 16
@echo Building GLFW Application...
g++ -I/Game Game/test.cpp Game/glad.c Game/Shader.cpp Game/Model.cpp Game/Camera.cpp Game/Texture.cpp Game/Collider.cpp -lglfw3 -lopengl32 -lgdi32 -g -o test.exe
@echo Complete
pause