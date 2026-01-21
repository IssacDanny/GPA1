# Whack-a-Zombie

**Course:** Game Programming (Assignment 1)  
**Language:** C++ (C++17 Standard)  
**Engine:** SFML 2.6.1 (Vendored)  
**Architecture:** Modular Monolith (SOLID Principles)

## 📜 Project Overview
Whack-a-Zombie is a reflex-based arcade game where the player must eliminate zombies spawning from designated graves before they retreat. The project demonstrates the use of a custom Game Loop, Entity Management, and Resource Management without relying on a heavy game engine like Unity or Unreal.

## ✨ Features
### Core Requirements
- **Dynamic Spawning:** Zombies appear at 6 distinct locations with randomized timing.
- **Hit Detection:** Pixel-perfect mouse interaction using coordinate mapping (supports window resizing).
- **Scoring System:** Real-time HUD displaying Score, Misses, and Accuracy percentage.
- **Game Loop:** Custom `Update()` and `Render()` cycle using `sf::Clock` for delta time.

### Bonus / Advanced Features
- **🔊 Audio System:** Background music and distinct hit sound effects.
- **🔇 Mute Toggle:** Press **'M'** to toggle audio on/off.
- **⚰️ Visual Context:** Static grave markers indicate spawn locations.
- **📏 Auto-Scaling:** Zombies automatically resize to fit the game grid regardless of the source texture resolution.
- **📉 Passive Miss Detection:** Letting a zombie escape counts as a miss, affecting accuracy.
- **💥 Visual Feedback:** Zombies turn red and "squash" when hit.

## 🏗️ Architecture
The project follows a **Modular Monolith** design to ensure maintainability:
- **GameEngine:** The central coordinator (God Object) that manages the window and main loop.
- **ZombieManager:** Handles the entity pool, spawning logic, and collision delegation.
- **ResourceManager:** A centralized asset loader (Singleton pattern style) to prevent duplicate memory allocations.
- **StandardZombie:** Encapsulates the Finite State Machine (Hidden -> Spawning -> Active -> Hit).
- **Interfaces:** Uses `IWhackable`, `ITelemetry`, and `IAssetSupplier` to enforce Dependency Inversion (DIP).

## 🚀 How to Build and Run
This project is **self-contained**. SFML is included in the `vendor` folder, so no global installation is required.

### Prerequisites
- **CMake** (Version 3.10 or higher)
- **C++ Compiler** (MSVC, MinGW, or Clang)

### Build Steps (Command Line)
1. Open a terminal in the project root directory.
2. Generate the build files:
   ```bash
   cmake -S . -B build
   ```
3. Compile the project:
   ```bash
   cmake --build build
   ```
4. Run the executable:
   ```bash
   .\build\Debug\WhackAZombie.exe
   ```

## 🎮 Controls
| Input | Action |
| :--- | :--- |
| **Mouse Move** | Aim Reticle |
| **Left Click** | Whack Zombie |
| **'M' Key** | Toggle Mute (Music & SFX) |
| **Esc / Close** | Quit Game |

## 📂 Directory Structure
```text
WhackAZombie/
├── assets/          # Textures, Sounds, and Fonts
├── include/         # Header files (.h)
├── src/             # Source code (.cpp)
├── vendor/          # SFML Library files
├── CMakeLists.txt   # Build configuration
└── README.md        # Documentation
```

## 🎨 Credits
- **Code:** Đinh Gia Kiệt
- **Engine:** SFML (Simple and Fast Multimedia Library)
- **Assets:** [Zombie](https://www.pinterest.com/pin/590323463712954308/), [Background](https://www.pinterest.com/pin/590323463712954264/), [Tombstone](https://www.pinterest.com/pin/590323463712955776/), [Hit sound](https://pixabay.com/sound-effects/film-special-effects-slap-90128/), [Background music](https://freetouse.com/music/walen/dark-heart)
