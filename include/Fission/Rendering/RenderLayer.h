#ifndef RENDERLAYER_H
#define RENDERLAYER_H

#include <SFML/Graphics/VertexArray.hpp>

class RenderLayer
{
    public:
        RenderLayer();
        ~RenderLayer();

        void render(sf::RenderTarget& target, const sf::RenderStates& states);

        void drawPoint(const sf::Vector2f& point);
        void drawLine(const sf::Vector2f& a, const sf::Vector2f& b);

    private:
        // This doesn't really feel right, but we get the proper vertex array using
        // sf::PrimitiveType objects as the indices. Since the last element in the enum is Quads,
        // we simply use sf::Quads+1 as the array size.
        // TODO: Do this better.
        sf::VertexArray mBatches[sf::Quads+1];
};

#endif // RENDERLAYER_H
