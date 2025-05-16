#ifndef PLAYER_H
#define PLAYER_H

#include <SDL2/SDL.h>

class Player {
public:
    Player(int x, int y, int width, int height, SDL_Texture* texture);
    ~Player();
    
    void handleEvent(SDL_Event& event);
    void update(int groundLevel);
    void render(SDL_Renderer* renderer, const SDL_Rect& destRect);    
    // Movement controls
    void accelerate();
    void brake();
    void coast();
    
    // Getters
    int getXPosition() const { return rect.x; }
    void setXPosition(int x) { rect.x = x; }
    const SDL_Rect& getRect() const { return rect; }
    SDL_Texture* getTexture() const { return texture; }

private:
    SDL_Rect rect;
    SDL_Texture* texture;
    
    // Physics variables
    float xVelocity;
    float yVelocity;
    float acceleration;
    float deceleration;
    float maxSpeed;
    float gravity;
    bool isJumping;

    static const int SPRITE_FRAMES = 4;
    static const int SPRITE_WIDTH = 1024;  
    static const int SPRITE_HEIGHT = 1024; // Full height of the sprite
    int currentFrame;
    int frameCounter;
    int frameDelay;
    SDL_Rect spriteClips[SPRITE_FRAMES];
    bool facingRight;
};

#endif