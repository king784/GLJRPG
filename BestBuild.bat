@echo Building GLFW Application...
g++ Game/main.cpp Game/glad.c Game/Shader.h Game/Shader.cpp Game/stb_image.h -lglfw3 -lopengl32 -lgdi32 -g -o Game.exe
@echo Complete, running
Game.exe