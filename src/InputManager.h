#pragma once
#include <SDL2/SDL.h>
#include <unordered_map>

class InputManager {
public:
    InputManager();
    ~InputManager();

    void update(SDL_Window* window, int preferredWidth, int preferredHeight); // Call this every frame
    bool isKeyDown(SDL_Scancode key) const;
    bool isKeyPressed(SDL_Scancode key) const;
    bool isKeyReleased(SDL_Scancode key) const;

    bool shouldQuit() const;

    void initGamepad(); // Call this once on startup
    void closeGamepad(); // Call this on shutdown

    bool isGamepadButtonDown(SDL_GameControllerButton button) const;
    bool isGamepadConnected() const;

private:

    const Uint8* currentKeyStates = nullptr;
    std::unordered_map<SDL_Scancode, bool> previousKeyStates;

    SDL_GameController* gamepad = nullptr;
    bool quitRequested = false;
};
