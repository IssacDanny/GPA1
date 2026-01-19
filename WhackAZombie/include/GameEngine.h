#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>
#include "Interface.h"

// Forward declarations to avoid circular dependencies
class ResourceManager;
class ZombieManager;
class HUD;

class GameEngine : public ITelemetry {
public:
    GameEngine();
    ~GameEngine();

    // The Master Loop
    void Run();

    // ITelemetry Implementation
    int GetScore() const override { return m_score; }
    int GetMisses() const override { return m_misses; }
    float GetAccuracy() const override;

    // Methods to modify state (called by ZombieManager)
    void RegisterHit();
    void RegisterMiss();

private:
    // Core Systems
    sf::RenderWindow m_window;
    sf::Sprite m_backgroundSprite;
    bool m_isRunning;

    // Game State
    int m_score;
    int m_misses;

    // Audio Systems
    sf::Music m_backgroundMusic; // Streams from disk
    sf::Sound m_hitSound;        // Plays from memory buffer
    
    // Mute Toggle State (For the bonus points)
    bool m_isMuted;

    // The Specialists (Modules)
    // We use unique_ptr for automatic memory management (RAII)
    std::unique_ptr<ResourceManager> m_resourceManager;
    std::unique_ptr<ZombieManager> m_zombieManager;
    std::unique_ptr<HUD> m_hud;

    // Internal Helpers
    void ProcessEvents();
    void Update(float deltaTime);
    void Render();
};