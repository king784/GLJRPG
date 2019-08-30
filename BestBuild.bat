@echo Building GLFW Application...
g++ Game/main.cpp Game/glad.c Game/Shader.h Game/Shader.cpp Game/Camera.h Game/Camera.cpp Game/stb_image.h Game/Model.cpp Game/Model.h -lglfw3 -lopengl32 -lgdi32 -g -o Game.exe
@echo Complete, running
Game.exe