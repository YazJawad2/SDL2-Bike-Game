#include "Game.h"
#include <iostream>
#include <ctime>


Game::Game() : 
    // Initialize in same order as declared in header
    window(nullptr), 
    renderer(nullptr), 
    running(false),
    gameState(MENU),
    player(nullptr), 
    groundTexture(nullptr), 
    menuBackgroundTexture(nullptr),
    playButtonTexture(nullptr), 
    quitButtonTexture(nullptr), 
    gameOverTexture(nullptr),
    aboutButtonTexture(nullptr),
    gameOverBackground(nullptr),
    font(nullptr),
    instructionsTexture(nullptr) {
    
    playButton = {550, 250, 200, 80};
    aboutButton = {550, 350, 200, 80};  // Positioned between play and quit
    quitButton = {550, 450, 200, 80}; 
}

Game::~Game() {
    stopMusic();  // Add this to ensure music stops before freeing
    
    if (backgroundMusic != nullptr) {
        Mix_FreeMusic(backgroundMusic);
        backgroundMusic = nullptr;
    }

    if (gameOverMusic != nullptr) {
        Mix_FreeMusic(gameOverMusic);
        gameOverMusic = nullptr;
    }
    
    if (menuMusic != nullptr) {
        Mix_FreeMusic(menuMusic);
        menuMusic = nullptr;
    }

    if (winMusic != nullptr) {  // Add this block
        Mix_FreeMusic(winMusic);
        winMusic = nullptr;
    }

    Mix_CloseAudio();
    cleanup();
}

bool Game::initialize() {

    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {  // Must come first
        std::cerr << "SDL_Init failed: " << SDL_GetError() << std::endl;
        return false;
    }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        std::cerr << "SDL_mixer could not initialize! Mix_Error: " << Mix_GetError() << std::endl;
        return false;
    }

    if (TTF_Init() == -1) {  // Must come after SDL_Init
        std::cerr << "TTF_Init failed: " << TTF_GetError() << std::endl;
        return false;
    }
    
    // Initialize SDL_image
    int imgFlags = IMG_INIT_PNG;
    if (!(IMG_Init(imgFlags) & imgFlags)) {
        std::cout << "SDL_image could not initialize! SDL_image Error: " << IMG_GetError() << std::endl;
        return false;
    }
    
    // Create window
    window = SDL_CreateWindow("Simple Platformer", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 
                              SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == nullptr) {
        std::cout << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }
    
    // Create renderer
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == nullptr) {
        std::cout << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }
    
    // Set rendering color
    
    // Initialize random seed
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    
    // Load media
    if (!loadMedia()) {
        std::cout << "Failed to load media!" << std::endl;
        return false;
    }
        
    return true;
}

