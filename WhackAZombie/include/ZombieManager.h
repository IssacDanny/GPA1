#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include "Interface.h"

class ZombieManager {
public:
    // Dependency Injection: We need the AssetSupplier to get textures.
    ZombieManager(IAssetSupplier* assetSupplier);
    ~ZombieManager();

    // The Game Loop hooks
    int Update(float deltaTime);
    void Render(sf::RenderWindow& window);

    // Input Handling
    // Returns true if a zombie was successfully hit.
    bool HandleClick(int x, int y);

private:
    // The Workforce
    std::vector<std::unique_ptr<IWhackable>> m_zombies;
    
    // The Locations (The 6+ required spawn points)
    std::vector<sf::Vector2f> m_spawnPoints;
    sf::Sprite m_graveSprite;
    
    // Spawning Logic
    float m_spawnTimer;
    float m_nextSpawnTime;

    // Internal Helper
    void SpawnRandomZombie();
    void InitializeSpawnPoints();
};