@echo off
color 16
@echo Building GLFW Application...
g++ -I/Game Game/test.cpp Game/glad.c -lglfw3 -lopengl32 -lgdi32 -g -o test.exe
@echo Complete
pause