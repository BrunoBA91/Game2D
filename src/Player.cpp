#include "Player.h"
#include "Collision.h"
#include "InputManager.h"
#include "AnimationManager.h"
#include "ResourceManager.h"

Player::Player(ResourceManager& resMgr, InputManager& inputMgr, AnimationManager& animMgr)
    : resourceManager(resMgr), inputManager(inputMgr), animationManager(animMgr)
{
    animationController.add("idle", animMgr.get("idle"));
    animationController.add("run", animMgr.get("run"));
    animationController.add("jump", animMgr.get("jump"));
    animationController.play("idle");

    physicsBody.attachTransform(&transform);
}

Player::~Player() {
    clean();
}

bool Player::init(SDL_Renderer* renderer, const std::string& imagePath, int x, int y, int w, int h) {
    resourceManager.loadTexture("player", imagePath, renderer);
    texture = resourceManager.getTexture("player");

    if (!texture) {
        SDL_Log("Failed to load texture from ResourceManager");
        return false;
    }

    physicsBody.init(static_cast<float>(x), static_cast<float>(y), w, h);
    physicsBody.setMaxFallSpeed(maxFallSpeed);
    return true;
}

void Player::update(const std::vector<SDL_Rect>& walls,
                    const std::vector<Entity*>& others,
                    float deltaTime) {
    handleInput();

    // Apply gravity
    physicsBody.applyGravity(gravity, deltaTime);

    // Apply movement with collision
    physicsBody.moveWithCollision(walls, physicsBody.getVelocity() * deltaTime);
    
    // Update grounded state
    checkIfStanding(walls);

    // Coyote Time
    if (isOnGround) {
        coyoteTimer = coyoteTime;  // reset when grounded
    } else {
        coyoteTimer -= deltaTime;  // count down
    }

    // Jump buffering
    if (jumpBufferTimer > 0.0f) {
        jumpBufferTimer -= deltaTime;

        if (coyoteTimer > 0.0f) {
            Vector2f velocity = physicsBody.getVelocity();
            velocity.y = jumpStrength;
            physicsBody.setVelocity(velocity.x, velocity.y);

            isOnGround = false;
            coyoteTimer = 0.0f;
            jumpBufferTimer = 0.0f;
            wantsToJump = false;
        }
    }

    const auto& vel = physicsBody.getVelocity();

    // Flip based on horizontal direction if moving
    if (vel.x > 0.0f) {
        setFlip(SDL_FLIP_NONE);
    } else if (vel.x < 0.0f) {
        setFlip(SDL_FLIP_HORIZONTAL);
    }

    // Animation Logic
    if (!isOnGround) {
        animationController.play("jump");
    } else if (vel.x != 0.0f) {
        animationController.play("run");
    } else {
        animationController.play("idle");
    }

    animationController.update();
}

void Player::render(SDL_Renderer* renderer) {
    SDL_Rect srcRect = animationController.getCurrentFrameRect();

    SDL_Rect dstRect;
    dstRect.x = static_cast<int>(transform.position.x);
    dstRect.y = static_cast<int>(transform.position.y);
    dstRect.w = srcRect.w;
    dstRect.h = srcRect.h;

    SDL_RenderCopyEx(renderer, texture, &srcRect, &dstRect, 0.0, &origin, getFlip());
}

void Player::handleInput() {
    Vector2f velocity = physicsBody.getVelocity();
    velocity.x = 0;

    if (inputManager.isKeyDown(SDL_SCANCODE_LEFT) || inputManager.isGamepadButtonDown(SDL_CONTROLLER_BUTTON_DPAD_LEFT)) {
        velocity.x = -moveSpeed;
    } else if (inputManager.isKeyDown(SDL_SCANCODE_RIGHT) || inputManager.isGamepadButtonDown(SDL_CONTROLLER_BUTTON_DPAD_RIGHT)) {
        velocity.x = moveSpeed;
    }

    if (inputManager.isKeyDown(SDL_SCANCODE_SPACE) || inputManager.isGamepadButtonDown(SDL_CONTROLLER_BUTTON_A)) {
        wantsToJump = true;
        jumpBufferTimer = jumpBufferTime;
    }    

    physicsBody.setVelocity(velocity.x, velocity.y);
}

void Player::checkIfStanding(const std::vector<SDL_Rect>& walls) {
    SDL_Rect feet = physicsBody.getRect();
    feet.y += 1;
    isOnGround = false;

    for (const SDL_Rect& wall : walls) {
        if (Collision::checkAABBCollision(feet, wall)) {
            isOnGround = true;
            break;
        }
    }
}

void Player::clean() {
    if (texture) {
        SDL_DestroyTexture(texture);
        texture = nullptr;
    }
}
