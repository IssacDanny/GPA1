#include "../include/HUD.h"
#include <string>
#include <iomanip> // For setprecision
#include <sstream> // For string formatting

HUD::HUD(const ITelemetry* telemetry, IAssetSupplier* assetSupplier)
    : m_telemetry(telemetry)
{
    // Get the font
    // Ensure you have a font file named "arial.ttf" (or similar) in assets/fonts/
    const sf::Font& font = assetSupplier->GetFont("game_font");

    // Initialize Score Text (Top Left)
    InitText(m_scoreText, font, 30, sf::Vector2f(20.f, 10.f), sf::Color::White);

    // Initialize Stats Text (Below Score)
    InitText(m_statsText, font, 20, sf::Vector2f(20.f, 50.f), sf::Color(200, 200, 200));
}

void HUD::InitText(sf::Text& text, const sf::Font& font, int size, sf::Vector2f pos, sf::Color color) {
    text.setFont(font);
    text.setCharacterSize(size);
    text.setPosition(pos);
    text.setFillColor(color);
}

void HUD::Update() {
    // 1. Fetch Data
    int score = m_telemetry->GetScore();
    int misses = m_telemetry->GetMisses();
    float accuracy = m_telemetry->GetAccuracy();

    // 2. Format Score
    m_scoreText.setString("Score: " + std::to_string(score));

    // 3. Format Stats (Misses & Accuracy)
    // We use a stringstream to format the float to 1 decimal place
    std::stringstream ss;
    ss << "Misses: " << misses << " | Accuracy: " << std::fixed << std::setprecision(1) << accuracy << "%";
    
    m_statsText.setString(ss.str());
}

void HUD::Render(sf::RenderWindow& window) {
    window.draw(m_scoreText);
    window.draw(m_statsText);
}