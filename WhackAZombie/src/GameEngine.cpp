#include "../include/GameEngine.h"
#include "../include/ResourceManager.h"
#include "../include/ZombieManager.h"
#include "../include/HUD.h"
#include <iostream>

// Screen Dimensions
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

GameEngine::GameEngine() 
    : m_isRunning(true), m_score(0), m_misses(0), m_isMuted(false)
{
    // 1. Setup Window
    m_window.create(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Whack-a-Zombie | Assignment 1");
    m_window.setFramerateLimit(60); 

    // 2. Initialize Sub-systems (CREATE THE MANAGER FIRST)
    m_resourceManager = std::make_unique<ResourceManager>();
    
    // 3. LOAD ASSETS (FILL THE MANAGER)
    m_resourceManager->LoadTexture("zombie_head", "zombie.png"); 
    m_resourceManager->LoadTexture("background", "background.png");
    m_resourceManager->LoadTexture("grave", "grave.png"); 
    m_resourceManager->LoadFont("game_font", "arial.ttf");

    // --- LOAD AUDIO ---
    // Load the short sound into the manager
    m_resourceManager->LoadSound("hit", "hit.mp3"); 
    
    // Setup the Hit Sound Player
    m_hitSound.setBuffer(m_resourceManager->GetSound("hit"));
    
    // Setup the Background Music (Stream directly)
    if (!m_backgroundMusic.openFromFile("assets/sounds/music.mp3")) {
        std::cerr << "[GameEngine] Error: Could not load music.mp3" << std::endl;
    } else {
        m_backgroundMusic.setLoop(true); // Make it repeat forever
        m_backgroundMusic.setVolume(50.0f); // 50% volume
        m_backgroundMusic.play();
    } 

    // 4. Setup the Background Sprite
    m_backgroundSprite.setTexture(m_resourceManager->GetTexture("background"));

    // --- STERLING'S SCALING LOGIC ---
    // Get the original size of the image
    sf::FloatRect spriteSize = m_backgroundSprite.getLocalBounds();

    // Calculate how much to stretch it to fit the screen (800x600)
    // Scale = Target Size / Original Size
    float scaleX = static_cast<float>(SCREEN_WIDTH) / spriteSize.width;
    float scaleY = static_cast<float>(SCREEN_HEIGHT) / spriteSize.height;

    // Apply the scale
    m_backgroundSprite.setScale(scaleX, scaleY);

    // 5. Initialize Managers
    m_zombieManager = std::make_unique<ZombieManager>(m_resourceManager.get());
    m_hud = std::make_unique<HUD>(this, m_resourceManager.get());
}

GameEngine::~GameEngine() {
    // Unique_ptrs clean themselves up
}

void GameEngine::Run() {
    sf::Clock clock;

    while (m_window.isOpen()) {
        // Calculate Delta Time (Time per frame)
        sf::Time dt = clock.restart();
        float deltaTime = dt.asSeconds();

        ProcessEvents();
        Update(deltaTime);
        Render();
    }
}

void GameEngine::ProcessEvents() {
    sf::Event event;
    while (m_window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            m_window.close();
        }

        // Key Press Handling
        if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::M) {
                m_isMuted = !m_isMuted; // Toggle state
                
                if (m_isMuted) {
                    m_backgroundMusic.pause();
                } else {
                    m_backgroundMusic.play();
                }
            }
        }
        
        // Handle Mouse Click
        if (event.type == sf::Event::MouseButtonPressed) {
            if (event.mouseButton.button == sf::Mouse::Left) {
                // 1. Get the pixel position of the click
                sf::Vector2i pixelPos(event.mouseButton.x, event.mouseButton.y);

                // 2. Convert it to "World" coordinates using the current view
                sf::Vector2f worldPos = m_window.mapPixelToCoords(pixelPos);

                // 3. Cast back to int (since our Manager expects ints)
                int mouseX = static_cast<int>(worldPos.x);
                int mouseY = static_cast<int>(worldPos.y);

                // Pass the translated coordinates to the Zombie Manager
                bool hit = m_zombieManager->HandleClick(mouseX, mouseY);
                
                if (hit) {
                    RegisterHit();
                } else {
                    RegisterMiss();
                }
            }
        }
    }
}

void GameEngine::Update(float deltaTime) {
    // Get the number of passive misses from the manager
    int passiveMisses = m_zombieManager->Update(deltaTime);
    
    // Add to the total
    m_misses += passiveMisses;

    m_hud->Update();
}

void GameEngine::Render() {
    // Draw Background
    m_window.draw(m_backgroundSprite);

    // Draw Zombies
    m_zombieManager->Render(m_window);

    // Draw HUD (Always on top)
    m_hud->Render(m_window);

    m_window.display();
}

// --- ITelemetry Implementation ---

float GameEngine::GetAccuracy() const {
    int totalShots = m_score + m_misses;
    if (totalShots == 0) return 0.0f; // Prevent division by zero
    return (static_cast<float>(m_score) / totalShots) * 100.0f;
}

void GameEngine::RegisterHit() {
    m_score++;
    // Play the sound (if not muted)
    if (!m_isMuted) {
        // Randomize pitch slightly for variety
        // m_hitSound.setPitch(0.9f + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / 0.2f)));
        
        m_hitSound.play();
    }
}

void GameEngine::RegisterMiss() {
    m_misses++;
}