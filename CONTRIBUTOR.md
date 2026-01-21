# Contributing to Whack-a-Zombie

Welcome.

This document outlines the architectural standards and workflows required to maintain the structural integrity of the project.

## 🛠️ Development Setup
1.  **Clone the Repository:**
    ```bash
    git clone <REPO_URL>
    ```
2.  **Build System:**
    We use **CMake**. Please do not commit manual Visual Studio solutions (`.sln`) or VS Code configurations (`.vscode`).
    ```bash
    cmake -S . -B build
    cmake --build build
    ```
3.  **Assets:**
    Ensure you have the required assets in `assets/textures/` and `assets/sounds/`. The build system copies these automatically, but you must verify they exist locally.

## 📐 Architectural Standards
This project follows a **Modular Monolith** architecture using **SOLID** principles.

### 1. The design Philosophy
*   **Single Responsibility:** Classes should do one thing. `ZombieManager` manages logic; `ResourceManager` manages memory. Do not mix them.
*   **Interfaces:** We rely heavily on interfaces (`IWhackable`, `ITelemetry`). If you add a new entity, ensure it implements the relevant contract.
*   **Dependency Injection:** Do not create global variables. Pass dependencies (like `IAssetSupplier*`) via constructors.

### 2. Naming Conventions
*   **Classes:** `PascalCase` (e.g., `GameEngine`, `StandardZombie`)
*   **Member Variables:** `m_camelCase` (e.g., `m_score`, `m_spawnTimer`)
*   **Methods:** `PascalCase` (e.g., `Update()`, `Render()`)
*   **Local Variables:** `camelCase` (e.g., `deltaTime`, `zombieIndex`)

### 3. File Structure
*   **Headers (`.h`):** Place in `include/`. Use `#pragma once`.
*   **Source (`.cpp`):** Place in `src/`.

## 🚀 Optimization Targets
If you are looking to improve performance, here are the priority areas:

### A. Texture Atlasing (High Priority)
**Current State:** We load individual textures (`zombie.png`, `grave.png`).
**Goal:** Combine these into a single `spritesheet.png`.
**Task:**
1.  Update `ResourceManager` to load one large texture.
2.  Update `StandardZombie` to accept a `sf::IntRect` (texture rectangle) to draw only a portion of the sheet.
**Benefit:** Reduces GPU texture switching, improving rendering speed.

### B. Sprite Batching (Medium Priority)
**Current State:** We call `window.draw()` for every single zombie and grave.
**Goal:** Use `sf::VertexArray` to draw all graves in a single draw call.
**Task:** Refactor `ZombieManager::Render` to populate a vertex array instead of iterating sprites.
**Benefit:** Drastically reduces CPU-to-GPU draw call overhead.

### C. Memory Management
**Current State:** We use `std::vector<std::unique_ptr<IWhackable>>`.
**Goal:** While safe, pointer indirection can cause cache misses.
**Task:** Investigate using a contiguous `std::vector<StandardZombie>` if polymorphism is not strictly required, or implement a custom memory allocator.

## 🤝 Workflow
1.  **Branching:** Create a new branch for your task.
    *   `feature/new-zombie-type`
    *   `optimize/texture-atlas`
    *   `fix/hitbox-bug`
2.  **Commits:** Write clear, imperative commit messages (e.g., "Add sound effect for missing a shot").
3.  **Pull Requests:** Submit a PR for review. Ensure the game builds and runs without warnings.
