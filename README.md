markdown
# 🎮 2D Platformer Game | C++ & SDL2

![Game Screenshot](/res/images/screenshot.png) 

A simple 2D platformer game featuring player movement, obstacle generation, and parallax backgrounds.

## ✨ Features
- 🏃‍♂️ Player movement (acceleration, braking, jumping)
- 🚧 Procedural obstacle generation
- 🌄 Parallax background system
- 🎵 Background music & sound effects
- ⏱ 60-second timer with win condition
- 🖥 Modern CMake build system

## 🛠 Requirements
- CMake ≥ 3.10
- SDL2 libraries:
  - SDL2_image
  - SDL2_ttf  
  - SDL2_mixer
- C++17 compiler

## 🚀 Quick Start

#bash
# Clone & build
git clone https://github.com/YazJawad2/SDL2-Bike-Game.git
cd SDL2-Platformer
mkdir build && cd build
cmake .. && make

# Run the game
./game
📦 Dependency Installation
Ubuntu/Debian
bash
sudo apt install -y libsdl2-dev libsdl2-image-dev libsdl2-ttf-dev libsdl2-mixer-dev cmake
macOS (Homebrew)
bash
brew install sdl2 sdl2_image sdl2_ttf sdl2_mixer cmake
Windows (vcpkg)
powershell
vcpkg install sdl2 sdl2-image sdl2-ttf sdl2-mixer
📂 Project Structure
.
├── CMakeLists.txt          # Build configuration
├── src/                    # Source code
│   ├── Game.cpp            # Main game loop
│   ├── Player.cpp          # Player physics
│   └── Obstacle.cpp        # Obstacle logic
├── include/                # Headers
└── res/                    # Assets
    ├── images/             # Textures
    ├── musique/            # Audio  
    └── font/               # Fonts
🛠 Build Options
Variable	Description	Default
SDL2_DIR	Custom SDL2 path	System
BUILD_TESTING	Enable tests	OFF
Example:

bash
cmake -DSDL2_DIR=/custom/path/to/sdl2 ..
📜 License
MIT License - See LICENSE for details.

Note: Place all game assets in the res/ folder relative to the executable!
