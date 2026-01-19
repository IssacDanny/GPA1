#pragma once
#include <SFML/Graphics.hpp>
#include "Interface.h"

class HUD {
public:
    // Dependency Injection: Needs Telemetry (Data) and Assets (Font)
    HUD(const ITelemetry* telemetry, IAssetSupplier* assetSupplier);
    ~HUD() = default;

    // Updates the text strings based on current stats
    void Update();

    // Draws the text to the screen
    void Render(sf::RenderWindow& window);

private:
    const ITelemetry* m_telemetry;
    
    // Visual Elements
    sf::Text m_scoreText;
    sf::Text m_statsText; // For Misses and Accuracy
    
    // Internal Helper to format the string
    void InitText(sf::Text& text, const sf::Font& font, int size, sf::Vector2f pos, sf::Color color);
};