bool Game::loadMedia() {
    // Load background texture
    backgroundLayers.push_back({
    loadTexture("res/images/back.png"),
    0,      // scrollX
    0.3f,   // speed
    0,      // yOffset (initialize to 0 if not used)
    SCREEN_HEIGHT  // height (set to screen height or your desired value)
    });

    backgroundLayers.push_back({
        loadTexture("res/images/middle.png"),
        0,      // scrollX
        0.6f,   // speed
        0,      // yOffset
        SCREEN_HEIGHT
    });

    backgroundLayers.push_back({
        loadTexture("res/images/fore-originale.png"),
        0,      // scrollX
        1.0f,   // speed
        5000,   // yOffset
        1000    // height
    });
        
    // Load menu background texture
    menuBackgroundTexture = loadTexture("res/images/MunuBackground.png");
    if (menuBackgroundTexture == nullptr) {
        std::cout << "Failed to load menu background texture!" << std::endl;
        return false;
    }
    
    if (!loadMusic("res/musique/GamePlay.mp3")) {
        std::cerr << "Failed to load background music!" << std::endl;
        return false;
    }

    menuMusic = Mix_LoadMUS("res/musique/MenuMusique.mp3"); // Change path to your menu music file
    if (menuMusic == nullptr) {
        std::cerr << "Failed to load menu music! Mix_Error: " << Mix_GetError() << std::endl;
        return false;
    }

    gameOverMusic = Mix_LoadMUS("res/musique/Game_Over.mp3"); // Chemin vers votre fichier
    if (gameOverMusic == nullptr) {
        std::cerr << "Failed to load game over music! Mix_Error: " << Mix_GetError() << std::endl;
        return false;
    }

     winMusic = Mix_LoadMUS("res/musique/YouWin.mp3"); // Change path to your win music file
    if (winMusic == nullptr) {
        std::cerr << "Failed to load win music! Mix_Error: " << Mix_GetError() << std::endl;
        return false;
    }

    
    // Load play button texture
    playButtonTexture = loadTexture("res/images/Start-Button.png");
    if (playButtonTexture == nullptr) {
        std::cout << "Failed to load play button texture!" << std::endl;
        return false;
    }
    
    // Load quit button texture
    quitButtonTexture = loadTexture("res/images/EXIT-BUTTON.png");
    if (quitButtonTexture == nullptr) {
        std::cout << "Failed to load quit button texture!" << std::endl;
        return false;
    }
    aboutButtonTexture = loadTexture("res/images/About-Button.png");
    if (aboutButtonTexture == nullptr) {
        std::cout << "Failed to load about button texture!" << std::endl;
        return false;
    }
    
    // Load game over texture
    gameOverTexture = loadTexture("res/images/Game-Over.png");
    if (gameOverTexture == nullptr) {
        std::cout << "Failed to load game over texture!" << std::endl;
        return false;
    }

    winTexture = loadTexture("res/images/win.png"); 
    if (winTexture == nullptr) {
    std::cout << "Failed to load win texture!" << std::endl;
    return false;
    }

    gameOverBackground = loadTexture("res/images/Gameover-Background.jpg");
    if (gameOverBackground == nullptr){
        std::cout << "Failed to load play button texture !" << std::endl;
        return false;
    }
    logo = loadTexture("res/images/logo.png");
    if (logo == nullptr){
        std::cout << "Failed to load play button texture !" << std::endl;
        return false;
    }

    //font

    font = TTF_OpenFont("res/font/Cyber.ttf", 20); // Adjust path and size as needed

    if (font == nullptr) {
        std::cout << "Failed to load font! SDL_ttf Error: " << TTF_GetError() << std::endl;
        return false;
    }
        std::string instructionsText = "COMMENT JOUER :\n\n"
                "1. Utilisez FLECHE DROITE pour accelerer\n"
                "2. Utilisez FLECHE GAUCHE pour freiner\n"
                "3. Appuyez sur ESPACE pour un arret d'urgence\n"
                "4. Appuyez sur FLECHE HAUT pour sauter\n\n"

                "OBJECTIF :\n\n"
                "- Survivez pendant 1 minute pour gagner la partie\n"
                "- Evitez les obstacles qui apparaissent sur votre chemin\n"
                "- Le jeu devient plus difficile au fur et a mesure que vous avancez\n";
    
    SDL_Color textColor = {255, 255, 255, 255}; // Black text
    SDL_Surface* textSurface = TTF_RenderText_Blended_Wrapped(font, instructionsText.c_str(), textColor, 500);
    if (textSurface == nullptr) {
        std::cout << "Unable to render text surface! SDL_ttf Error: " << TTF_GetError() << std::endl;
        return false;
    }
    
    instructionsTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    if (instructionsTexture == nullptr) {
        std::cout << "Unable to create texture from rendered text! SDL Error: " << SDL_GetError() << std::endl;
    }
    
    SDL_FreeSurface(textSurface);
    
    return true;
}

