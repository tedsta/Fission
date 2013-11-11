#ifndef ITEMCOMPONENT_H
#define ITEMCOMPONENT_H

#include <Sqrat/sqrat.h>
#include <SFML/Graphics/Texture.hpp>
#include "Core/Component.h"

class Scene;
class Entity;

class Item
{
    public:
        Item(std::string name, sf::Texture* texture, HSQUIRRELVM vm, std::string useFunc);

        void use();

        Entity* spawn(Scene* scene, sf::Vector2f pos);

    private:
        std::string mName;
        sf::Texture* mTexture;
        Sqrat::Function mUseFunc;
};

class ItemComponent : public Component
{
    public:
        ItemComponent();
        virtual ~ItemComponent();

        static TypeBits Type;
        const TypeBits getTypeBits() const {return Type;}
        static Component* factory() {return new ItemComponent;}

    private:
};

#endif // ITEMCOMPONENT_H
