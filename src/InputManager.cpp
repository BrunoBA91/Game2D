#include "InputManager.h"
#include <iostream>

InputManager::InputManager() {
    
}

InputManager::~InputManager() {
    closeGamepad();
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

void InputManager::update(SDL_Window* window, int preferredWidth, int preferredHeight) {
    quitRequested = false;
    
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            quitRequested = true;
        } else if (event.type == SDL_KEYDOWN) {
            if (event.key.keysym.sym == SDLK_ESCAPE) {
                quitRequested = true;
            } else if (event.key.keysym.sym == SDLK_f) {
                // Toggle fullscreen
                Uint32 currentFlags = SDL_GetWindowFlags(window);
                if (currentFlags & SDL_WINDOW_FULLSCREEN_DESKTOP) {
                    SDL_SetWindowFullscreen(window, 0);  // Go windowed         
                    SDL_SetWindowSize(window, preferredWidth, preferredHeight);
                    SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
                } else {
                    SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP); // Go fullscreen
                }
            }
        }
    }

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
    bool wasDown = previousKeyStates.count(key) ? previousKeyStates.at(key) : false;
    return currentKeyStates[key] && !wasDown;
}

bool InputManager::isKeyReleased(SDL_Scancode key) const {
    bool wasDown = previousKeyStates.count(key) ? previousKeyStates.at(key) : false;
    return !currentKeyStates[key] && wasDown;
}

bool InputManager::shouldQuit() const {
    return quitRequested;
}