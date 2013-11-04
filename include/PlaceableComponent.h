#ifndef PLACEABLECOMPONENT_H
#define PLACEABLECOMPONENT_H

#include "Core/Component.h"

class PlaceableComponent : public Component
{
    public:
        PlaceableComponent();
        virtual ~PlaceableComponent();

        static TypeBits Type;
        const TypeBits getTypeBits() const {return Type;}
        static Component* factory() {return new PlaceableComponent;}

    private:
};

#endif // PLACEABLECOMPONENT_H
