CXX = g++
CXXFLAGS = -std=c++17

SDL_CFLAGS := $(shell sdl2-config --cflags)
SDL_LDFLAGS := $(shell sdl2-config --libs) -lSDL2_image

SRC = src/main.cpp src/Game.cpp src/Player.cpp src/Enemy.cpp src/EntityManager.cpp src/Animation.cpp src/AnimationManager.cpp src/AnimationController.cpp src/PhysicsBody.cpp src/InputManager.cpp \
	src/resourceManager.cpp src/SpriteRenderer.cpp
OUT = game


all:
	$(CXX) $(CXXFLAGS) $(SDL_CFLAGS) $(SRC) -o $(OUT) $(SDL_LDFLAGS)
	./game

clean:
	rm -f $(OUT)
