#include "../include/ResourceManager.h"
#include <iostream>

ResourceManager::ResourceManager() {
    // Constructor
}

ResourceManager::~ResourceManager() {
    // Destructor (Maps clean themselves up automatically)
}

// ----------------------------------------------------------------
// Loading Methods
// ----------------------------------------------------------------

bool ResourceManager::LoadTexture(const std::string& name, const std::string& filename) {
    sf::Texture tex;

    if (!tex.loadFromFile("assets/textures/" + filename)) {
        std::cerr << "[ResourceManager] Error: Failed to load texture: " << filename << std::endl;
        return false;
    }
    
    // Store in the map
    m_textures[name] = tex;
    std::cout << "[ResourceManager] Loaded Texture: " << name << std::endl;
    return true;
}

bool ResourceManager::LoadSound(const std::string& name, const std::string& filename) {
    sf::SoundBuffer buffer;

    if (!buffer.loadFromFile("assets/sounds/" + filename)) {
        std::cerr << "[ResourceManager] Error: Failed to load sound: " << filename << std::endl;
        return false;
    }

    m_sounds[name] = buffer;
    std::cout << "[ResourceManager] Loaded Sound: " << name << std::endl;
    return true;
}

bool ResourceManager::LoadFont(const std::string& name, const std::string& filename) {
    sf::Font font;
    if (!font.loadFromFile("assets/fonts/" + filename)) {
        std::cerr << "[ResourceManager] Error: Failed to load font: " << filename << std::endl;
        return false;
    }

    m_fonts[name] = font;
    std::cout << "[ResourceManager] Loaded Font: " << name << std::endl;
    return true;
}

// ----------------------------------------------------------------
// Retrieval Methods
// ----------------------------------------------------------------

const sf::Texture& ResourceManager::GetTexture(const std::string& name) const {
    auto found = m_textures.find(name);
    if (found == m_textures.end()) {
        std::cerr << "[ResourceManager] CRITICAL: Texture not found: " << name << std::endl;
        
        // Safety Check: If map is empty, we cannot return begin().
        if (m_textures.empty()) {
            // We are in trouble. Throw an exception or exit.
            // For this assignment, we might just crash, but let's print why.
            std::cerr << "[ResourceManager] FATAL: No textures loaded! Check assets folder." << std::endl;
            exit(-1); 
        }
        
        return m_textures.begin()->second; 
    }
    return found->second;
}

const sf::SoundBuffer& ResourceManager::GetSound(const std::string& name) const {
    auto found = m_sounds.find(name);
    if (found == m_sounds.end()) {
        std::cerr << "[ResourceManager] CRITICAL: Sound not found: " << name << std::endl;

        if (m_sounds.empty()) {
            std::cerr << "[ResourceManager] FATAL: No sounds loaded! Check assets folder." << std::endl;
            exit(-1); 
        }

        return m_sounds.begin()->second;
    }
    return found->second;
}

const sf::Font& ResourceManager::GetFont(const std::string& name) const {
    auto found = m_fonts.find(name);
    if (found == m_fonts.end()) {
        std::cerr << "[ResourceManager] CRITICAL: Font not found: " << name << std::endl;

        if (m_fonts.empty()) {
            std::cerr << "[ResourceManager] FATAL: No fonts loaded! Check assets folder." << std::endl;
            exit(-1); 
        }

        return m_fonts.begin()->second;
    }
    return found->second;
}