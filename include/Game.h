#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <vector>
#include <memory>
#include <string>
#include "Player.h"
#include "Obstacle.h"
#include <SDL2/SDL_ttf.h>  
#include <SDL2/SDL_mixer.h>
enum GameState {
    MENU,
    PLAYING,
    GAME_OVER,
    ABOUT,
    WIN
};

class Game {
public:
    Game();
    ~Game();
    
    bool initialize();
    void run();
    void cleanup();

    bool loadMusic(const std::string& path);
    void playMusic(int loops = -1);  // -1 = infinite loop
    void stopMusic();

    void playMusic(Mix_Music* music, int loops = -1);

    
    // Getters
    SDL_Renderer* getRenderer() const { return renderer; }
    int getScreenWidth() const { return SCREEN_WIDTH; }
    int getScreenHeight() const { return SCREEN_HEIGHT; }
    
    // Change game state
    void setGameState(GameState state) { gameState = state; }
    GameState getGameState() const { return gameState; }
    
    // Check if a point is inside a rectangle (for button clicks)
    bool isPointInRect(int x, int y, const SDL_Rect& rect) const;
    
    // Start a new game
    void startNewGame();
    
    // Load texture from file
    SDL_Texture* loadTexture(const std::string& path);
    
    
private:
    // Constants
    const int SCREEN_WIDTH = 1300;
    const int SCREEN_HEIGHT = 650;
    const int GROUND_LEVEL = 610;
    float playerSpeed = 5.0f; 

    Mix_Music* backgroundMusic = nullptr;

    Mix_Music* menuMusic = nullptr;

    Mix_Music* gameOverMusic = nullptr; // Ajoutez cette ligne

    Mix_Music* winMusic = nullptr;  // Add this line


    bool isMenuMusicPlaying = false;
    bool isGameMusicPlaying = false;
    bool isGameOverMusicPlaying = false; // Ajoutez cette ligne
    bool isWinMusicPlaying = false;  // Add this line


    void updateMusicState();

    float cameraX; // Current camera position
    float cameraTargetX; // Where camera wants to be
    float cameraLerpSpeed = 0.1f; // Smoothing factor
    
    // SDL Components
    SDL_Window* window;
    SDL_Renderer* renderer;
    bool running;  // Moved to match initialization order in constructor
    
    // Game components
    GameState gameState;
    std::unique_ptr<Player> player;
    std::vector<std::unique_ptr<Obstacle>> obstacles;
    
    // Textures
    SDL_Texture* groundTexture;
    SDL_Texture* menuBackgroundTexture;
    SDL_Texture* playButtonTexture;
    SDL_Texture* quitButtonTexture;
    SDL_Texture* gameOverTexture;
    SDL_Texture* aboutButtonTexture;
    SDL_Texture* gameOverBackground;
    SDL_Texture* logo;

    TTF_Font* font;
    SDL_Texture* instructionsTexture;

    SDL_Rect aboutButton;
    struct ParallaxLayer {
        SDL_Texture* texture;
        int scrollX;
        float speed; 
        int yOffset;  // New: vertical position adjustment
        int height;   // New: custom height// Lower = slower (distant layers)
    };
    std::vector<ParallaxLayer> backgroundLayers;
    
    // Game loop methods
    void handleEvents();
    void update();
    void render();
    
    // Menu components
    SDL_Rect playButton;
    SDL_Rect quitButton;
    
    // Menu methods
    void renderMenu();
    void handleMenuEvents(SDL_Event& event);

    void handleAboutEvents(SDL_Event& event);
    void renderAbout();
    
    // Game Over methods
    void renderGameOver();
    void handleGameOverEvents(SDL_Event& event);
    
    //win
    void renderWin();
    void handleWinEvents(SDL_Event& event);

    // Generate obstacles
    void generateObstacles();
    
    // Load all game textures
    bool loadMedia();

        // Timer variables
    Uint32 gameStartTime;
    Uint32 gameDuration = 60000; // 1 minute in milliseconds
    bool timerStarted = false;
    SDL_Texture* winTexture;
    void renderTimer();
};

#endif // GAME_H