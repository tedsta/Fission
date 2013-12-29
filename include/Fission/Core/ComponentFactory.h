#ifndef COMPONENTFACTORY_H
#define COMPONENTFACTORY_H

#include <unordered_map>
#include <typeinfo>
#include <assert.h>
#include <bitset>
#include "Fission/Core/ComponentType.h"
#include "Fission/Core/Component.h"

class ComponentFactory
{
    public:
        static void deleteComponentTypes();

        template<typename c>
        static void registerComponent()
        {
            assert((std::is_base_of< Component, c >::value == true));

            const std::type_info& t = typeid(c);
            ComponentType* type = mComponentTypes[t.hash_code()];

            if(type == NULL) {
                type = new ComponentType();
                mComponentTypes[t.hash_code()] = type;
            }
        }

        static ComponentType& getTypeFor(const std::type_info &t);

        // Gets the component type object
        template<typename c>
        static ComponentType& getTypeFor()
        {
            //Check if we are being legal with components and shizzle
            //Component * c = (component*)0;

            assert((std::is_base_of<Component, c >::value == true));

            return getTypeFor(typeid(c));
        }

        // Gets the bit set of a component
        template<typename c>
        static std::bitset<MAX_COMPONENTS> getBit() {

            //Check if we are being legal with components and shizzle
            //Component * c = (component*)0;

            assert((std::is_base_of< Component, c >::value == true));
            return getTypeFor(typeid(c)).getBit();
        }

        // Gets the component ID
        template<typename c>
        static int getID()
        {
            //Check if we are being legal with components and shizzle

            assert((std::is_base_of< Component, c >::value == true));

            return getTypeFor(typeid(c)).getID();
        };

        static int getTypeCount()
        {
            return mComponentTypes.size();
        }

    private:
        ComponentFactory();
        static std::unordered_map<size_t, ComponentType*> mComponentTypes;
};

#endif // COMPONENTFACTORY_H
