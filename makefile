run: main.cpp Shader.cpp Camera.cpp
	g++ -o run main.cpp Shader.cpp Camera.cpp -lGLEW -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -lSOIL
