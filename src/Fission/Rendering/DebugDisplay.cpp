#include <Fission/Rendering/DebugDisplay.h>

#include <SFML/Graphics/Text.hpp>

namespace fsn
{
    DebugDisplay::DebugDisplay(sf::Font* font) : mFont(font)
    {
        //ctor
    }

    DebugDisplay::~DebugDisplay()
    {
        //dtor
    }

    void DebugDisplay::render(sf::RenderTarget& target)
    {
        int y = 0;
        for (auto it : mMessages)
        {
            sf::Text msg;
            msg.setFont(*mFont);
            msg.setString(it.first+": "+it.second);
            msg.setCharacterSize(24);
            msg.setColor(sf::Color::White);
            msg.setPosition(5, y);
            target.draw(msg);
            y += 30;
        }
    }
}