SDL_Texture* Game::loadTexture(const std::string& path) {
    // Load image at specified path
    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
    if (loadedSurface == nullptr) {
        std::cout << "Unable to load image " << path << "! SDL_image Error: " << IMG_GetError() << std::endl;
        return nullptr;
    }
    
    // Create texture from surface pixels
    SDL_Texture* newTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
    if (newTexture == nullptr) {
        std::cout << "Unable to create texture from " << path << "! SDL Error: " << SDL_GetError() << std::endl;
    }
    
    // Get rid of old loaded surface
    SDL_FreeSurface(loadedSurface);
    
    return newTexture;
}

void Game::run() {
    if (!initialize()) {
        return;
    }
    
    running = true;
    
    // Game loop
    while (running) {
        handleEvents();
        update();
        render();
        

    }
}

void Game::cleanup() {
    // Free textures
    for (auto& layer : backgroundLayers) {
        if (layer.texture != nullptr) {
            SDL_DestroyTexture(layer.texture);
            layer.texture = nullptr;
        }
    }
    
    if (groundTexture != nullptr) {
        SDL_DestroyTexture(groundTexture);
        groundTexture = nullptr;
    }
    
    if (menuBackgroundTexture != nullptr) {
        SDL_DestroyTexture(menuBackgroundTexture);
        menuBackgroundTexture = nullptr;
    }
    
    if (playButtonTexture != nullptr) {
        SDL_DestroyTexture(playButtonTexture);
        playButtonTexture = nullptr;
    }
    
    if (quitButtonTexture != nullptr) {
        SDL_DestroyTexture(quitButtonTexture);
        quitButtonTexture = nullptr;
    }
    if (aboutButtonTexture != nullptr) {
        SDL_DestroyTexture(aboutButtonTexture);
        aboutButtonTexture = nullptr;
    }
    
    if (gameOverTexture != nullptr) {
        SDL_DestroyTexture(gameOverTexture);
        gameOverTexture = nullptr;
    }
    if (winTexture != nullptr) {
    SDL_DestroyTexture(winTexture);
    winTexture = nullptr;
    }
    if (gameOverBackground != nullptr) {
        SDL_DestroyTexture(gameOverBackground);
        gameOverBackground = nullptr;
    }   
    if (logo != nullptr){
        SDL_DestroyTexture(logo);
        logo = nullptr;
    }
    // Destroy renderer
    if (renderer != nullptr) {
        SDL_DestroyRenderer(renderer);
        renderer = nullptr;
    }
    
    // Destroy window
    if (window != nullptr) {
        SDL_DestroyWindow(window);
        window = nullptr;
    }
    if (instructionsTexture != nullptr) {
    SDL_DestroyTexture(instructionsTexture);
    instructionsTexture = nullptr;
    }
    
    if (font != nullptr) {
        TTF_CloseFont(font);
        font = nullptr;
    }
    
    TTF_Quit();

    
    IMG_Quit();
    SDL_Quit();
}

void Game::handleEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            running = false;
        }
        
        switch (gameState) {
            case MENU:
                handleMenuEvents(event);
                break;
            case PLAYING:
                if (player) {
                    player->handleEvent(event);
                }
                break;
            case GAME_OVER:
                handleGameOverEvents(event);
                break;
            case ABOUT:
                handleAboutEvents(event);
                break;
            case WIN:
                handleWinEvents(event);
                break;
        }
    }
}

