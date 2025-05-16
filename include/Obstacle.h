#ifndef OBSTACLE_H
#define OBSTACLE_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

class Obstacle {
public:
    Obstacle(int x, int y, int width, int height, SDL_Texture* texture);
    ~Obstacle();
    
    void update();
    void render(SDL_Renderer* renderer);
    bool checkCollision(const SDL_Rect& playerRect);
    
    // Getters
    int getXPosition() const { return rect.x; }
    int getWidth() const { return rect.w; }
    const SDL_Rect& getRect() const { return rect; }
    SDL_Texture* getTexture() const { return texture; } // Make texture accessible
    
private:
    SDL_Rect rect;
    SDL_Texture* texture;
    
    // Movement variables
    
    // Constants
    static constexpr float BASE_SPEED = 5.0f; // Made static constexpr to fix uninitialized warning
};

#endif // OBSTACLE_H