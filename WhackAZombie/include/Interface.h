#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <string>

// ----------------------------------------------------------------
// Contract: IWhackable
// Description: Defines any entity that can be spawned, hit, and drawn.
// ----------------------------------------------------------------
class IWhackable {
public:
    virtual ~IWhackable() = default;

    // Called every frame to update logic (timers, animation)
    virtual bool Update(float deltaTime) = 0;
    
    // Called every frame to draw the entity
    virtual void Render(sf::RenderWindow& window) = 0;

    // Checks if a click at (x, y) hits this entity
    // Returns true if hit, false otherwise.
    virtual bool TryHit(int x, int y) = 0;

    // Returns true if the entity is currently active (alive/spawning)
    virtual bool IsActive() const = 0;

    // Returns the current position (useful for debugging or effects)
    virtual sf::Vector2f GetPosition() const = 0;
};

// ----------------------------------------------------------------
// Contract: ITelemetry
// Description: Allows the HUD to read scores without changing them.
// ----------------------------------------------------------------
class ITelemetry {
public:
    virtual ~ITelemetry() = default;

    virtual int GetScore() const = 0;
    virtual int GetMisses() const = 0;
    virtual float GetAccuracy() const = 0;
};

// ----------------------------------------------------------------
// Contract: IAssetSupplier
// Description: Provides resources so entities don't load them manually.
// ----------------------------------------------------------------
class IAssetSupplier {
public:
    virtual ~IAssetSupplier() = default;

    virtual const sf::Texture& GetTexture(const std::string& name) const = 0;
    virtual const sf::SoundBuffer& GetSound(const std::string& name) const = 0;
    virtual const sf::Font& GetFont(const std::string& name) const = 0;
};