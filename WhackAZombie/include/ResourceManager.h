#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <map>
#include <string>
#include <iostream>
#include "Interface.h"

class ResourceManager : public IAssetSupplier {
public:
    ResourceManager();
    ~ResourceManager();

    // --- The Contract Implementation ---
    // These methods return a reference to the asset.
    // If the asset is not found, they should handle the error gracefully.
    const sf::Texture& GetTexture(const std::string& name) const override;
    const sf::SoundBuffer& GetSound(const std::string& name) const override;
    const sf::Font& GetFont(const std::string& name) const override;

    // --- The Loading Bay ---
    // We call these once at the start of the game to load files from disk.
    // Returns true if successful, false if the file is missing.
    bool LoadTexture(const std::string& name, const std::string& filename);
    bool LoadSound(const std::string& name, const std::string& filename);
    bool LoadFont(const std::string& name, const std::string& filename);

private:
    // The Storage Depots (Maps)
    // We use mutable maps so we can potentially implement "lazy loading" if needed,
    // though pre-loading is preferred for performance.
    std::map<std::string, sf::Texture> m_textures;
    std::map<std::string, sf::SoundBuffer> m_sounds;
    std::map<std::string, sf::Font> m_fonts;
};