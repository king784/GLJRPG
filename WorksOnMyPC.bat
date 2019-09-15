color 02
@echo Building GLFW Application...
g++ -I/Game Game/main.cpp Game/glad.c Game/Shader.cpp Game/Model.cpp Game/Camera.cpp Game/Texture.cpp Game/AudioManager.cpp -static-libstdc++ -static-libgcc -lglfw3 -lopengl32 -lgdi32 -lOpenAL32 -lalut -g -o Game.exe
@echo Complete
pause