void Game::update() {
    updateMusicState();  // Add this at the start of update()
    if (player) {
    cameraTargetX = player->getXPosition() - (SCREEN_WIDTH / 6.5);
    cameraX += (cameraTargetX - cameraX) ;
}
    if (gameState == PLAYING && timerStarted) {
        // Update player position (move forward)
        Uint32 currentTime = SDL_GetTicks();
        if (currentTime - gameStartTime >= gameDuration) {
            gameState = WIN;
            stopMusic();
            return;
        }
        if (player) {
            player->update(GROUND_LEVEL);
        }
        
        // Update obstacles (now stationary)
        for (auto& obstacle : obstacles) {
            // No need to update position since obstacles are stationary
            // Just check collision
            if (player && obstacle->checkCollision(player->getRect())) {
                gameState = GAME_OVER;
                break;
            }
        }
        
        // Remove obstacles that are behind the player
        auto it = obstacles.begin();
        while (it != obstacles.end()) {
            if ((*it)->getXPosition() + (*it)->getWidth() < player->getXPosition() - SCREEN_WIDTH/2) {
                it = obstacles.erase(it);
            } else {
                ++it;
            }
        }
        
        // Generate new obstacles ahead of the player
            if (rand() % 100 < 2 && obstacles.size() < 2) {
                int minDistance = SCREEN_WIDTH ;  // Minimum spawn distance (entire screen width)
                int maxDistance = SCREEN_WIDTH + 500;  // Max spawn distance
                int obstacleX = player->getXPosition() + minDistance + rand() % (maxDistance - minDistance);
                
                int carWidth = 330;
                int carHeight = 180;
                obstacles.push_back(std::make_unique<Obstacle>(
                    obstacleX,
                    GROUND_LEVEL - (carHeight - 30),  // Aligns bottom of car with ground
                    carWidth,
                    carHeight,
                    loadTexture("res/images/Barriere.png")
                ));
            }
    }
}

void Game::render() {
    // Clear the screen
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);

    
    // Draw based on game state
    switch (gameState) {
        case MENU:
            renderMenu();
            break;
            
        case PLAYING: {
        
        // Calculate camera offset to keep player centered
        int cameraOffset = player->getXPosition() - (SCREEN_WIDTH / 3); // Keep player 1/3 from left
        
        // Render parallax layers with proper offset
        for (ParallaxLayer& layer : backgroundLayers) {
            // Calculate parallax scrolling effect
            int layerOffset = static_cast<int>(cameraX * layer.speed);            
            // Main texture
            SDL_Rect bgDest = {-layerOffset % SCREEN_WIDTH, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
            SDL_RenderCopy(renderer, layer.texture, NULL, &bgDest);
            
            // Seamless repeating copy
            if (layerOffset % SCREEN_WIDTH != 0) {
                SDL_Rect bgDest2 = {(-layerOffset % SCREEN_WIDTH) + SCREEN_WIDTH, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
                SDL_RenderCopy(renderer, layer.texture, NULL, &bgDest2);
            }
        }
        
        // Render ground with proper offset
        SDL_Rect groundRect = {
            -cameraOffset % SCREEN_WIDTH, 
            GROUND_LEVEL, 
            SCREEN_WIDTH, 
            SCREEN_HEIGHT - GROUND_LEVEL
        };
        if (groundTexture) {
            SDL_RenderCopy(renderer, groundTexture, NULL, &groundRect);
        }
        
        // Draw player at fixed screen position (1/3 from left)
        SDL_Rect playerRect = player->getRect();
        playerRect.x = SCREEN_WIDTH / 3; // Fixed screen position
        playerRect.y = player->getRect().y; // Keep original Y position
        player->render(renderer, playerRect);
        
        // Draw obstacles with proper offset
        for (auto& obstacle : obstacles) {
            SDL_Rect obstacleRect = obstacle->getRect();
            obstacleRect.x -= cameraOffset;
            SDL_RenderCopy(renderer, obstacle->getTexture(), NULL, &obstacleRect);
        }
        renderTimer();
        break;
    }
            
        case GAME_OVER:
            renderGameOver();
            break;

        case ABOUT:
            renderAbout();
            break;
        case WIN:
            renderWin();
            break;
    }
    
    // Update the screen
    SDL_RenderPresent(renderer);
}

void Game::renderMenu() {

    SDL_Rect logoRect = {460, -50, 400, 360};

    // Draw menu background
    SDL_RenderCopy(renderer, menuBackgroundTexture, NULL, NULL);
    SDL_RenderCopy(renderer, logo, NULL, &logoRect);
    
    // Draw buttons
    SDL_RenderCopy(renderer, playButtonTexture, NULL, &playButton);
    SDL_RenderCopy(renderer, aboutButtonTexture, NULL, &aboutButton);
    SDL_RenderCopy(renderer, quitButtonTexture, NULL, &quitButton);
}

void Game::handleMenuEvents(SDL_Event& event) {
    if (event.type == SDL_MOUSEBUTTONDOWN) {
        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);
        
        if (isPointInRect(mouseX, mouseY, playButton)) {
            startNewGame();
        }
        else if (isPointInRect(mouseX, mouseY, aboutButton)) {
            gameState = ABOUT;
        }
        else if (isPointInRect(mouseX, mouseY, quitButton)) {
            running = false;
        }
    }
}

