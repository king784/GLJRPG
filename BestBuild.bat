@echo Building GLFW Application...
g++ Game/main.cpp Game/glad.c -lglfw3 -lopengl32 -lgdi32 -o Game.exe
@echo Complete, running
Game.exe