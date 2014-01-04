#ifndef DEBUGDISPLAY_H
#define DEBUGDISPLAY_H

#include <string>
#include <map>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

namespace fsn
{
    class DebugDisplay
    {
        public:
            DebugDisplay(sf::Font* font);
            virtual ~DebugDisplay();

            void render(sf::RenderTarget& target);

            void setMessage(std::string name, std::string msg){mMessages[name]=msg;}

        private:
            sf::Font* mFont;
            std::map<std::string, std::string> mMessages;
    };
}

#endif // DEBUGDISPLAY_H