bool Game::loadMusic(const std::string& path) {
    // Free existing music if any
    if (backgroundMusic != nullptr) {
        Mix_FreeMusic(backgroundMusic);
        backgroundMusic = nullptr;
    }

    backgroundMusic = Mix_LoadMUS(path.c_str());
    if (backgroundMusic == nullptr) {
        std::cerr << "Failed to load music! Mix_Error: " << Mix_GetError() << std::endl;
        return false;
    }
    return true;
}

void Game::playMusic(int loops) {
    if (backgroundMusic != nullptr) {
        Mix_PlayMusic(backgroundMusic, loops);
    }
}
void Game::playMusic(Mix_Music* music, int loops) {
    if (music != nullptr) {
        Mix_HaltMusic();  // Stop any currently playing music first
        Mix_PlayMusic(music, loops);
    }
}

void Game::stopMusic() {
    Mix_HaltMusic();
}

void Game::updateMusicState() {
    switch (gameState) {
        case MENU:
            if (!isMenuMusicPlaying) {
                stopMusic();
                playMusic(menuMusic, -1);
                isMenuMusicPlaying = true;
                isGameMusicPlaying = false;
                isGameOverMusicPlaying = false;
                isWinMusicPlaying = false;
            }
            break;
            
        case PLAYING:
            if (!isGameMusicPlaying) {
                stopMusic();
                playMusic(backgroundMusic, -1);
                isGameMusicPlaying = true;
                isMenuMusicPlaying = false;
                isGameOverMusicPlaying = false;
                isWinMusicPlaying = false;
            }
            break;
            
        case GAME_OVER:
            if (!isGameOverMusicPlaying) {
                stopMusic();
                playMusic(gameOverMusic, 0); // Play once
                isGameOverMusicPlaying = true;
                isMenuMusicPlaying = false;
                isGameMusicPlaying = false;
                isWinMusicPlaying = false;
            }
            break;
            
        case WIN:  // Add this case
            if (!isWinMusicPlaying) {
                stopMusic();
                playMusic(winMusic, 0); // Play once
                isWinMusicPlaying = true;
                isMenuMusicPlaying = false;
                isGameMusicPlaying = false;
                isGameOverMusicPlaying = false;
            }
            break;
            
        case ABOUT:
            isMenuMusicPlaying = true;
            isGameMusicPlaying = false;
            isGameOverMusicPlaying = false;
            isWinMusicPlaying = false;
            break;
    }
}
void Game::renderGameOver() {
    // Draw game over background (using the same background as in-game)
    
    // Draw game over text
    SDL_Rect gameOverRect = {450, 0, 400, 300};
    SDL_Rect gameOverBackgroundRect = {0, 0, 1300, 655};

    SDL_RenderCopy(renderer, gameOverBackground, NULL, &gameOverBackgroundRect);

    SDL_RenderCopy(renderer, gameOverTexture, NULL, &gameOverRect);

    SDL_Rect playButton2 = {550, 350, 200, 80};
    // Draw restart button at the same position as the play button
    SDL_RenderCopy(renderer, playButtonTexture, NULL, &playButton2);
    
    // Draw quit button
    SDL_RenderCopy(renderer, quitButtonTexture, NULL, &quitButton);
}

