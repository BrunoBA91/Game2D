// InputManager.cpp
#include "InputManager.h"

InputManager& InputManager::getInstance() {
    static InputManager instance;
    return instance;
}

void InputManager::initGamepad() {
    if (SDL_NumJoysticks() > 0 && SDL_IsGameController(0)) {
        gamepad = SDL_GameControllerOpen(0);
        if (!gamepad) {
            SDL_Log("Gamepad not opened: %s", SDL_GetError());
        }
    }
}

void InputManager::closeGamepad() {
    if (gamepad) {
        SDL_GameControllerClose(gamepad);
        gamepad = nullptr;
    }
}

bool InputManager::isGamepadConnected() const {
    return gamepad != nullptr;
}

bool InputManager::isGamepadButtonDown(SDL_GameControllerButton button) const {
    return gamepad && SDL_GameControllerGetButton(gamepad, button);
}

void InputManager::update() {
    SDL_PumpEvents(); // Refresh keyboard state
    currentKeyStates = SDL_GetKeyboardState(nullptr);

    for (int i = 0; i < SDL_NUM_SCANCODES; ++i) {
        SDL_Scancode key = static_cast<SDL_Scancode>(i);
        bool isDownNow = currentKeyStates[key];
        bool wasDownBefore = previousKeyStates[key];
        previousKeyStates[key] = isDownNow;
    }
}

bool InputManager::isKeyDown(SDL_Scancode key) const {
    return currentKeyStates[key];
}

bool InputManager::isKeyPressed(SDL_Scancode key) const {
    return currentKeyStates[key] && !previousKeyStates.at(key);
}

bool InputManager::isKeyReleased(SDL_Scancode key) const {
    return !currentKeyStates[key] && previousKeyStates.at(key);
}

bool InputManager::shouldQuit() const {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT ||
           (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE)) {
            return true;
        }
    }
    return false;
}