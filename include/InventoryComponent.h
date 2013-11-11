#ifndef INVENTORYCOMPONENT_H
#define INVENTORYCOMPONENT_H

#include "Core/Component.h"

class InventoryComponent : public Component
{
    public:
        InventoryComponent();
        virtual ~InventoryComponent();

        static TypeBits Type;
        const TypeBits getTypeBits() const {return Type;}
        static Component* factory() {return new InventoryComponent;}

    private:
};

#endif // INVENTORYCOMPONENT_H
