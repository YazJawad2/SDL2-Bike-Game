#include "Player.h"

Player::Player(int x, int y, int width, int height, SDL_Texture* texture) 
    : texture(texture), xVelocity(0), yVelocity(0), acceleration(8.0f),
      deceleration(8.0f), maxSpeed(8.0f), gravity(0.5f), isJumping(false),
      currentFrame(0), frameCounter(0), frameDelay(10), facingRight(true) {
    rect.x = x;
    rect.y = y;
    rect.w = width;
    rect.h = height;
    
    // Set up sprite clips for animation
    for (int i = 0; i < SPRITE_FRAMES; i++) {
        spriteClips[i].x = i * SPRITE_WIDTH;
        spriteClips[i].y = 0;
        spriteClips[i].w = SPRITE_WIDTH;
        spriteClips[i].h = SPRITE_HEIGHT;
    }
}

Player::~Player() {
    // Texture is managed by Game class, no cleanup needed here
}

void Player::accelerate() {
    while(xVelocity <= maxSpeed){
        xVelocity+= acceleration;
    }
    
}

void Player::brake() {
    xVelocity -= acceleration * 2.5f; // Brake harder than acceleration
    if (xVelocity < -maxSpeed/2) xVelocity = -maxSpeed; // Reverse slower
}


void Player::handleEvent(SDL_Event& event) {
    const Uint8* keystate = SDL_GetKeyboardState(NULL);
    
    if (keystate[SDL_SCANCODE_RIGHT] || keystate[SDL_SCANCODE_D]) {
        accelerate();
        facingRight = true;
    } 
    else if (keystate[SDL_SCANCODE_LEFT] || keystate[SDL_SCANCODE_A]) {
        brake();
        facingRight = false;
    }

    if (event.type == SDL_KEYDOWN && event.key.repeat == 0) {
        switch (event.key.keysym.sym) {
            case SDLK_SPACE:
                xVelocity = 0;
                break;
            case (SDLK_UP):
                if (!isJumping) {
                    yVelocity = -20.0f;
                    isJumping = true;
                    currentFrame = 3; // Set to jumping frame immediately
                }
                break;
        }
    }
}

void Player::update(int groundLevel) {
    // Apply horizontal movement
    rect.x += static_cast<int>(xVelocity);
    
    // Apply gravity
    yVelocity += gravity;
    rect.y += static_cast<int>(yVelocity);
    
    // Ground collision
    if (rect.y + rect.h > groundLevel) {
        rect.y = groundLevel - rect.h;
        yVelocity = 0;
        isJumping = false;
    }
    
    // Screen boundaries (optional)
    
    // Handle animation
    // Update facing direction based on movement
    if (xVelocity > 0.1f) {
        facingRight = true;
    } else if (xVelocity < -0.1f) {
        facingRight = false;
    }
    
    // Update animation frame
    frameCounter++;
    if (frameCounter >= frameDelay) {
        frameCounter = 0;
        
        // Use the jumping frame when jumping
        if (isJumping) {
            currentFrame = 3; // Use the 4th frame (index 3) for jumping
        } else if (std::abs(xVelocity) > 0.1f) {
            // Cycle through the first 3 frames for movement
            currentFrame = (currentFrame + 1) % 3;
        } else {
            // Use the first frame when idle
            currentFrame = 0;
        }
    }
}

void Player::render(SDL_Renderer* renderer, const SDL_Rect& destRect) {
    if (texture) {
        SDL_RendererFlip flip = facingRight ? SDL_FLIP_NONE : SDL_FLIP_HORIZONTAL;
        SDL_RenderCopyEx(renderer, texture, &spriteClips[currentFrame], &destRect, 0.0, NULL, flip);
    } else {
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderFillRect(renderer, &destRect);
    }
}