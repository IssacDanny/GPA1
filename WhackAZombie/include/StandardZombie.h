#pragma once
#include "Interface.h"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

// The Lifecycle of a Zombie
enum class ZombieState {
    Hidden,     // Waiting in the grave
    Spawning,   // Popping up (Animation)
    Active,     // Fully visible, waiting to be hit
    Hit,        // Struck by player (Death Animation)
    Despawning  // Time ran out, retreating (Optional)
};

class StandardZombie : public IWhackable {
public:
    // Constructor: Requires the texture (skin) and the fixed position on screen.
    StandardZombie(const sf::Texture& texture, sf::Vector2f position);
    ~StandardZombie() = default;

    // --- IWhackable Contract Implementation ---
    bool Update(float deltaTime) override;
    void Render(sf::RenderWindow& window) override;
    bool TryHit(int x, int y) override;
    bool IsActive() const override;
    sf::Vector2f GetPosition() const override;

    // --- Command Methods ---
    // Called by the Manager to wake the zombie up
    void Spawn();
    
    // Called to force the zombie back to the grave (reset)
    void Reset();

private:
    sf::Sprite m_sprite;
    ZombieState m_state;
    float m_maxScale;
    
    // Timers
    float m_stateTimer;      // Tracks how long we've been in the current state
    float m_activeDuration;  // How long the zombie stays up (Difficulty)

    // Visual Settings
    sf::Vector2f m_basePosition;
    
    // Internal Helpers
    void UpdateAnimation(float deltaTime);
};