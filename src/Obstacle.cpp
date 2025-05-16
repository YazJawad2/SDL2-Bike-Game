#include "Obstacle.h"

Obstacle::Obstacle(int x, int y, int width, int height, SDL_Texture* texture) 
    : texture(texture) { // Removed xVelocity
    rect.x = x;
    rect.y = y;
    rect.w = width;
    rect.h = height;
}

Obstacle::~Obstacle() {
    // Note: We don't destroy the texture here as it's managed by the Game class
}

void Obstacle::update() {
    }

void Obstacle::render(SDL_Renderer* renderer) {
    // Render the obstacle texture
    if (texture != nullptr) {
        SDL_RenderCopy(renderer, texture, NULL, &rect);
    } 
}

bool Obstacle::checkCollision(const SDL_Rect& playerRect) {
    // Add padding or custom logic
    SDL_Rect adjustedRect = {
        rect.x + 10,  // Shrink hitbox horizontally
        rect.y + 5,   // Adjust vertically
        rect.w - 20,
        rect.h - 10
    };
    return SDL_HasIntersection(&adjustedRect, &playerRect);
}