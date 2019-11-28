CC=g++
CFLAGS=-Wall -g -std=c++14 `sdl-config --cflags`
LDFLAGS=-lGL -lGLU -lGLEW -lSDL -lpng -lz
SOURCES=main.cpp InputHandler.cpp RenderContext.cpp RenderData.cpp Renderer.cpp geometry.cpp ShaderProgram.cpp TextureManager.cpp Particles.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=../Shader/Fireworks

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(OBJECTS) $(CFLAGS) $(LDFLAGS) -o $@

.cpp.o:
	$(CC) -c $(CFLAGS) $< -o $@

clean:
	rm -rf *o $(EXECUTABLE)
