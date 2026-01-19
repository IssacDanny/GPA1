#include "../include/StandardZombie.h"
#include <iostream>

// Configuration Constants (Tweaking these changes the "Game Feel")
const float SPAWN_ANIMATION_TIME = 0.2f; // Seconds to pop up
const float HIT_ANIMATION_TIME = 0.2f;   // Seconds to show "hit" frame
const float DEFAULT_LIFETIME = 1.5f;     // How long it stays up (1500ms)
const float TARGET_SIZE = 100.0f;

StandardZombie::StandardZombie(const sf::Texture& texture, sf::Vector2f position)
    : m_basePosition(position), m_state(ZombieState::Hidden), m_stateTimer(0.0f), m_activeDuration(DEFAULT_LIFETIME)
{
    // Setup the sprite
    m_sprite.setTexture(texture);
    
    // Center the origin so it scales from the middle, not the top-left
    sf::FloatRect bounds = m_sprite.getLocalBounds();
    m_sprite.setOrigin(bounds.width / 2.0f, bounds.height / 2.0f);
    
    // Calculate how much we need to shrink/grow the image to fit 100px
    float scaleX = TARGET_SIZE / bounds.width;
    float scaleY = TARGET_SIZE / bounds.height;
    m_maxScale = std::min(scaleX, scaleY); // Keep aspect ratio

    m_sprite.setPosition(position);
    m_sprite.setScale(0.0f, 0.0f); // Start invisible (scaled down)
}

void StandardZombie::Spawn() {
    if (m_state == ZombieState::Hidden) {
        m_state = ZombieState::Spawning;
        m_stateTimer = 0.0f;
        m_sprite.setColor(sf::Color::White); // Reset color
        m_sprite.setScale(0.0f, 0.0f);       // Reset size
    }
}

void StandardZombie::Reset() {
    m_state = ZombieState::Hidden;
    m_sprite.setScale(0.0f, 0.0f);
}

bool StandardZombie::Update(float deltaTime) {
    m_stateTimer += deltaTime;

    switch (m_state) {
        case ZombieState::Spawning:
            {
                float progress = m_stateTimer / SPAWN_ANIMATION_TIME;
                
                if (progress >= 1.0f) {
                    // Animation Complete: Set to the CALCULATED max scale
                    m_sprite.setScale(m_maxScale, m_maxScale); 
                    m_state = ZombieState::Active;
                    m_stateTimer = 0.0f;
                } else {
                    // Animate from 0 to m_maxScale
                    float currentScale = progress * m_maxScale;
                    m_sprite.setScale(currentScale, currentScale);
                }
            }
            return false;

        case ZombieState::Active:
            // Wait for player to hit. If time runs out, hide.
            if (m_stateTimer >= m_activeDuration) {
                Reset(); // Missed! Back to grave.
                return true;
            }
            return false;

        case ZombieState::Hit:
            // Show "Hit" feedback (e.g., shrink or fade)
            if (m_stateTimer >= HIT_ANIMATION_TIME) {
                Reset(); // Animation done, hide.
            }
            return false;

        default:
            return false;
    }
}

void StandardZombie::Render(sf::RenderWindow& window) {
    if (m_state != ZombieState::Hidden) {
        window.draw(m_sprite);
    }
}

bool StandardZombie::TryHit(int x, int y) {
    // Only allow hits if the zombie is fully active
    if (m_state != ZombieState::Active) return false;

    // SFML's built-in hitbox check
    // We map the mouse coordinates to the sprite's world coordinates
    if (m_sprite.getGlobalBounds().contains(static_cast<float>(x), static_cast<float>(y))) {
        // SUCCESS!
        m_state = ZombieState::Hit;
        m_stateTimer = 0.0f;
        
        // Visual Feedback: Turn Red
        m_sprite.setColor(sf::Color::Red);
        
        // Visual Feedback: Squash slightly
        m_sprite.setScale(m_maxScale * 1.2f, m_maxScale * 0.8f);
        
        return true;
    }

    return false;
}

bool StandardZombie::IsActive() const {
    return m_state != ZombieState::Hidden;
}

sf::Vector2f StandardZombie::GetPosition() const {
    return m_basePosition;
}