void Game::renderWin() {
    // Draw win background (you can reuse the game over background or create a new one)
    SDL_Rect winBackgroundRect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
    SDL_RenderCopy(renderer, gameOverBackground, NULL, &winBackgroundRect);
    
    // Draw win text
    SDL_Rect winRect = {450, 0, 400, 300};
    SDL_Rect playButton2 = {550, 350, 200, 80};

    SDL_RenderCopy(renderer, winTexture, NULL, &winRect);
    
    // Draw buttons
    SDL_RenderCopy(renderer, playButtonTexture, NULL, &playButton2);
    SDL_RenderCopy(renderer, quitButtonTexture, NULL, &quitButton);
}

void Game::renderAbout() {
    // Draw menu background (same as main menu)
    SDL_RenderCopy(renderer, menuBackgroundTexture, NULL, NULL);

    // Draw instructions text
    if (instructionsTexture != nullptr) {
        SDL_Rect textRect = {220, 120, 860, 410}; // Slightly smaller than background
        SDL_RenderCopy(renderer, instructionsTexture, NULL, &textRect);
    }
    
    // Draw back button (positioned differently than in menu)
    SDL_Rect backButton = {550, 570, 200, 60};
    SDL_RenderCopy(renderer, playButtonTexture, NULL, &backButton);
}

void Game::handleAboutEvents(SDL_Event& event) {
    if (event.type == SDL_MOUSEBUTTONDOWN) {
        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);
        
        // Check if back button was clicked
        SDL_Rect backButton = {500, 570, 200, 60};
        if (isPointInRect(mouseX, mouseY, backButton)) {
            gameState = MENU;
        }
    }
}

void Game::handleWinEvents(SDL_Event& event) {
    if (event.type == SDL_MOUSEBUTTONDOWN) {
        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);
        SDL_Rect playButton2 = {550, 350, 200, 80};

        if (isPointInRect(mouseX, mouseY, playButton2)) {
            startNewGame();
        }
        else if (isPointInRect(mouseX, mouseY, quitButton)) {
            running = false;
        }
    }
}
void Game::handleGameOverEvents(SDL_Event& event) {
    if (event.type == SDL_MOUSEBUTTONDOWN) {
        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);
        SDL_Rect playButton2 = {550, 350, 200, 80};

        
        // Check if restart button was clicked
        if (isPointInRect(mouseX, mouseY, playButton2)) {
            startNewGame();
        }
        
        // Check if quit button was clicked
        if (isPointInRect(mouseX, mouseY, quitButton)) {
            running = false;
        }
    }
}

void Game::renderTimer() {
    if (gameState != PLAYING || !timerStarted) return;
    
    Uint32 elapsed = SDL_GetTicks() - gameStartTime;
    Uint32 remaining = (gameDuration - elapsed) / 1000; // Convert to seconds
    
    std::string timerText = "Time: " + std::to_string(remaining) + "s";
    SDL_Color textColor = {255, 255, 255, 255};
    
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, timerText.c_str(), textColor);
    if (textSurface) {
        SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
        if (textTexture) {
            SDL_Rect textRect = {20, 20, textSurface->w, textSurface->h};
            SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
            SDL_DestroyTexture(textTexture);
        }
        SDL_FreeSurface(textSurface);
    }
}

bool Game::isPointInRect(int x, int y, const SDL_Rect& rect) const {
    return (x >= rect.x && x <= rect.x + rect.w &&
            y >= rect.y && y <= rect.y + rect.h);
}

void Game::startNewGame() {
    // Reset game components
    player = std::make_unique<Player>(200, GROUND_LEVEL - 150, 150, 150, 
        loadTexture("res/images/sprite_sheet.png"));
    obstacles.clear();
    
    // Reset music state
    isMenuMusicPlaying = false;
    isGameMusicPlaying = false;
    isGameOverMusicPlaying = false;
    isWinMusicPlaying = false;  // Add this line
    
    // Reset parallax layers
    for (auto& layer : backgroundLayers) {
        layer.scrollX = 0;
    }
    
    // Reset timer
    gameStartTime = SDL_GetTicks();
    timerStarted = true;
    
    // Change state to playing
    gameState = PLAYING;
    updateMusicState();
}