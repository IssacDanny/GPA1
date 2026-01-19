#include "../include/ZombieManager.h"
#include "../include/StandardZombie.h"
#include <iostream>
#include <cstdlib> // For rand()
#include <ctime>   // For time()

// Configuration
const int MAX_ZOMBIES = 6; // One for each spawn point
const float MIN_SPAWN_TIME = 0.5f;
const float MAX_SPAWN_TIME = 1.5f;

ZombieManager::ZombieManager(IAssetSupplier* assetSupplier) 
    : m_spawnTimer(0.0f), m_nextSpawnTime(1.0f)
{
    // Seed the random number generator
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    InitializeSpawnPoints();

    // --- SETUP GRAVE SPRITE ---
    const sf::Texture& graveTex = assetSupplier->GetTexture("grave");
    m_graveSprite.setTexture(graveTex);

    // Center the origin (Crucial for aligning with spawn points)
    sf::FloatRect bounds = m_graveSprite.getLocalBounds();
    m_graveSprite.setOrigin(bounds.width / 2.0f, bounds.height / 2.0f);

    // Optional: Scale it to be slightly smaller than the zombie (e.g., 80px)
    // Assuming the grave image is roughly the same size as the zombie image
    float targetSize = 80.0f; 
    float scale = targetSize / bounds.width;
    m_graveSprite.setScale(scale, scale);
    // --------------------------

    // Create the Zombie Pool
    // We create one zombie for each spawn point to prevent overlap.
    // They start in the "Hidden" state.
    const sf::Texture& zombieTexture = assetSupplier->GetTexture("zombie_head"); // Ensure this name matches your asset!

    for (const auto& point : m_spawnPoints) {
        m_zombies.push_back(std::make_unique<StandardZombie>(zombieTexture, point));
    }
}

ZombieManager::~ZombieManager() {
    // unique_ptr handles cleanup automatically
}

void ZombieManager::InitializeSpawnPoints() {
    // Define 6 distinct positions on an 800x600 screen.
    // We arrange them in a grid for tidiness.
    // Row 1
    m_spawnPoints.emplace_back(200.0f, 200.0f);
    m_spawnPoints.emplace_back(400.0f, 200.0f);
    m_spawnPoints.emplace_back(600.0f, 200.0f);
    
    // Row 2
    m_spawnPoints.emplace_back(200.0f, 400.0f);
    m_spawnPoints.emplace_back(400.0f, 400.0f);
    m_spawnPoints.emplace_back(600.0f, 400.0f);
}

int ZombieManager::Update(float deltaTime) {
    int missesThisFrame = 0;

    // 1. Update all existing zombies
    for (auto& zombie : m_zombies) {
        // If Update() returns true, it means the zombie escaped.
        if (zombie->Update(deltaTime)) {
            missesThisFrame++;
        }
    }

    // 2. Handle Spawning Logic
    m_spawnTimer += deltaTime;
    if (m_spawnTimer >= m_nextSpawnTime) {
        SpawnRandomZombie();
        m_spawnTimer = 0.0f;
        m_nextSpawnTime = MIN_SPAWN_TIME + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (MAX_SPAWN_TIME - MIN_SPAWN_TIME)));
    }

    return missesThisFrame;
}

void ZombieManager::SpawnRandomZombie() {
    // Try to find a zombie that is currently hidden
    // We try a few times to find a free spot. If all are busy, we skip this spawn.
    int attempts = 0;
    while (attempts < 10) {
        int index = std::rand() % m_zombies.size();
        
        // We need to cast to StandardZombie to access the Spawn() method
        // because the vector holds IWhackable pointers.
        // In a pure ECS, we would handle this differently, but dynamic_cast is safe here.
        StandardZombie* z = dynamic_cast<StandardZombie*>(m_zombies[index].get());
        
        if (z && !z->IsActive()) {
            z->Spawn();
            return; // Success
        }
        attempts++;
    }
    // If we get here, the screen is likely full of zombies.
}

void ZombieManager::Render(sf::RenderWindow& window) {
    // 1. Draw Graves at all spawn points
    for (const auto& point : m_spawnPoints) {
        m_graveSprite.setPosition(point);
        window.draw(m_graveSprite);
    }

    // 2. Draw Zombies (on top of graves)
    for (auto& zombie : m_zombies) {
        zombie->Render(window);
    }
}

bool ZombieManager::HandleClick(int x, int y) {
    // Check if the click hit any ACTIVE zombie
    // We iterate backwards so if zombies overlap, we hit the one "on top" first.
    for (auto it = m_zombies.rbegin(); it != m_zombies.rend(); ++it) {
        if ((*it)->TryHit(x, y)) {
            return true; // Hit registered!
        }
    }
    return false; // Missed everything
}