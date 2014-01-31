#include "Fission/Rendering/RenderLayer.h"

#include <SFML/Graphics/RenderTarget.hpp>

RenderLayer::RenderLayer()
{
    // TODO: More elegance plz
    // sf::Points is first in sf::PrimitiveType enum, sf::Quads is last.
    for (int prim = sf::Points; prim <= sf::Quads; prim++)
        mBatches[prim].setPrimitiveType(static_cast<sf::PrimitiveType>(prim));
}

RenderLayer::~RenderLayer()
{
    //dtor
}

void RenderLayer::render(sf::RenderTarget& target, const sf::RenderStates& states)
{
    // TODO: More elegance plz
    for (int prim = sf::Points; prim <= sf::Quads; prim++)
        target.draw(mBatches[prim], states);
}
